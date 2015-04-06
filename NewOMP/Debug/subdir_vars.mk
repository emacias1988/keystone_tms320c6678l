################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../omp_config.cfg 

C_SRCS += \
../cmp_function.c \
../omp_hello.c \
../tables.c 

OBJS += \
./cmp_function.obj \
./omp_hello.obj \
./tables.obj 

C_DEPS += \
./cmp_function.pp \
./omp_hello.pp \
./tables.pp 

GEN_MISC_DIRS += \
./configPkg/ 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_OPTS += \
./configPkg/compiler.opt 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" 

C_DEPS__QUOTED += \
"cmp_function.pp" \
"omp_hello.pp" \
"tables.pp" 

OBJS__QUOTED += \
"cmp_function.obj" \
"omp_hello.obj" \
"tables.obj" 

C_SRCS__QUOTED += \
"../cmp_function.c" \
"../omp_hello.c" \
"../tables.c" 

GEN_CMDS__FLAG += \
-l"./configPkg/linker.cmd" 

GEN_OPTS__FLAG += \
--cmd_file="./configPkg/compiler.opt" 


