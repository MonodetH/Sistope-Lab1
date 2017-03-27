CC = gcc
CFLAGS = -Wall -g
SRCS = main.c processes.c handlers.c
OBJS = ${SRCS:.c=.o}

build: $(OBJS)
	$(CC) $(CFLAGS) -o Lab $(SRCS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f Lab *.o 

rebuild: clean build