# 指定交叉编译 ARM 平台
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# 跳过编译器链接测试，仅测试编译器是否能生成静态库，防止 CMake 在配置阶段测试编译器时报错
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 指定 ARM 交叉编译器
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_SIZE arm-none-eabi-size)