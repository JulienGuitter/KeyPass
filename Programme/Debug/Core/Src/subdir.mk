################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DFRobot_ID809.c \
../Core/Src/HID_CDC.c \
../Core/Src/adc.c \
../Core/Src/gpio.c \
../Core/Src/i2c.c \
../Core/Src/lcd_i2c.c \
../Core/Src/main.c \
../Core/Src/password.c \
../Core/Src/spi.c \
../Core/Src/stm32f1xx_hal_msp.c \
../Core/Src/stm32f1xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f1xx.c \
../Core/Src/usart.c \
../Core/Src/usb.c \
../Core/Src/w25qxx.c 

OBJS += \
./Core/Src/DFRobot_ID809.o \
./Core/Src/HID_CDC.o \
./Core/Src/adc.o \
./Core/Src/gpio.o \
./Core/Src/i2c.o \
./Core/Src/lcd_i2c.o \
./Core/Src/main.o \
./Core/Src/password.o \
./Core/Src/spi.o \
./Core/Src/stm32f1xx_hal_msp.o \
./Core/Src/stm32f1xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f1xx.o \
./Core/Src/usart.o \
./Core/Src/usb.o \
./Core/Src/w25qxx.o 

C_DEPS += \
./Core/Src/DFRobot_ID809.d \
./Core/Src/HID_CDC.d \
./Core/Src/adc.d \
./Core/Src/gpio.d \
./Core/Src/i2c.d \
./Core/Src/lcd_i2c.d \
./Core/Src/main.d \
./Core/Src/password.d \
./Core/Src/spi.d \
./Core/Src/stm32f1xx_hal_msp.d \
./Core/Src/stm32f1xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f1xx.d \
./Core/Src/usart.d \
./Core/Src/usb.d \
./Core/Src/w25qxx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Composite -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_KEYBOARD/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DFRobot_ID809.d ./Core/Src/DFRobot_ID809.o ./Core/Src/DFRobot_ID809.su ./Core/Src/HID_CDC.d ./Core/Src/HID_CDC.o ./Core/Src/HID_CDC.su ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/i2c.d ./Core/Src/i2c.o ./Core/Src/i2c.su ./Core/Src/lcd_i2c.d ./Core/Src/lcd_i2c.o ./Core/Src/lcd_i2c.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/password.d ./Core/Src/password.o ./Core/Src/password.su ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f1xx_hal_msp.d ./Core/Src/stm32f1xx_hal_msp.o ./Core/Src/stm32f1xx_hal_msp.su ./Core/Src/stm32f1xx_it.d ./Core/Src/stm32f1xx_it.o ./Core/Src/stm32f1xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f1xx.d ./Core/Src/system_stm32f1xx.o ./Core/Src/system_stm32f1xx.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usb.d ./Core/Src/usb.o ./Core/Src/usb.su ./Core/Src/w25qxx.d ./Core/Src/w25qxx.o ./Core/Src/w25qxx.su

.PHONY: clean-Core-2f-Src

