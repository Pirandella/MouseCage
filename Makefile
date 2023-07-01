CC := gcc
TARGET := mousecage
CFLAGS = -std=c11 -I $(IDIR) -Wall -O1 -g3 -lX11 -lXtst -lXext `pkg-config --cflags xmu` `pkg-config --libs xmu` -D _DEFAULT_SOURCE

ODIR := ./obj
SDIR := ./src
IDIR := ./inc

_DEPS := arg_parser.h x.h
DEPS := $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ := main.o arg_parser.o x.o
OBJ := $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(TARGET)
