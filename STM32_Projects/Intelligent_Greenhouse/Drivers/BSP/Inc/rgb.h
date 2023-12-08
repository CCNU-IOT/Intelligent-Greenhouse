#ifndef __RGB_H__
#define __RGB_H__
#include "main.h"
#include "spi.h"
// #include "dma.h"
#define RGB_LIGHT_BEADS_NUM                                 (7)
/* 使用1byte来代替WS2818B的一位:0或1 */ 
/* APB1总线36MHz时钟,SPI2执行8分频,即4.5MHz,每bit的发送时间为222ns*/
#define WS2818B_0                                           (0xC0)    /* 1100 0000,高电平444ns,低电平1332ns,符合WS2818B的0码要求 */               
#define WS2818B_1                                           (0xFC)    /* 1111 1100,高电平1332ns,低电平444ns,符合WS2818B的1码要求 */


typedef struct
{
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
}RGBColor_TypeDef;

extern const RGBColor_TypeDef RED;
extern const RGBColor_TypeDef GREEN;
extern const RGBColor_TypeDef BLUE;
extern const RGBColor_TypeDef SKY;
extern const RGBColor_TypeDef MAGENTA;
extern const RGBColor_TypeDef YELLOW;
extern const RGBColor_TypeDef ORANGE;
extern const RGBColor_TypeDef BLACK;                        /*黑色,同时表示关灯*/
extern const RGBColor_TypeDef WHITE;
extern const RGBColor_TypeDef PURPLE;

void spi_rgb_strip_light_init(void);
void spi_rgb_strip_light_display_color(RGBColor_TypeDef rgb_strip_light_color);

extern SPI_HandleTypeDef spi_rgb_strip_light_handle;
#endif
