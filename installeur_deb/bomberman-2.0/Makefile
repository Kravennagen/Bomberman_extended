SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

LIBS = `sdl2-config --libs`

CFLAGS = -D _DEFAULT_SOURCE --std=c11 `sdl2-config --cflags` -g

NAME = bomberman

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS) -lSDL2_image -lpthread

.PHONY: clean fclean

install:
	install bomberman $(DESTDIR)$(prefix)/bin
	install bomberman.1 $(DESTDIR)$(prefix)/share/man/man1

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: clean all
