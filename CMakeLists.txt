cmake_minimum_required(VERSION 3.28.3)

# 设置项目名，并开启 C, C++, ASM 语言支持
project(hello_led C CXX ASM)

# ==========================================
# 1. 设置芯片架构 (Cortex-M3)
# ==========================================
set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb")

# ==========================================
# 2. 设置 C/C++/ASM 全局编译选项
# ==========================================
# -O0 关闭优化方便调试，-g 生成调试信息，-Wall 开启所有警告
# -fdata-sections -ffunction-sections 用于后续去除死代码
set(CMAKE_C_FLAGS   "${MCU_FLAGS} -O0 -g -Wall -fdata-sections -ffunction-sections")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -g -x assembler-with-cpp")

# ==========================================
# 3. 设置链接器选项
# ==========================================
# 指定 GCC 链接脚本所在的路径
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/startup/gd32f10x_flash.ld)

# -Wl,--gc-sections 清除未使用的函数和变量段
set(CMAKE_EXE_LINKER_FLAGS "${MCU_FLAGS} -T${LINKER_SCRIPT} -Wl,--gc-sections")

# ==========================================
# 4. 全局宏定义
# ==========================================
add_compile_definitions(
    USE_STDPERIPH_DRIVER # 使用标准外设库驱动
    GD32F10X_HD          # 大容量芯片 (VET6 是 512K Flash)
)

# ==========================================
# 5. 设置头文件搜索路径
# ==========================================
include_directories(
    app/inc
    lib/CMSIS
    lib/CMSIS/GD/GD32F10x/Include
    lib/GD32F10x_standard_peripheral/Include
)

# ==========================================
# 6. 收集所有源码文件
# ==========================================
# 递归收集外设库和应用的源码
file(GLOB_RECURSE LIB_SOURCES "lib/GD32F10x_standard_peripheral/Source/*.c")
file(GLOB_RECURSE APP_SOURCES "app/src/*.c")
file(GLOB_RECURSE APP_SOURCES "app/*.c")

# 手动指定 CMSIS 核心源码和汇编启动文件
set(CMSIS_SOURCES  "lib/CMSIS/GD/GD32F10x/Source/system_gd32f10x.c")
set(STARTUP_SOURCE "startup/startup_gd32f10x_hd.S")

# ==========================================
# 7. 定义并生成最终的可执行文件 (.elf)
# ==========================================
add_executable(${PROJECT_NAME}.elf 
    ${APP_SOURCES}
    ${LIB_SOURCES}
    ${CMSIS_SOURCES}
    ${STARTUP_SOURCE}
)

# --specs=nano.specs  使用精简版 C 库，省 Flash 空间
# --specs=nosys.specs 不使用操作系统底层系统调用
target_link_options(${PROJECT_NAME}.elf PRIVATE
    --specs=nano.specs
    --specs=nosys.specs
)

# ==========================================
# 8. 编译后置动作：生成 .bin 和 .hex 文件
# ==========================================
# 使用 objcopy 将带调试信息的 elf 转换为烧录机需要的 bin 和 hex
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary -S ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMENT "Building .hex and .bin files, and printing size..."
)