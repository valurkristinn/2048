CFLAGS = -g
SRCS = gameLogic.c
OBJS = $(SRCS:.c=.o)

all: game

game: $(OBJS)
	$(CC) $(OBJS) -o game

clean:
	rm -f $(OBJS) game
