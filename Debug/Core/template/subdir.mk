################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/template/config_adc.c \
../Core/template/config_hrtim.c \
../Core/template/cv_cc.c \
../Core/template/font.c \
../Core/template/kalman.c \
../Core/template/mbase.c \
../Core/template/mppt.c \
../Core/template/oled.c \
../Core/template/pid.c \
../Core/template/print_adapt.c \
../Core/template/protect.c \
../Core/template/sin_analyzer.c \
../Core/template/soft_start.c \
../Core/template/spll_1ph_sogi.c \
../Core/template/spwm.c \
../Core/template/vofa.c 

OBJS += \
./Core/template/config_adc.o \
./Core/template/config_hrtim.o \
./Core/template/cv_cc.o \
./Core/template/font.o \
./Core/template/kalman.o \
./Core/template/mbase.o \
./Core/template/mppt.o \
./Core/template/oled.o \
./Core/template/pid.o \
./Core/template/print_adapt.o \
./Core/template/protect.o \
./Core/template/sin_analyzer.o \
./Core/template/soft_start.o \
./Core/template/spll_1ph_sogi.o \
./Core/template/spwm.o \
./Core/template/vofa.o 

C_DEPS += \
./Core/template/config_adc.d \
./Core/template/config_hrtim.d \
./Core/template/cv_cc.d \
./Core/template/font.d \
./Core/template/kalman.d \
./Core/template/mbase.d \
./Core/template/mppt.d \
./Core/template/oled.d \
./Core/template/pid.d \
./Core/template/print_adapt.d \
./Core/template/protect.d \
./Core/template/sin_analyzer.d \
./Core/template/soft_start.d \
./Core/template/spll_1ph_sogi.d \
./Core/template/spwm.d \
./Core/template/vofa.d 


# Each subdirectory must supply rules for building sources it contributes
Core/template/%.o Core/template/%.su Core/template/%.cyclo: ../Core/template/%.c Core/template/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Core/template -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-template

clean-Core-2f-template:
	-$(RM) ./Core/template/config_adc.cyclo ./Core/template/config_adc.d ./Core/template/config_adc.o ./Core/template/config_adc.su ./Core/template/config_hrtim.cyclo ./Core/template/config_hrtim.d ./Core/template/config_hrtim.o ./Core/template/config_hrtim.su ./Core/template/cv_cc.cyclo ./Core/template/cv_cc.d ./Core/template/cv_cc.o ./Core/template/cv_cc.su ./Core/template/font.cyclo ./Core/template/font.d ./Core/template/font.o ./Core/template/font.su ./Core/template/kalman.cyclo ./Core/template/kalman.d ./Core/template/kalman.o ./Core/template/kalman.su ./Core/template/mbase.cyclo ./Core/template/mbase.d ./Core/template/mbase.o ./Core/template/mbase.su ./Core/template/mppt.cyclo ./Core/template/mppt.d ./Core/template/mppt.o ./Core/template/mppt.su ./Core/template/oled.cyclo ./Core/template/oled.d ./Core/template/oled.o ./Core/template/oled.su ./Core/template/pid.cyclo ./Core/template/pid.d ./Core/template/pid.o ./Core/template/pid.su ./Core/template/print_adapt.cyclo ./Core/template/print_adapt.d ./Core/template/print_adapt.o ./Core/template/print_adapt.su ./Core/template/protect.cyclo ./Core/template/protect.d ./Core/template/protect.o ./Core/template/protect.su ./Core/template/sin_analyzer.cyclo ./Core/template/sin_analyzer.d ./Core/template/sin_analyzer.o ./Core/template/sin_analyzer.su ./Core/template/soft_start.cyclo ./Core/template/soft_start.d ./Core/template/soft_start.o ./Core/template/soft_start.su ./Core/template/spll_1ph_sogi.cyclo ./Core/template/spll_1ph_sogi.d ./Core/template/spll_1ph_sogi.o ./Core/template/spll_1ph_sogi.su ./Core/template/spwm.cyclo ./Core/template/spwm.d ./Core/template/spwm.o ./Core/template/spwm.su ./Core/template/vofa.cyclo ./Core/template/vofa.d ./Core/template/vofa.o ./Core/template/vofa.su

.PHONY: clean-Core-2f-template

