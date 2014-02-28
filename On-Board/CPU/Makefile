# makefile (use -pg for profiling)

IDIR = include
CC = g++
DEBUG =
PROFILE =
CFLAGS = -Wall -c -w $(DEBUG) -I$(IDIR) $(PROFILE)

SDIR = src
ODIR = bin
LIBS = -lm -lpthread
RES  = results

_DEPS = arm.h joints.h rover.h serial.h timing.h wheel.h i2c.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = rover.o arm.o joints.o serial.o timing.o wheel.o i2c.o main.o
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
