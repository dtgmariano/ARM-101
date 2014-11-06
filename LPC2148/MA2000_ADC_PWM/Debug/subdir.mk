################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../adc.c \
../cpu_init.c \
../low_level_init.c \
../ma2000main.c \
../pwm.c \
../uart.c 

S_UPPER_SRCS += \
../startup.S 

OBJS += \
./adc.o \
./cpu_init.o \
./low_level_init.o \
./ma2000main.o \
./pwm.o \
./startup.o \
./uart.o 

C_DEPS += \
./adc.d \
./cpu_init.d \
./low_level_init.d \
./ma2000main.d \
./pwm.d \
./uart.d 

S_UPPER_DEPS += \
./startup.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\Dropbox\ARM\PROGRAMAS\LPC2148\MA2000_ADC_PWM" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Yagarto Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\Dropbox\ARM\PROGRAMAS\LPC2148\MA2000_ADC_PWM" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


