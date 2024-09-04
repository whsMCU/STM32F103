################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/common/cli.c \
../src/common/cli_gui.c \
../src/common/log.c \
../src/common/maths.c \
../src/common/ring_buffer.c 

OBJS += \
./src/common/cli.o \
./src/common/cli_gui.o \
./src/common/log.o \
./src/common/maths.o \
./src/common/ring_buffer.o 

C_DEPS += \
./src/common/cli.d \
./src/common/cli_gui.d \
./src/common/log.d \
./src/common/maths.d \
./src/common/ring_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/%.o src/common/%.su src/common/%.cyclo: ../src/common/%.c src/common/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/bsp" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/common" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src/hw" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/src" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc" -I"C:/Users/jjins/Documents/Project/STM32F103/stm32f103_fw/lib/STM32F1xx_HAL_Driver/Inc/Legacy" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-common

clean-src-2f-common:
	-$(RM) ./src/common/cli.cyclo ./src/common/cli.d ./src/common/cli.o ./src/common/cli.su ./src/common/cli_gui.cyclo ./src/common/cli_gui.d ./src/common/cli_gui.o ./src/common/cli_gui.su ./src/common/log.cyclo ./src/common/log.d ./src/common/log.o ./src/common/log.su ./src/common/maths.cyclo ./src/common/maths.d ./src/common/maths.o ./src/common/maths.su ./src/common/ring_buffer.cyclo ./src/common/ring_buffer.d ./src/common/ring_buffer.o ./src/common/ring_buffer.su

.PHONY: clean-src-2f-common

