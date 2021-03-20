include application.mk

BUILDDIR := ./build

SHELL     := /bin/bash
TOOLCHAIN ?= arm-none-eabi
PART       = apollo3
CPU        = cortex-m4
FPU        = fpv4-sp-d16
FABI       = hard

#### Required Executables ####
CC   = $(TOOLCHAIN)-gcc
CXX  = $(TOOLCHAIN)-g++
CPP  = $(TOOLCHAIN)-cpp
LD   = $(TOOLCHAIN)-ld
OCP  = $(TOOLCHAIN)-objcopy
OD   = $(TOOLCHAIN)-objdump
RD   = $(TOOLCHAIN)-readelf
AR   = $(TOOLCHAIN)-ar
SIZE = $(TOOLCHAIN)-size

ifeq ($(OS), Windows_NT)
  MKDIR = mkdir
  CP    = cp
  RM    = rm
  PYTHON = python
else
  MKDIR = mkdir -p
  CP   = $(shell which cp 2>/dev/null)
  RM   = $(shell which rm 2>/dev/null)
  PYTHON = python3
endif

DEFINES += -Dgcc
DEFINES += -DAM_PART_APOLLO3
DEFINES += -DAM_PACKAGE_BGA
DEFINES += -DPART_apollo3
ifdef DEBUG
  DEFINES += -DAM_ASSERT_INVALID_THRESHOLD=0
  DEFINES += -DAM_DEBUG_ASSERT
  DEFINES += -DAM_DEBUG_PRINTF
endif

CFLAGS += -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI)
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -MMD -MP -std=c99 -Wall
CFLAGS += $(DEFINES)
#ifdef DEBUG
  CFLAGS += -g -O0
#else
#  CFLAGS += -O3
#endif

CXXFLAGS += -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI) -MMD
#ifdef DEBUG
  CXXFLAGS += -g -O0
#else
#  CXXFLAGS += -O3
#endif


OCPFLAGS = -Obinary
ODFLAGS  = -S

INCLUDES += -I$(AMBIQ_SDK)/inc/apollo3
INCLUDES += -I$(AMBIQ_SDK)/inc/apollo3/hal
INCLUDES += -I$(AMBIQ_SDK)/inc/apollo3/regs
INCLUDES += -I$(AMBIQ_SDK)/inc/utils

INCLUDES += -I$(CMSIS_SDK)/CMSIS/Include
INCLUDES += -I$(CMSIS_SDK)/Device/ARM/apollo3/Include

LIBS += -lam_hal
LIBS += -lam_utils

LDSCRIPT := ../variants/nm180100evb/AMA3B1KK.ld

INCLUDES += -I.
INCLUDES += -I../cores/arduino
INCLUDES += -I../cores/arduino/api
INCLUDES += -I../variants/nm180100evb

VPATH += .
VPATH += ../cores/arduino
VPATH += ../cores/arduino/api
VPATH += ../variants/nm180100evb

CSRC = $(filter %.c, $(SRC))
CXXSRC = $(filter %.cpp, $(SRC))
ASRC = $(filter %.s, $(SRC))

OBJS  = $(CSRC:%.c=$(BUILDDIR)/%.o)
OBJS += $(CXXSRC:%.cpp=$(BUILDDIR)/%.o)
OBJS += $(ASRC:%.s=$(BUILDDIR)/%.o)

DEPS  = $(CSRC:%.c=$(BUILDDIR)/%.d)
DEPS += $(CXXSRC:%.cpp=$(BUILDDIR)/%.d)
DEPS += $(ASRC:%.s=$(BUILDDIR)/%.d)

CFLAGS += $(INCLUDES)
CFLAGS += $(DEFINES)

CXXFLAGS += $(INCLUDES)
CXXFLAGS += $(DEFINES)

