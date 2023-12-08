#include "spi.h"
/**
 * @brief       SPI初始化回调函数
 * @param       hspi:SPI句柄
 * @retval      无
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    GPIO_InitTypeDef gpio_config_struct = {0};
    if (hspi->Instance == SPI_NORFLASH_SPI)
    {
        __HAL_RCC_SPI2_CLK_ENABLE();                                    /* SPI2时钟使能 */
        __HAL_RCC_GPIOB_CLK_ENABLE();                                   /* GPIOB时钟使能 */

        /*配置CS引脚*/
        gpio_config_struct.Pin = SPI_NORFLASH_CS_PIN;
        gpio_config_struct.Mode = GPIO_MODE_OUTPUT_PP;
        gpio_config_struct.Pull = GPIO_PULLUP;
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_NORFLASH_CS_PORT, &gpio_config_struct);

        /*配置SCK引脚*/
        gpio_config_struct.Pin = SPI_NORFLASH_SCK_PIN;
        gpio_config_struct.Mode = GPIO_MODE_AF_PP;
        gpio_config_struct.Pull = GPIO_PULLUP;                          /*时钟上拉,因为空闲状态是高电平*/
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_NORFLASH_SCK_PORT, &gpio_config_struct);

        /*配置MOSI引脚*/
        gpio_config_struct.Pin = SPI_NORFLASH_MOSI_PIN;
        gpio_config_struct.Mode = GPIO_MODE_AF_PP;
        gpio_config_struct.Pull = GPIO_NOPULL;                         
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_NORFLASH_MOSI_PORT, &gpio_config_struct);

        /*配置MISO引脚*/
        gpio_config_struct.Pin = SPI_NORFLASH_MISO_PIN;
        gpio_config_struct.Mode = GPIO_MODE_AF_INPUT;
        gpio_config_struct.Pull = GPIO_NOPULL;                          
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_NORFLASH_MISO_PORT, &gpio_config_struct);
    }
    else if (hspi->Instance == SPI_RGB_STRIP_LIGHT_SPI)
    {
        __HAL_RCC_SPI2_CLK_ENABLE();                                    /* SPI1时钟使能 */
        __HAL_RCC_GPIOB_CLK_ENABLE();                                   /* GPIOA时钟使能 */

        /*配置CS引脚*/
        gpio_config_struct.Pin = SPI_RGB_STRIP_LIGHT_CS_PIN;
        gpio_config_struct.Mode = GPIO_MODE_OUTPUT_PP;
        gpio_config_struct.Pull = GPIO_PULLUP;
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_RGB_STRIP_LIGHT_CS_PORT, &gpio_config_struct);

        /*配置SCK引脚*/
        gpio_config_struct.Pin = SPI_RGB_STRIP_LIGHT_SCK_PIN;
        gpio_config_struct.Mode = GPIO_MODE_AF_PP;
        gpio_config_struct.Pull = GPIO_PULLUP;                          /*时钟上拉,因为空闲状态是高电平*/
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_RGB_STRIP_LIGHT_SCK_PORT, &gpio_config_struct);

        /*配置MOSI引脚*/
        gpio_config_struct.Pin = SPI_RGB_STRIP_LIGHT_MOSI_PIN;
        gpio_config_struct.Mode = GPIO_MODE_AF_PP;
        gpio_config_struct.Pull = GPIO_PULLDOWN;                         
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_RGB_STRIP_LIGHT_MOSI_PORT, &gpio_config_struct);

        /*配置MISO引脚*/
        gpio_config_struct.Pin = SPI_RGB_STRIP_LIGHT_MISO_PIN;
        gpio_config_struct.Mode = GPIO_MODE_AF_INPUT;
        gpio_config_struct.Pull = GPIO_NOPULL;                          
        gpio_config_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(SPI_RGB_STRIP_LIGHT_MISO_PORT, &gpio_config_struct);  
    }
}
