CC = gcc
CFLAGS = -Wall

LIBS = -lgd -lpng

TARGET = pngsplash

SRCS = pngsplash.c

OBJS = $(SRCS:.c=.o)

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

clean:
	$(RM) *.o *~ $(TARGET)

default: all
