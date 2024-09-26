CFLAGS = -lncurses
CC = gcc

snake: snake.o draw.o
	$(CC) -o snake snake.o draw.o $(CFLAGS)

draw.o: draw.c draw.h constants.h
	$(CC) -c draw.c $(CFLAGS)

snake.o: snake.c constants.h draw.h
	$(CC) -c snake.c $(CFLAGS)

clean:
	rm -f *.o snake

