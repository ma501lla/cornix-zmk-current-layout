#include <stdint.h>

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/led_strip.h>
#include <zephyr/init.h>
#include <zephyr/kernel.h>

#include <zmk/event_manager.h>
#include <zmk/events/layer_state_changed.h>

#define LED_NODE DT_ALIAS(status_ws2812)
#define LED_POWER_NODE DT_ALIAS(cornix_led_power)

#if !DT_NODE_HAS_STATUS(LED_NODE, okay)
#error "cornix_layer9_led_test requires a status-ws2812 LED strip alias"
#endif

#if !DT_NODE_HAS_PROP(LED_POWER_NODE, control_gpios)
#error "cornix_layer9_led_test requires a cornix-led-power alias with control-gpios"
#endif

#define TARGET_LAYER 9
#define LED_COUNT 2
#define POWER_SETTLE_MS 250
#define STARTUP_DELAY_MS 1000

static const struct device *const led_dev = DEVICE_DT_GET(LED_NODE);
static const struct gpio_dt_spec led_power = GPIO_DT_SPEC_GET(LED_POWER_NODE, control_gpios);

static struct k_work_delayable show_work;
static struct k_work_delayable startup_work;
static bool layer9_active;

static void set_led_power(bool on) {
    if (device_is_ready(led_power.port)) {
        gpio_pin_set_dt(&led_power, on ? 1 : 0);
    }
}

static void write_pixels(uint8_t red, uint8_t green, uint8_t blue) {
    struct led_rgb pixels[LED_COUNT] = {
        {.r = red, .g = green, .b = blue},
        {.r = red, .g = green, .b = blue},
    };

    led_strip_update_rgb(led_dev, pixels, LED_COUNT);
}

static void show_work_handler(struct k_work *work) {
    if (!layer9_active || !device_is_ready(led_dev)) {
        return;
    }

    set_led_power(true);
    write_pixels(32, 32, 32);
}

static void startup_work_handler(struct k_work *work) {
    if (!device_is_ready(led_dev)) {
        return;
    }

    set_led_power(true);
    k_msleep(POWER_SETTLE_MS);

    for (int i = 0; i < 5; i++) {
        write_pixels(48, 0, 0);
        k_msleep(200);
        write_pixels(0, 0, 0);
        k_msleep(120);
    }

    write_pixels(48, 0, 0);
}

static int cornix_layer9_led_listener(const zmk_event_t *eh) {
    const struct zmk_layer_state_changed *ev = as_zmk_layer_state_changed(eh);

    if (ev == NULL || ev->layer != TARGET_LAYER) {
        return ZMK_EV_EVENT_BUBBLE;
    }

    layer9_active = ev->state;

    if (layer9_active) {
        set_led_power(true);
        k_work_schedule(&show_work, K_MSEC(POWER_SETTLE_MS));
    } else if (device_is_ready(led_dev)) {
        write_pixels(48, 0, 0);
    }

    return ZMK_EV_EVENT_BUBBLE;
}

static int cornix_layer9_led_init(void) {
    k_work_init_delayable(&show_work, show_work_handler);
    k_work_init_delayable(&startup_work, startup_work_handler);

    if (device_is_ready(led_power.port)) {
        gpio_pin_configure_dt(&led_power, GPIO_OUTPUT_ACTIVE);
    }

    k_work_schedule(&startup_work, K_MSEC(STARTUP_DELAY_MS));

    return 0;
}

SYS_INIT(cornix_layer9_led_init, APPLICATION, 90);

ZMK_LISTENER(cornix_layer9_led_test, cornix_layer9_led_listener);
ZMK_SUBSCRIPTION(cornix_layer9_led_test, zmk_layer_state_changed);
