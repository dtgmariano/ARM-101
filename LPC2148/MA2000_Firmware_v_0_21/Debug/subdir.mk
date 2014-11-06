################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc_module.c \
../cpu_init.c \
../low_level_init.c \
../main.c \
../pwm_module.c 

S_UPPER_SRCS += \
../startup.S 

OBJS += \
./adc_module.o \
./cpu_init.o \
./low_level_init.o \
./main.o \
./pwm_module.o \
./startup.o 

C_DEPS += \
./adc_module.d \
./cpu_init.d \
./low_level_init.d \
./main.d \
./pwm_module.d 

S_UPPER_DEPS += \
./startup.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\ARM\PROGRAMAS\LPC2148\MA2000 Firmware v0.21" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\ARM\PROGRAMAS\LPC2148\MA2000 Firmware v0.21" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


