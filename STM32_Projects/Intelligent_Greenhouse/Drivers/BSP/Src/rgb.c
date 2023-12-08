#include "..\Inc\rgb.h"

const RGBColor_TypeDef RED      =  {255, 0, 0};
const RGBColor_TypeDef GREEN    =  {0, 255, 0};
const RGBColor_TypeDef BLUE     =  {0, 0, 255};
const RGBColor_TypeDef SKY      =  {0, 255, 255};
const RGBColor_TypeDef MAGENTA  =  {255, 0, 255};
const RGBColor_TypeDef YELLOW   =  {255, 255, 0};
const RGBColor_TypeDef ORANGE   =  {127, 106, 0};
const RGBColor_TypeDef BLACK    =  {0, 0, 0};
const RGBColor_TypeDef WHITE    =  {255, 255, 255};
const RGBColor_TypeDef PURPLE   =  {65, 105, 225};

static uint8_t rgb_light_beads_buf[RGB_LIGHT_BEADS_NUM][24] = {0};                             /* 灯珠颜色配置数组 */

SPI_HandleTypeDef spi_rgb_strip_light_handle = {0};                  /* SPI1句柄 */

/**
 * @brief 配置SPI_RGB_Strip_Light片选信号
*/
static void spi_rgb_strip_light_cs_status(SPI_CS_Status status)
{
    HAL_GPIO_WritePin(SPI_RGB_STRIP_LIGHT_CS_PORT, SPI_RGB_STRIP_LIGHT_CS_PIN, (GPIO_PinState)status);
}
/**
 * @brief       SPI_RGB_Strip_Light初始化函数
 * @note        主机模式,8位数据,禁止硬件片选
 * @param       无
 * @retval      无
 */
void spi_rgb_strip_light_init(void)
{
    spi_rgb_strip_light_handle.Instance = SPI_RGB_STRIP_LIGHT_SPI;                 /* SPI1 */
    spi_rgb_strip_light_handle.Init.Mode = SPI_MODE_MASTER;                        /* 设置SPI工作模式,设置为主模式:STM32作为主设备 */
    spi_rgb_strip_light_handle.Init.Direction = SPI_DIRECTION_2LINES;              /* 设置SPI单向或者双向的数据模式:SPI设置为双线全双工 */
    spi_rgb_strip_light_handle.Init.DataSize = SPI_DATASIZE_8BIT;                  /* 设置SPI的数据大小:SPI发送/接收8bit帧结构 */
    spi_rgb_strip_light_handle.Init.CLKPolarity = SPI_POLARITY_LOW;                /* 串行同步时钟的空闲状态为高电平 */
    spi_rgb_strip_light_handle.Init.CLKPhase = SPI_PHASE_2EDGE;                    /* 串行同步时钟的第二个跳变沿(上升沿)数据被采样 */
    spi_rgb_strip_light_handle.Init.NSS = SPI_NSS_SOFT;                            /* NSS信号由硬件(NSS管脚)还是软件(使用SSI位)管理:内部NSS信号由SSI位控制,即软件管理 */
    spi_rgb_strip_light_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;   /* 定义波特率预分频的值:波特率预分频值为4 */
    spi_rgb_strip_light_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;                   /* 指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始,由Flash芯片规定 */
    spi_rgb_strip_light_handle.Init.TIMode = SPI_TIMODE_DISABLE;                   /* 关闭TI模式:使用摩托罗拉模式 */
    spi_rgb_strip_light_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;   /* 关闭硬件CRC校验 */
    spi_rgb_strip_light_handle.Init.CRCPolynomial = 7;                             /* CRC值计算的多项式 */
    HAL_SPI_Init(&spi_rgb_strip_light_handle);                                     /* 初始化 */

    __HAL_SPI_ENABLE(&spi_rgb_strip_light_handle);                                 /* 使能SPI2 */

    spi_rgb_strip_light_cs_status(Low);                                            /* 拉低片选信号,虽然用不到CS信号 */
}

/**
 * @brief  设置灯带上灯珠的颜色
 * @note   配置rgb_light_beads_buf数组
 * @param  rgb_light_bead_id:灯珠序号,0 ~ 255 
 * @param  rgb_light_bead_color:灯珠颜色,RGBColor_TypeDef结构体
 * @retval 无
*/
static void spi_rgb_strip_light_setcolor(uint8_t rgb_light_bead_id, RGBColor_TypeDef rgb_light_bead_color)
{
	uint16_t i = 0;         /*循环变量*/

    /*存储G7 ~ G0,对应array[id][0] ~ array[id][7]*/
    for (i = 0; i < 8; i++)
    {
        rgb_light_beads_buf[rgb_light_bead_id][i] = (((rgb_light_bead_color.Green & (1 << (7 - i)))) ?   WS2818B_1 : WS2818B_0);   
    }
    /*存储R7 ~ R0,对应array[id][8] ~ array[id][15]*/
    for (i = 8; i < 16; i++)
    {
        rgb_light_beads_buf[rgb_light_bead_id][i] = (((rgb_light_bead_color.Red & (1 << (15 - i)))) ?   WS2818B_1 : WS2818B_0);
    }
    /*存储B7 ~ B0,对应array[id][16] ~ array[id][23]*/
    for (i = 16; i < 24; i++)
    {
        rgb_light_beads_buf[rgb_light_bead_id][i] = (((rgb_light_bead_color.Blue & (1 << (23 - i)))) ?   WS2818B_1 : WS2818B_0);
    }
    /* 传输过程:G7 G6 G5 ... G0 R7 R6 R5 ... R0 B7 B6 B5 ... B0*/
    /* 因此可以断定,芯片手册中的24bit数据结构应该是G R B,不是R G B,但高位先发是正确的 */
}
/**
 * @brief       SPI读写一个字节数据(SPI初始化之后才能使用)
 * @param       pbuf:要发送的数据
 * @retval      无
 */
static void spi_rgb_strip_light_write_data(uint8_t *pbuf)
{
    HAL_SPI_Transmit(&spi_rgb_strip_light_handle, pbuf, RGB_LIGHT_BEADS_NUM * 24, 1000);     /*通常来讲,timeout设置为1000*/
    /*不需要从WS2812B中接收数据*/
}
/**
 * @brief  让RGB灯带显示某个颜色
 * @param  rgb_strip_light_color:待显示颜色
 * @retval 无
*/
void spi_rgb_strip_light_display_color(RGBColor_TypeDef rgb_strip_light_color)
{
    uint8_t i = 0;          /*循环变量*/
    uint8_t k = 0;
    for (i = 0 ; i < RGB_LIGHT_BEADS_NUM; i++)
    {
        spi_rgb_strip_light_setcolor(i, rgb_strip_light_color);
    }
    spi_rgb_strip_light_write_data((uint8_t *)rgb_light_beads_buf);
}
