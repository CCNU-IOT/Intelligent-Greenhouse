#ifndef __SPI_H__
#define __SPI_H__
#include "main.h"
/*****************************************************Common***********************************************************/
typedef enum
{
    Low = 0,
    High = 1,
}SPI_CS_Status;
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);
/**********************************************************************************************************************/


/*****************************************************SPI_NOR Flash****************************************************/
#define SPI_NORFLASH_SPI                   SPI2

#define SPI_NORFLASH_CS_PORT               GPIOB
#define SPI_NORFLASH_CS_PIN                GPIO_PIN_12

#define SPI_NORFLASH_SCK_PORT              GPIOB
#define SPI_NORFLASH_SCK_PIN               GPIO_PIN_13

#define SPI_NORFLASH_MISO_PORT             GPIOB
#define SPI_NORFLASH_MISO_PIN              GPIO_PIN_14

#define SPI_NORFLASH_MOSI_PORT             GPIOB
#define SPI_NORFLASH_MOSI_PIN              GPIO_PIN_15

/**********************************************************************************************************************/



/***************************************************SPI_RGB_STRIP_LIGHT************************************************/
#define SPI_RGB_STRIP_LIGHT_SPI                   SPI2

#define SPI_RGB_STRIP_LIGHT_CS_PORT               GPIOB
#define SPI_RGB_STRIP_LIGHT_CS_PIN                GPIO_PIN_12

#define SPI_RGB_STRIP_LIGHT_SCK_PORT              GPIOB
#define SPI_RGB_STRIP_LIGHT_SCK_PIN               GPIO_PIN_13

#define SPI_RGB_STRIP_LIGHT_MISO_PORT             GPIOB
#define SPI_RGB_STRIP_LIGHT_MISO_PIN              GPIO_PIN_14

#define SPI_RGB_STRIP_LIGHT_MOSI_PORT             GPIOB
#define SPI_RGB_STRIP_LIGHT_MOSI_PIN              GPIO_PIN_15
/**********************************************************************************************************************/
#endif
