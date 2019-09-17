CC = gcc
CFLAGS =
TARGET = logParse
OBJS = main.o
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
clean:
	/bin/rm -f *.o $(TARGET)

