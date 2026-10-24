idf_component_register(
    SRCS "main.c" #to add new files just add another "<file_name>"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio
)
