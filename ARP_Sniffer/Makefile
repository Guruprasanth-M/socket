CC      = gcc
CFLAGS  = -Wall -Wextra -Iinclude
LDFLAGS = -lpcap

SRC = src/main.c src/sniff.c src/utils.c src/ui.c
OBJ = $(SRC:.c=.o)

TARGET = arpsniffer

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
