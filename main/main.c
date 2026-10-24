#include <stdio.h>

#include "driver/gpio.h" //for gpio functions
#include "freertos/FreeRTOS.h" //it's required for a vTaskDelay()

const gpio_num_t LED_PIN = GPIO_NUM_4;
const uint32_t SLEEP = 1000;

void app_main(void) // u can't init main function, because it is declared in espidf files
// app_main exact the same as main
{
	uint8_t ledState = 0;
	gpio_reset_pin(LED_PIN);
	gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

	while(1){
		ledState = !ledState;
		gpio_set_level(LED_PIN, ledState);

		printf("LED state: %d\n", ledState);

		vTaskDelay(SLEEP/portTICK_PERIOD_MS);
	}


}
