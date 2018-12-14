#ifndef CLIENT_H_
#define CLIENT_H_

#include "bomberman.h"
#include "win_lose.h"

void	client(char* host, int port);
int	game_is_finish(t_game *game, int userIndex);
#endif
