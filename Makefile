CC = gcc
CFLAGS = -Wall -I./libs -I./libs/max7219 -I./libs/mpu6050 -I./libs/ds3231
LIBS = -lwiringPi -lm

SRC = main.c libs/max7219/max7219.c libs/mpu6050/mpu6050.c libs/ds3231/ds3231.c
OBJ = $(SRC:.c=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
