/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "driver/spi_master.h"

#ifdef CONFIG_IDF_TARGET_ESP32
#define MSP430    HSPI_HOST

#define PIN_NUM_MISO 25
#define PIN_NUM_MOSI 23
#define PIN_NUM_CLK  19
#define PIN_NUM_CS   22

#define PIN_NUM_DC   21
#define PIN_NUM_RST  18
#define PIN_NUM_BCKL 5
#endif

#define PARALLEL_LINES 16


void app_main(void)
{
    printf("Hello world!\n");

    esp_err_t ret;
    spi_device_handle_t spi;
    spi_bus_config_t buscfg={
        .miso_io_num=PIN_NUM_MISO,
        .mosi_io_num=PIN_NUM_MOSI,
        .sclk_io_num=PIN_NUM_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=PARALLEL_LINES*320*2+8
    };

    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10000000, //Clock out at 10 MHz
        .mode=0,                                //SPI mode 0
        .spics_io_num=PIN_NUM_CS,               //CS pin
        .queue_size=1,                          //We want to be able to queue 1 transactions at a time
    };

    spi_transaction_t data; //TODO
    

    // Initialize the SPI bus

    ret = spi_bus_initialize(MSP430, &buscfg, 0); //Disable DMA
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(MSP430, &devcfg, &spi);
    ESP_ERROR_CHECK(ret);

    //ret = spi_device_transmit(MSP430, ) TODO

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
