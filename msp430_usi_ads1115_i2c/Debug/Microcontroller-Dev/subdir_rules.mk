################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
Microcontroller-Dev/main.obj: ../Microcontroller-Dev/main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/CCS/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/CCS/ccsv7/ccs_base/msp430/include" --include_path="D:/CCS/Workspace/blinkLED/msp430_usi_i2c" --include_path="D:/CCS/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430G2452__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Microcontroller-Dev/main.d" --obj_directory="Microcontroller-Dev" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Microcontroller-Dev/usi_i2c.obj: ../Microcontroller-Dev/usi_i2c.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"D:/CCS/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/bin/cl430" -vmsp --use_hw_mpy=none --include_path="D:/CCS/ccsv7/ccs_base/msp430/include" --include_path="D:/CCS/Workspace/blinkLED/msp430_usi_i2c" --include_path="D:/CCS/ccsv7/tools/compiler/ti-cgt-msp430_16.9.1.LTS/include" --advice:power=all --define=__MSP430G2452__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="Microcontroller-Dev/usi_i2c.d" --obj_directory="Microcontroller-Dev" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


