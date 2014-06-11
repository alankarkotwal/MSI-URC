# makefile (use -pg for profiling)

IDIR = include
CC = g++
DEBUG =
PROFILE =
CFLAGS = -Wall -c -w -fpermissive $(DEBUG) -I$(IDIR) $(PROFILE)

SDIR = src
ODIR = bin
LIBS = -lm -lpthread -lwiringPi
RES  = results

_DEPS = arm_rpi.h serial.h timing.h i2c.h config.h decoding.h encoding.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = arm_rpi.o serial.o timing.o i2c.o main.o decoding.o encoding.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) $(CFLAGS) -o $@ $<

all: dir $(ODIR)/main

dir:
	mkdir -p $(ODIR)

$(ODIR)/main: $(OBJ)
	$(CC) -I$(IDIR) -o $@ $^ $(PROFILE) $(LIBS)

clean:
	rm -rf $(ODIR) *~ $(INCDIR)/*~

distclean: clean

rebuild: distclean all

.PHONY: clean
