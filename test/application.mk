#******************************************************************************
#
# Step 1
# Define the locations of the various SDKs and libraries.
#
#******************************************************************************
AMBIQ_SDK ?= ../system/ambiq
CMSIS_SDK ?= ../system/CMSIS/5.7.0

#******************************************************************************
#
# Step 2
# Specify the location of the board support package to be used.
#
#******************************************************************************

#******************************************************************************
#
# Step 3
# Specify output target version and name
#
#******************************************************************************
TARGET_VERSION := 0x00

ifdef DEBUG
    TARGET      := nmapp-dev
    TARGET_OTA  := nmapp_ota-dev
    TARGET_WIRE := nmapp_wire-dev
else
    TARGET      := nmapp
    TARGET_OTA  := nmapp_ota
    TARGET_WIRE := nmapp_wire
endif

#******************************************************************************
#
# Step 4
# Include additional source, header, libraries or paths below.
#
# Examples:
#   INCLUDES += -Iadditional_include_path
#   VPATH    += additional_source_path
#   LIBS     += -ladditional_library
#******************************************************************************

#INCLUDES += -I$(NM_SDK)/platform/console

#VPATH += $(NM_SDK)/platform/console

#SRC += console_task.c
#SRC += gpio_service.c
#SRC += iom_service.c

SRC += Common.cpp
SRC += IPAddress.cpp
SRC += PluggableUSB.cpp
SRC += Print.cpp
SRC += Stream.cpp
SRC += WString.cpp

SRC += delay.c
SRC += dtostrf.c
SRC += interrupts.c
SRC += itoa.c
SRC += pinmap.c
SRC += timermap.c
SRC += startup_gcc.c
SRC += wiring_analog.c
SRC += wiring_digital.c
SRC += wiring_shift.c
SRC += wiring_pulse.c
SRC += main.cpp
SRC += HardwareSerial.cpp
SRC += Tone.cpp

SRC += PeripheralPins.c
SRC += Peripherals.cpp
SRC += variant.cpp

SRC += Servo.cpp

INCLUDES += -I../libraries/Servo/src
INCLUDES += -I../libraries/Servo/src/apollo3
VPATH += ../libraries/Servo/src
VPATH += ../libraries/Servo/src/apollo3

INCLUDES += -I../libraries/Wire/src
VPATH += ../libraries/Wire/src
SRC += Wire.cpp

SRC += test.cpp

