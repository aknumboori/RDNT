#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

// This program is the fastest way to confirm the mode switch button is operating
// Currently lacking: change the Case LED based on state, Better Debouncing for input button

#define GPIO_INPUT_27 GPIO_NUM_27
#define ESP_INTR_FLAG_DEFAULT 0

static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}

static void gpio_task_example(void *arg)
{
    uint32_t io_num;
    for (;;)
    {
        if (xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY))
        {
            printf("GPIO[%" PRIu32 "] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // zero-initialize the config structure.
    gpio_config_t io_conf = {};

    // bit mask of the pins, use GPIO27 here
    io_conf.pin_bit_mask = (1ULL << GPIO_INPUT_27);
    // set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    // disable pull-down mode
    io_conf.pull_down_en = 0;
    // disable pull-up mode
    io_conf.pull_up_en = 1;
    // interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;

    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t)); // TODO: experiment with queue size
    // start gpio task
    xTaskCreate(gpio_task_example, "gpio_task_example", 2048, NULL, 20, NULL);

    // install gpio isr service
    ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
    // hook isr handler for specific gpio pin
    ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_INPUT_27, gpio_isr_handler, (void *)GPIO_INPUT_27));

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    while (1)
    {
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}