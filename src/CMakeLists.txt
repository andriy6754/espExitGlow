idf_component_register(
    SRCS "weather.c" "clock.c" "air_alert.c" "wifi.c" "led.c" "display.c" "main.c" #to add new files just add another "<file_name>"
    INCLUDE_DIRS "."
    REQUIRES esp_timer esp_driver_gpio led_strip log freertos esp_http_client esp-tls espressif__cjson esp_wifi nvs_flash esp_event esp_netif
)