LFLAGS += -mthumb -mcpu=$(CPU) -mfpu=$(FPU) -mfloat-abi=$(FABI)
#LFLAGS += -nostartfiles -static
LFLAGS += -Wl,--entry,Reset_Handler
LFLAGS += -Wl,-Map,$(BUILDDIR)/$(TARGET).map
LFLAGS += -Wl,--start-group
LFLAGS += -L$(AMBIQ_SDK)/lib
LFLAGS += -L$(CMSIS_SDK)/CMSIS/Lib
LFLAGS += --specs=nano.specs
LFLAGS += --specs=nosys.specs
LFLAGS += -larm_cortexM4lf_math
LFLAGS += -lsupc++
LFLAGS += -lstdc++
LFLAGS += -lm
LFLAGS += $(LIBS)
LFLAGS += -Wl,--end-group
LFLAGS += -Wl,--gc-sections
#LFLAGS += -Wl,--wrap,_calloc_r
LFLAGS += -Wl,--wrap,_memalign_r
LFLAGS += -Wl,--wrap,atexit
LFLAGS += -Wl,--wrap,exit
LFLAGS += -Wl,--wrap,fprintf
LFLAGS += -Wl,--wrap,printf
LFLAGS += -Wl,--wrap,snprintf
#LFLAGS += -Wl,--wrap,sprintf
LFLAGS += -Wl,--wrap,vfprintf
LFLAGS += -Wl,--wrap,vprintf
LFLAGS += -Wl,--wrap,vsnprintf
LFLAGS += -Wl,--wrap,vsprintf
LFLAGS += -Wl,-n

all: directories $(BUILDDIR)/$(TARGET).bin

ota: directories $(BUILDDIR)/$(TARGET_OTA).bin

wire: directories $(BUILDDIR)/$(TARGET_WIRE).bin

directories: $(BUILDDIR)

$(BUILDDIR):
	@$(MKDIR) $@

$(BUILDDIR)/%.o: %.c $(BUILDDIR)/%.d
	@echo "Compiling $(COMPILERNAME) $<"
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.cpp $(BUILDDIR)/%.d
	@echo "Compiling $(COMPILERNAME) $<"
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(BUILDDIR)/%.o: %.s $(BUILDDIR)/%.d
	@echo "Assembling $(COMPILERNAME) $<"
	$(CC) -c $(CFLAGS) $< -o $@

$(BUILDDIR)/$(TARGET).axf: $(OBJS)
	@echo "Linking $@"
	$(CC) -Wl,-T,$(LDSCRIPT) -o $@ $(OBJS) $(LFLAGS)

$(BUILDDIR)/$(TARGET).bin: $(BUILDDIR)/$(TARGET).axf
	$(OCP) $(OCPFLAGS) $< $@
	$(OD) $(ODFLAGS) $< > $(BUILDDIR)/$(TARGET).lst

$(BUILDDIR)/$(TARGET_OTA).bin: $(BUILDDIR)/$(TARGET).bin
	@echo "Generating OTA image $@"
	$(PYTHON) ./tools/create_cust_image_blob.py --bin $< --load-address 0xc000 --magic-num 0xcb -o $(BUILDDIR)/$(TARGET_OTA)_temp --version $(TARGET_VERSION)
	$(PYTHON) ./tools/ota_binary_converter.py --appbin $(BUILDDIR)/$(TARGET_OTA)_temp.bin -o $(BUILDDIR)/$(TARGET_OTA)
	@$(RM) -rf $(BUILDDIR)/$(TARGET_OTA)_temp.bin

$(BUILDDIR)/$(TARGET_WIRE).bin: $(BUILDDIR)/$(TARGET).bin
	@echo "Generating UART wire image $@"
	$(PYTHON) ./tools/create_cust_image_blob.py --bin $< --load-address 0xc000 --magic-num 0xcb -o $(BUILDDIR)/$(TARGET_WIRE)_temp --version 0x0
	$(PYTHON) ./tools/create_cust_wireupdate_blob.py --load-address 0x20000 --bin $(BUILDDIR)/$(TARGET_WIRE)_temp.bin -i 6 -o $(BUILDDIR)/$(TARGET_WIRE) --options 0x1
	@$(RM) -rf $(BUILDDIR)/$(TARGET_WIRE)_temp.bin


clean:
	@echo "Cleaning..."
	$(RM) -f $(OBJS) $(DEPS) $(BUILDDIR)/$(TARGET).a
	$(RM) -rf $(BUILDDIR)

$(BUILDDIR)/%.d: ;

-include $(DEPS)

