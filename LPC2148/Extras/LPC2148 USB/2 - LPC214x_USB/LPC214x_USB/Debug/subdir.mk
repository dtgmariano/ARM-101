################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../delay.c \
../iprintf.c \
../lcd.c \
../main_custom.c \
../startup.c \
../uart.c \
../usbcontrol.c \
../usbhw_lpc.c \
../usbinit.c \
../usbstdreq.c 

S_UPPER_SRCS += \
../crt.S 

OBJS += \
./crt.o \
./delay.o \
./iprintf.o \
./lcd.o \
./main_custom.o \
./startup.o \
./uart.o \
./usbcontrol.o \
./usbhw_lpc.o \
./usbinit.o \
./usbstdreq.o 

C_DEPS += \
./delay.d \
./iprintf.d \
./lcd.d \
./main_custom.d \
./startup.d \
./uart.d \
./usbcontrol.d \
./usbhw_lpc.d \
./usbinit.d \
./usbstdreq.d 

S_UPPER_DEPS += \
./crt.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.S
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC Assembler'
	arm-none-eabi-gcc -x assembler-with-cpp -I"C:\eclipse1\workspace\LPC214x_USB" -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Sourcery Windows GCC C Compiler'
	arm-none-eabi-gcc -I"C:\eclipse1\workspace\LPC214x_USB" -O0 -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=arm7tdmi-s -g3 -gdwarf-2 -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


