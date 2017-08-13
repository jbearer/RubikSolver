# Variables used in makefiles across the project

# External dependencies:
#	Arduino cores (optional - used for viewing compiler output without building
#		in Arduino IDE): path depends on local PC configuration.
#	AVR: (optional) a windows installation is available at
#		http://andybrown.me.uk/2012/04/28/avr-gcc-4-7-0-and-avr-libc-1-8-0-compiled-for-windows/
#
#	Boost/serialization; Boost/archive (required)

# Path to arduino libraries. Specific to local PC
AVR_PATH = C:/Program_Files_x86/Arduino/hardware/arduino/avr

# Path to Boost installation
BOOST_PATH = usr/include/boost

################################################################################
# Hardware settings
################################################################################

COM_PORT = \\.\\COM10

################################################################################
# Arduino compiler settings
################################################################################

# Pin layout (standard = Arduino Uno)
PIN_LAYOUT = standard

# Arduino Uno architecture
MCU = atmega328p

# Arduino processor frequency
F_CPU = 16000000UL

# Compiler for building Arduino executables
ARDCXX = avr-g++

# Arduino optimization level
ARDOPT = s

# Arduino comipler C++ standard
ARDSTD = c++11

ARDWARNINGS = -Wall -Wextra -pedantic

# Include path for Arduino compiler
ARDINCL = -I$(AVR_PATH)/cores/arduino -I$(AVR_PATH)/variants/$(PIN_LAYOUT)

################################################################################
# C++ compiler settings
################################################################################

DEBUG = true

OPTIMIZATION = 2

STD = c++11

WARNINGS = -Wall -Wextra -pedantic

CXX = g++

INCL = -isystem $(BOOST_PATH)

################################################################################
# C++ linker settings
################################################################################

LIB_FLAG = -L$(BOOST_PATH)

LIBS = -lboost_serialization

################################################################################
# DO NOT CHANGE THE FOLLOWING - Generates variables based on above settings
################################################################################

ifeq ($(DEBUG), true)
DEBUG_FLAG = -g
else
DEBUG_FLAG = 
endif

ARDCXXFLAGS = $(ARDWARNINGS) -O$(ARDOPT) -DF_CPU=$(F_CPU) -std=$(ARDSTD) -mmcu=$(MCU)
CXXFLAGS = $(DEBUG_FLAG) -O$(OPTIMIZATION) -std=$(STD) $(WARNINGS) \
-DCOM_PORT=$(COM_PORT) -DPC_BUILD=1

LINKERFLAGS = $(LIB_FLAG) $(LIBS)