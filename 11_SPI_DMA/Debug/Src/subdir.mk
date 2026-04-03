################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ADC_DMA.c \
../Src/DMA.c \
../Src/SPI_DMA.c \
../Src/UART.c \
../Src/UART_DMA.c \
../Src/main.c \
../Src/syscalls.c \
../Src/sysmem.c 

OBJS += \
./Src/ADC_DMA.o \
./Src/DMA.o \
./Src/SPI_DMA.o \
./Src/UART.o \
./Src/UART_DMA.o \
./Src/main.o \
./Src/syscalls.o \
./Src/sysmem.o 

C_DEPS += \
./Src/ADC_DMA.d \
./Src/DMA.d \
./Src/SPI_DMA.d \
./Src/UART.d \
./Src/UART_DMA.d \
./Src/main.d \
./Src/syscalls.d \
./Src/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su Src/%.cyclo: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DNUCLEO_F401RE -DSTM32 -DSTM32F401RETx -DSTM32F4 -DSTM32F401xE -c -I../Inc -I"C:/Users/MSI/Desktop/Work/STM_/Course Udemy/0_test/chip_headers/CMSIS/Include" -I"C:/Users/MSI/Desktop/Work/STM_/Course Udemy/0_test/chip_headers/CMSIS/Device/ST/STM32F4xx/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/ADC_DMA.cyclo ./Src/ADC_DMA.d ./Src/ADC_DMA.o ./Src/ADC_DMA.su ./Src/DMA.cyclo ./Src/DMA.d ./Src/DMA.o ./Src/DMA.su ./Src/SPI_DMA.cyclo ./Src/SPI_DMA.d ./Src/SPI_DMA.o ./Src/SPI_DMA.su ./Src/UART.cyclo ./Src/UART.d ./Src/UART.o ./Src/UART.su ./Src/UART_DMA.cyclo ./Src/UART_DMA.d ./Src/UART_DMA.o ./Src/UART_DMA.su ./Src/main.cyclo ./Src/main.d ./Src/main.o ./Src/main.su ./Src/syscalls.cyclo ./Src/syscalls.d ./Src/syscalls.o ./Src/syscalls.su ./Src/sysmem.cyclo ./Src/sysmem.d ./Src/sysmem.o ./Src/sysmem.su

.PHONY: clean-Src

