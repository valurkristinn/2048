CFLAGS = -g -I/opt/homebrew/opt/ncurses/include -D_XOPEN_SOURCE_EXTENDED
LDFLAGS = -L/opt/homebrew/opt/ncurses/lib -lncursesw
SRCS = main.c gameLogic.c ui.c
OBJS = $(SRCS:.c=.o)

all: game

game: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o 2048 
clean:
	rm -f $(OBJS) 2048
