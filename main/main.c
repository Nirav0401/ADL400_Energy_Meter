/* UART asynchronous example, that uses separate RX and TX tasks

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "driver/uart.h"
#include "string.h"
#include "driver/gpio.h"
#include "driver/touch_pad.h"

static const int RX_BUF_SIZE = 1024;
// uint16_t touch_value;

#define read_pin (GPIO_NUM_34)
#define TXD_PIN (GPIO_NUM_32)
#define RXD_PIN (GPIO_NUM_14)
// #define touch_pad (GPIO_NUM_13)

// #define TXD_PIN (GPIO_NUM_4)
// #define RXD_PIN (GPIO_NUM_5)

#define TXD0_PIN (GPIO_NUM_1)
#define RXD0_PIN (GPIO_NUM_3)


// int num = 0;

void init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_driver_install(UART_NUM_0, RX_BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_set_pin(UART_NUM_0, TXD0_PIN, RXD0_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

// int sendData(const char* logName, const char* data)
// {
//    const int len = strlen(data);
//    const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
//    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
//    return txBytes;
// }

// static void tx_task(void *arg)
// {
// //    static const char *TX_TASK_TAG = "TX_TASK";
// //    esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
//     char* Txdata = (char*) malloc(30);
//     while (1) {
// //        sendData(TX_TASK_TAG, "Hello world");
//         // sprintf (Txdata, "Hello world index = %d\r\n", num++);
//         sprintf (Txdata, "Hello world\r\n");
//         uart_write_bytes(UART_NUM_1, Txdata, strlen(Txdata));
//         uart_write_bytes(UART_NUM_0, Txdata, strlen(Txdata));
//         vTaskDelay(2000 / portTICK_PERIOD_MS);
//     }
//         free(Txdata);
// }

// static void rx_task(void *arg)
// {
//     static const char *RX_TASK_TAG = "RX_TASK";
//     esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
//     uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE+1);  // 1 additional for null character
//     while (1) {
//         const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_RATE_MS);
//         if (rxBytes > 0) {
//             data[rxBytes] = "\0";
//             ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
// //            ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
//         }
//     }
//     free(data);
// }

// void app_main(void)
// {
//     init();
//     xTaskCreate(rx_task, "uart_rx_task", 1024*2, NULL, configMAX_PRIORITIES, NULL);
//     xTaskCreate(tx_task, "uart_tx_task", 1024*2, NULL, configMAX_PRIORITIES-1, NULL);

// }

/*
void make_call(const char *phone_number) {
    char command[64];
    sprintf(command, "ATD%s;\r\n", phone_number);
    uart_write_bytes(UART_NUM_1, command, strlen(command));
    vTaskDelay(pdMS_TO_TICKS(10000)); // Wait for call to be established (adjust delay as needed)
    uart_write_bytes(UART_NUM_1, "ATH\r\n", 5); // Hang up the call
}*/


void app_main(void)
{
    init();
        
    while(1)
    {
    char mobileNumber[] = "+911234567890";  // Enter the Mobile Number you want to send the messages to
    char ATcommand[80];
    uint8_t buffer[30] = {0};
    uint8_t ATisOK = 0;

    gpio_set_direction(read_pin, GPIO_MODE_INPUT);
    printf("Enable pin status: %d\n", gpio_get_level(read_pin));

    if(gpio_get_level(read_pin) == 1)
    {
        printf("Enable pin status: %d\n", gpio_get_level(read_pin));

        while(!ATisOK){
            sprintf(ATcommand,"AT\r\n");
            uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
            uart_read_bytes(UART_NUM_1, buffer, 30, 1000 / portTICK_RATE_MS);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            if(strstr((char *)buffer,"OK")){
                ATisOK = 1;
            }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            memset(buffer,0,sizeof(buffer));
        }
        sprintf(ATcommand,"AT+CMGF=1\r\n");  // to convert SMS to text
        uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
        uart_read_bytes(UART_NUM_1, buffer, 30,  1000 / portTICK_RATE_MS);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        memset(buffer,0,sizeof(buffer));

        sprintf(ATcommand,"AT+CMGS=\"%s\"\r\n",mobileNumber); // to send an SMS message
        uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        sprintf(ATcommand,"Alert!, Emergency help Required!\r\n");
        sprintf(ATcommand,"Hello World, STM32 started%c",0x1a); // 0x1a for message terminal ascii code
        uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
        uart_read_bytes(UART_NUM_1, buffer, 30, 1000 / portTICK_RATE_MS);
        memset(buffer,0,sizeof(buffer));
        vTaskDelay(4000 / portTICK_PERIOD_MS);

        // for making call
        sprintf(ATcommand, "ATD%s;\r\n", mobileNumber);
        uart_write_bytes(UART_NUM_1, ATcommand, strlen(ATcommand));
        vTaskDelay(30000 / portTICK_PERIOD_MS); // Wait for call to be established (adjust delay as needed)
        uart_write_bytes(UART_NUM_1, "ATH\r\n", 5); // Hang up the call

        // for receiving the text message from the gsm module
        // sprintf(ATcommand,"AT+CMGF=1\r\n");
        // uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
        // uart_read_bytes(UART_NUM_1, buffer, 30,  1000 / portTICK_RATE_MS);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // memset(buffer,0,sizeof(buffer));

        sprintf(ATcommand,"AT+CMGL=\"ALL\"\r\n");
        uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            sprintf(ATcommand,"AT+CMGR=1\r\n");
            uart_write_bytes(UART_NUM_1, (uint8_t *)ATcommand,strlen(ATcommand));
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            int len = uart_read_bytes(UART_NUM_1, buffer, 30, 1000 / portTICK_PERIOD_MS); // Read UART data
            if (len > 0) {
                printf("Received SMS: %s\n", buffer);
                // Add your SMS parsing logic here to extract SMS content
            }
            vTaskDelay(pdMS_TO_TICKS(1000)); // Delay between SMS checks
        }
        memset(buffer, 0, sizeof(buffer));

        gpio_set_direction(read_pin, GPIO_MODE_OUTPUT);
        gpio_set_level(read_pin, 0);
        printf("Enable pin status: %d\n", gpio_set_level(read_pin, 0));
        }
    } 
} 








