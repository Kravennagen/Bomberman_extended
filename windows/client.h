#ifndef CLIENT_H_
#define CLIENT_H_

#include "bomberman.h"
#include "SDL2/SDL.h"
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include "SDL2/SDL_image.h"
#include <string.h>

typedef struct s_tile {
  int x;
  int y;
} t_tile;

enum e_map_flag {
	map_flag_bomb = 0x10,
	map_flag_breakable_wall = 0x20,
	map_flag_wall = 0x40,
	map_flag_burning = 0x80,
	map_flag_bonus = 0x08,
};

void	map_init(char*);
char	map_get(char*, int y, int x);
void	map_set(char*, int y, int x, char val);
int	map_cell_burning(char cell);
int	map_is_wall(char cell);
int map_is_breakable_wall(char cell);
int map_has_bomb(char cell);
int map_has_bonus(char cell);
int	map_get_bonus(char cell);
void handle_action(SDL_KeyboardEvent* key, int sockfd, t_player_info* infos);
void	client(char* host, int port);
int	game_is_finish(t_game *game, int userIndex);
void	display(SDL_Surface *screen, t_game *game/*, t_player_info *player*/);

#endif
