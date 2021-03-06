SRC = $(wildcard *.c)

OBJ = $(SRC:.c=.o)

LIBS = `sdl2-config --libs`

CFLAGS = -D _DEFAULT_SOURCE -Wall -Wextra -Wpedantic --std=c11 `sdl2-config --cflags` -g

NAME = bomberman

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LIBS) -lSDL2_image -lpthread

.PHONY: clean fclean

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: clean all