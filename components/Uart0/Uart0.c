#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "driver/gpio.h"

#include "Uart0.h"

#define UART0_TXD (UART_PIN_NO_CHANGE)
#define UART0_RXD (UART_PIN_NO_CHANGE)
#define UART0_RTS (UART_PIN_NO_CHANGE)
#define UART0_CTS (UART_PIN_NO_CHANGE)

#define BUF_SIZE 1024
static const char *TAG = "UART0";


void Uart0_Task(void *arg);

void Uart0_Init(void)
{
	uart_config_t uart_config = {
		.baud_rate = 115200,
		.data_bits = UART_DATA_8_BITS,
		.parity = UART_PARITY_DISABLE,
		.stop_bits = UART_STOP_BITS_1,
		.flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
	//这个函数自带  xQueueCreate ();功能，所以在uart使用队列时直接就可以用
	//只需要用QueueHandle_t 创建一个句柄即可
	uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
	uart_param_config(UART_NUM_0, &uart_config);
	uart_set_pin(UART_NUM_0, UART0_TXD, UART0_TXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
	ESP_LOGI(TAG, "START to  uart0");

}

