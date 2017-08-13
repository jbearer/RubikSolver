
# CC = gcc
# CCFLAGS = -I -Wall

# SOURCES=$(wildcard *.c)
# OBJECTS=$(SOURCES:.c=.o)
# TARGET=des

# OBJDIR = obj

# all: $(TARGET)

# $(TARGET): $(OBJECTS)
# 	$(CC) -o $@ $^ $(LDFLAGS) 

# # $(OBJDIR)/%.o: %.c %.h
# # 	$(CC) $(CCFLAGS) -c $<

# $(OBJDIR)/%.o: %.c
# 	$(CC) $(CCFLAGS) -c $<

# clean:
# 	rm -f *.o $(TARGET)

TARGET = prog
LIBS = -lm
CC = gcc
CFLAGS = -g -Wall

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)