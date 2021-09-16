#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "nvs_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "E2prom.h"
#include "Uart0.h"
#include "driver/uart.h"

static const char *TAG = "i2c-example";
static const char *TAG1 = "uart-example";
static const char *TAG2 = "uart0-example";
uint8_t E2Prom_Test_Write[] = {0x04, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0E};
uint8_t E2Prom_Test_Write_FF[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t E2Prom_Test_Write_00[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
uint8_t E2Prom_Test_Read[32] = {0x00};
uint8_t E2Prom_Test_Read1[32] = {0x00};
#define BLINK_BLUE_GPIO 21
#define BLINK_Yellow_GPIO 17
uint8_t UART1_RX_data[9];
int length = 0;

void app_uart_task(void *pvParameters)
{
    while (1)
    {
        length = uart_read_bytes(UART_NUM_0, UART1_RX_data, 9, 20 / portTICK_RATE_MS); //串口接收API
        
        ESP_LOG_BUFFER_HEX(TAG1, UART1_RX_data, 9);
        ESP_LOGI(TAG, "%d", __LINE__);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

static void i2c_test_task(void *arg)
{
    while (1)
    {

        if (length)
        {
            ESP_LOGI(TAG,"the data about rev %d",length);
            E2P_Write(PRODUCT_ID_ADDR, E2Prom_Test_Write_FF, 9);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            ESP_LOGE(TAG, "%d", __LINE__);
            E2P_Write(PRODUCT_ID_ADDR, UART1_RX_data, 9);
            ESP_LOG_BUFFER_HEX(TAG2, UART1_RX_data, 9);
            length = 0;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
        ESP_LOGE(TAG, "%d", __LINE__);
        E2P_Read(PRODUCT_ID_ADDR, E2Prom_Test_Read, 9);
        ESP_LOGI(TAG, "%d", __LINE__);
        esp_log_buffer_hex(TAG, E2Prom_Test_Read, 9);
        E2P_Write(SERISE_NUM_ADDR, E2Prom_Test_Write, 9);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        E2P_Read(SERISE_NUM_ADDR, E2Prom_Test_Read1, 9);
        esp_log_buffer_hex(TAG, E2Prom_Test_Read1, 9);
        ESP_LOGE(TAG, "%d", __LINE__);
        if ((E2Prom_Test_Read[0] != 0x01) || (E2Prom_Test_Read[1] != 0x02) || (E2Prom_Test_Read[2] != 0x03) || (E2Prom_Test_Read[3] != 0x04) || (E2Prom_Test_Read[4] != 0x05) || (E2Prom_Test_Read[5] != 0x06) || (E2Prom_Test_Read[6] != 0x07) || (E2Prom_Test_Read[7] != 0x08) || (E2Prom_Test_Read[8] != 0x0A))
        {
            ESP_LOGE(TAG, "Failed to  EEPROM");
            gpio_set_level(BLINK_Yellow_GPIO, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(BLINK_Yellow_GPIO, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        else
        {
            ESP_LOGI(TAG, "Success to  EEPROM");
            gpio_set_level(BLINK_BLUE_GPIO, 0);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            gpio_set_level(BLINK_BLUE_GPIO, 1);
            vTaskDelay(100 / portTICK_PERIOD_MS);
        }
        ESP_LOGE(TAG, "%d", __LINE__);
    }

    vTaskDelete(NULL);
}

void app_main(void)
{
    #if  E2PROM_WP_EN
	gpio_pad_select_gpio(E2PROM_WP_GPIO);
    gpio_set_direction(E2PROM_WP_GPIO, GPIO_MODE_OUTPUT);
	gpio_set_level(E2PROM_WP_GPIO, 1);
	#endif

    gpio_pad_select_gpio(BLINK_BLUE_GPIO);
    gpio_set_direction(BLINK_BLUE_GPIO, GPIO_MODE_OUTPUT);
    gpio_pad_select_gpio(BLINK_Yellow_GPIO);
    gpio_set_direction(BLINK_Yellow_GPIO, GPIO_MODE_OUTPUT);

    E2prom_Init();
    Uart0_Init();
    gpio_set_level(BLINK_Yellow_GPIO, 0);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(BLINK_Yellow_GPIO, 1);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    xTaskCreate(i2c_test_task, "i2c_test_task_0", 1024 * 2, NULL, 1, NULL);
    xTaskCreate(app_uart_task, "uart_task", 4096, NULL, 3, NULL);
}
