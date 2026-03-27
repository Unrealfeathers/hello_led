#include "system_gd32f10x.h"

#include "gd32f10x_rcu.h"
#include "gd32f10x_gpio.h"

/**
 * @brief 简易软件延时函数
 */
void simple_delay(uint32_t count)
{
    while (count--) {
        // 保持时钟周期
        __NOP();
    }
}

/**
 * @brief 初始化 PC13 为推挽输出
 */
void gpio_config()
{
    // 开启 GPIOC 的外设时钟
    rcu_periph_clock_enable(RCU_GPIOC);

    /* 配置 PC13 引脚模式:
     * GPIO_MODE_OUT_PP: 推挽输出
     * GPIO_OSPEED_50MHZ: 50MHz 最大翻转频率
     * GPIO_PIN_13: 指定 13 号引脚
     */
    gpio_init(GPIOC, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
}

int main()
{
    // 初始化 GPIO
    gpio_config();

    while(1) {
        // Reset (低电平) 点亮 LED
        gpio_bit_reset(GPIOC, GPIO_PIN_13);
        simple_delay(0x7FFFFF);

        // Set (高电平) 熄灭 LED
        gpio_bit_set(GPIOC, GPIO_PIN_13);
        simple_delay(0x7FFFFF);
    }
}