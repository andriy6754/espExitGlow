idf_component_register(
    SRCS "weather.c" "time.c" "air_alert.c" "main.c" #to add new files just add another "<file_name>"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio led_strip
)
