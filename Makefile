CFLAGS = -g
SRCS = gameLogic.c
OBJS = $(SRCS:.c=.o)

all: game

game: $(OBJS)
	$(CC) $(OBJS) -o 2048 
clean:
	rm -f $(OBJS) 2048 
