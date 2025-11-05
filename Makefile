CFLAGS = -g 
LDFLAGS = -lncurses
SRCS = main.c gameLogic.c ui.c
OBJS = $(SRCS:.c=.o)

all: game

game: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o 2048 
clean:
	rm -f $(OBJS) 2048 
