################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../random/src/fortuna.c \
../random/src/fortuna_entropy_accumulator.c \
../random/src/fortuna_prng.c \
../random/src/fortuna_seed_manager.c 

OBJS += \
./random/src/fortuna.o \
./random/src/fortuna_entropy_accumulator.o \
./random/src/fortuna_prng.o \
./random/src/fortuna_seed_manager.o 

C_DEPS += \
./random/src/fortuna.d \
./random/src/fortuna_entropy_accumulator.d \
./random/src/fortuna_prng.d \
./random/src/fortuna_seed_manager.d 


# Each subdirectory must supply rules for building sources it contributes
random/src/fortuna.o: ../random/src/fortuna.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' '-DRETARGET_UART0=1' '-DRETARGET_VCOM=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/Device/SiliconLabs/EFM32GG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/EFM32GG_STK3700/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emdrv/gpiointerrupt/inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\demo" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"random/src/fortuna.d" -MT"random/src/fortuna.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

random/src/fortuna_entropy_accumulator.o: ../random/src/fortuna_entropy_accumulator.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' '-DRETARGET_UART0=1' '-DRETARGET_VCOM=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/Device/SiliconLabs/EFM32GG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/EFM32GG_STK3700/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emdrv/gpiointerrupt/inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\demo" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"random/src/fortuna_entropy_accumulator.d" -MT"random/src/fortuna_entropy_accumulator.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

random/src/fortuna_prng.o: ../random/src/fortuna_prng.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' '-DRETARGET_UART0=1' '-DRETARGET_VCOM=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/Device/SiliconLabs/EFM32GG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/EFM32GG_STK3700/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emdrv/gpiointerrupt/inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\demo" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"random/src/fortuna_prng.d" -MT"random/src/fortuna_prng.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

random/src/fortuna_seed_manager.o: ../random/src/fortuna_seed_manager.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM C Compiler'
	arm-none-eabi-gcc -g -gdwarf-2 -mcpu=cortex-m3 -mthumb -std=c99 '-DEFM32GG990F1024=1' '-DDEBUG=1' '-DRETARGET_UART0=1' '-DRETARGET_VCOM=1' -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/CMSIS/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/bsp" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emlib/inc" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/common/drivers" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/Device/SiliconLabs/EFM32GG/Include" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//hardware/kit/EFM32GG_STK3700/config" -I"C:/SiliconLabs/SimplicityStudio/v4/developer/sdks/gecko_sdk_suite/v2.3//platform/emdrv/gpiointerrupt/inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\inc" -I"C:\Users\small\SimplicityStudio\v4_workspace\Homework_version_2\random\demo" -O0 -Wall -c -fmessage-length=0 -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -MMD -MP -MF"random/src/fortuna_seed_manager.d" -MT"random/src/fortuna_seed_manager.o" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


