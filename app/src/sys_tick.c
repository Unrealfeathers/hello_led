#include "sys_tick.h"

// 倒计时变量，__IO (volatile) 防止被编译器过度优化
static __IO uint32_t delay;

void delay_1ms(uint32_t count)
{
    delay = count;

    while (0U != delay);
}

void delay_decrement()
{
    if (0U != delay) {
        delay--;
    }
}

void led_spark()
{
    static __IO uint32_t led_timing = 0;

    if (led_timing++ >= 500) {
        led_timing = 0;

        uint8_t current_state = gpio_input_bit_get(GPIOC, GPIO_PIN_13);
        gpio_bit_write(GPIOC, GPIO_PIN_13, (bit_status)(1 - current_state));
    }
}