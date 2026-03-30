#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "gd32f10x.h"

extern uint32_t SystemCoreClock;

/*-----------------------------------------------------------
 * 核心基础配置
 *----------------------------------------------------------*/
#define configUSE_PREEMPTION                    1
#define configUSE_TIME_SLICING                  1
#define configUSE_IDLE_HOOK                     0
#define configUSE_TICK_HOOK                     0
#define configCPU_CLOCK_HZ                      ( SystemCoreClock )
#define configTICK_RATE_HZ                      ( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES                    ( 5 )
#define configMINIMAL_STACK_SIZE                ( ( unsigned short ) 128 )
#define configMAX_TASK_NAME_LEN                 ( 16 )
#define configUSE_16_BIT_TICKS                  0
#define configIDLE_SHOULD_YIELD                 1

/*-----------------------------------------------------------
 * 内存与堆栈分配 (GD32F103VET6 有 64KB SRAM)
 *----------------------------------------------------------*/
#define configSUPPORT_DYNAMIC_ALLOCATION        1
#define configTOTAL_HEAP_SIZE                   ( ( size_t ) ( 48 * 1024 ) )
#define configCHECK_FOR_STACK_OVERFLOW          2

/*-----------------------------------------------------------
 * 调试与断言
 *----------------------------------------------------------*/
#define configUSE_TRACE_FACILITY                1
// 发生致命错误或传参非法时，直接关闭中断并死循环，方便 GDB 抓住现场
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

/*-----------------------------------------------------------
 * 互斥量与信号量
 *----------------------------------------------------------*/
#define configUSE_MUTEXES                       1
#define configUSE_RECURSIVE_MUTEXES             1
#define configUSE_COUNTING_SEMAPHORES           1
#define configQUEUE_REGISTRY_SIZE               8

/*-----------------------------------------------------------
 * 协程 (已废弃，直接关闭)
 *----------------------------------------------------------*/
#define configUSE_CO_ROUTINES                   0
#define configMAX_CO_ROUTINE_PRIORITIES         ( 2 )

/*-----------------------------------------------------------
 * 开启的 API 函数
 *----------------------------------------------------------*/
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskCleanUpResources           0
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1

/*-----------------------------------------------------------
 * Cortex-M3 中断优先级配置 (重点核心)
 *----------------------------------------------------------*/

// GD32F10x 系列的 NVIC 使用 4 位来表示中断优先级
#define configPRIO_BITS                         4

// 库函数可用的最低优先级
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY         15

/* FreeRTOS 可接管的最高中断优先级，设置为 5
 * 优先级 0~4 的中断不会被 FreeRTOS 屏蔽，适合极高实时性要求的裸机中断，
 * 但在这些极高优先级中断里，绝对不能调用任何带有 "FromISR" 后缀的 FreeRTOS API！ */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY    5

// 转换为 Cortex-M3 硬件所需的实际左移值
#define configKERNEL_INTERRUPT_PRIORITY         ( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*-----------------------------------------------------------
 * 映射 FreeRTOS 底层中断
 *----------------------------------------------------------*/
#define vPortSVCHandler     SVC_Handler
#define xPortPendSVHandler  PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
