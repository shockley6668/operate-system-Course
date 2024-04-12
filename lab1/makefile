CC=gcc
CFLAGS = -g -Wall
OBJS = ComputeTask.o IOTask.o mainProg.o
TARGET = mainProg

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(CFLAGS)

Code_1A: Code_1A.o
	$(CC) -o Code_1A Code_1A.o $(CFLAGS)

Code_1B: Code_1B.o ComputeTask.o IOTask.o
	$(CC) -o Code_1B Code_1B.o ComputeTask.o IOTask.o $(CFLAGS)

Code_1C: Code_1C.o ComputeTask.o IOTask.o
	$(CC) -o Code_1C Code_1C.o ComputeTask.o IOTask.o $(CFLAGS)

clean:
	rm  -rf  *.o  $(TARGET) Code_1A+