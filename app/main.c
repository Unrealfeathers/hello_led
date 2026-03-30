#include "gd32f10x.h"
#include "gd32f10x_rcu.h"
#include "gd32f10x_gpio.h"

#include "FreeRTOS.h"
#include "task.h"

/**
 * @brief 栈溢出钩子函数
 */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // 发生栈溢出时，进入死循环
    while(1);
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

/**
 * @brief FreeRTOS LED 闪烁测试任务
 */
void vTaskLedBlink(void *pvParameters)
{
    while (1) {
        // Reset (低电平) 点亮 LED
        gpio_bit_reset(GPIOC, GPIO_PIN_13);
        vTaskDelay(pdMS_TO_TICKS(1000));

        // Set (高电平) 熄灭 LED
        gpio_bit_set(GPIOC, GPIO_PIN_13);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main()
{
    // 初始化 GPIO
    gpio_config();

    /* 创建 LED 闪烁任务
     * 参数1: 任务函数指针
     * 参数2: 任务名称
     * 参数3: 任务栈大小（单位：字 = 4 字节）
     * 参数4: 传递给任务的参数
     * 参数5: 任务优先级
     * 参数6: 任务句柄
     */
    xTaskCreate(vTaskLedBlink, 
                "LED_BLINK", 
                configMINIMAL_STACK_SIZE, 
                NULL, 
                tskIDLE_PRIORITY + 1, 
                NULL);

    vTaskStartScheduler();

    // 死循环拦截
    while(1);
}