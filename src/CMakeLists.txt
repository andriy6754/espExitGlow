idf_component_register(
    SRCS "weather.c" "clock.c" "air_alert.c" "wifi.c" "led.c" "display.c" "main.c" "colors.c" "led_screen.c" "first_frames.c" #to add new files just add another "<file_name>"
    INCLUDE_DIRS "."
    REQUIRES esp_timer esp_driver_gpio log freertos esp_http_client esp-tls espressif__cjson esp_wifi nvs_flash esp_event esp_netif
)

# load .env file

set(ENV_FILE "${CMAKE_SOURCE_DIR}/.env")
if(EXISTS "${ENV_FILE}")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${ENV_FILE}") # re-run cmake on change
    file(STRINGS "${ENV_FILE}" ENV_LINES REGEX "^[A-Za-z_][A-Za-z0-9_]*=")
    foreach(line IN LISTS ENV_LINES)
        string(REGEX MATCH "^([^=]+)=(.*)$" _ "${line}")
        target_compile_definitions(${COMPONENT_LIB} PRIVATE "${CMAKE_MATCH_1}=\"${CMAKE_MATCH_2}\"")
    endforeach()
else()
    message(WARNING ".env not found - API keys will be undefined")
endif()
