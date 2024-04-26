CC = gcc
CFLAGS = -Wall -Wextra -pthread
mainProg : Producer.o Consumer.o mainProg.o ProducerConsumerUtilities.o
	$(CC) $(CFLAGS) -o mainProg Producer.o Consumer.o mainProg.o ProducerConsumerUtilities.o

clean:
	rm -f producer consumer
