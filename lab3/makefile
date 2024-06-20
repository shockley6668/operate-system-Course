CC=gcc
CFLAGS = -g -Wall
OBJS = mainProg.o
TARGET = mainProg

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)
3Prog: 3Prog.o
	$(CC) -o 3Prog 3Prog.o $(CFLAGS)

clean:
	rm  -rf  *.o  $(TARGET)