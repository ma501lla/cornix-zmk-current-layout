if(CONFIG_CORNIX_LAYER9_LED_TEST)
    zephyr_library()
    zephyr_library_sources(src/cornix_layer9_led_test.c)
endif()
