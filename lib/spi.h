#ifndef __SPI_H
#define __SPI_H
#include "f411.h"

struct spi {
    volatile uint32_t CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR, I2SPR;
};
#define SPI1 ((struct spi *)0x40013000)

/* SPI_SR */
#define SPI_SR_TXE BIT(1)
#define SPI_SR_RXNE BIT(0)
#define SPI_SR_BSY BIT(7)

enum { SPI_BDC_2, SPI_BDC_4, SPI_BDC_8, SPI_BDC_16, SPI_BDC_32, SPI_BDC_64, SPI_BDC_128, SPI_BDC_256 };
enum { SPI_TYPE_SLAVE, SPI_TYPE_MASTER };

typedef struct {
    uint8_t baud_rate_control;
    uint8_t type;
    uint16_t miso_pin;
    uint16_t mosi_pin;
    uint16_t sck_pin;
    uint16_t cs_pin;
} spi_cfg_t;

typedef struct {
    struct spi *port;
    spi_cfg_t cfg;
} spi_dev_t;

void spi_config(spi_dev_t *dev);
void spi_write(spi_dev_t *dev, uint8_t data);
void spi_read(spi_dev_t *dev, uint8_t *data, int size);

#endif /* __SPI_H */