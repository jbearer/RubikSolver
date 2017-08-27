# Variables used in makefiles across the project

# External dependencies:
#	Boost/serialization; Boost/archive (required)

# Path to Boost installation
BOOST_PATH = /usr/local/include/boost

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

CXXFLAGS = $(DEBUG_FLAG) -O$(OPTIMIZATION) -std=$(STD) $(WARNINGS)

LINKERFLAGS = $(LIB_FLAG) $(LIBS)
