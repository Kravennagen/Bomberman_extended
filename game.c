#include "Headers/bomberman.h"
#include "Headers/client.h"

static void move_player(t_game* game, int userIndex, int y, int x)
{
  t_player_info* player = game->players + userIndex;
  if (x < 0 || x >= MAP_ROW || y < 0 || y >= MAP_COL)
    return;
  char cell = map_get(game->map, y, x);
  if (map_is_wall(cell) || map_has_bomb(cell))
    return;
  for (int i = 0; i < MAX_PLAYERS; ++i)
    if (game->players[i].alive && i != userIndex && game->players[i].x_pos == x && game->players[i].y_pos == y)
      return;
  player->y_pos = y;
  player->x_pos = x;
}

void  game_process(t_server* server, t_client_request* req, int userIndex)
{
  t_player_info* player = server->game.players + userIndex;
  printf("Received action from %d\n", userIndex);
  if (req->x_pos == -1 || req->x_pos == 1)
  {
    int newX = player->x_pos + req->x_pos;
    move_player(&server->game, userIndex, player->y_pos, newX);
  }
  else if (req->y_pos == -1 || req->y_pos == 1)
  {
    int newY = player->y_pos + req->y_pos;
    move_player(&server->game, userIndex, newY, player->x_pos);
  }
  else if (req->command && player->bombs_left && !server->game.bomb)
  {
    printf("bomb=%d\n", player->bombs_left);
    t_bomb* bomb = malloc(sizeof *bomb);
    if (!bomb)
      return;
    bomb->ticks_left = BOMB_TICKS;
    bomb->y = player->y_pos;
    bomb->x = player->x_pos;
    bomb->prev = NULL;
    bomb->next = NULL;
    map_set(server->game.map, bomb->y, bomb->x, map_flag_bomb);
    if(!server->game.bomb)
      server->game.bomb = bomb;
    else
    {
      t_bomb* temp_bomb = server->game.bomb;
      printf("dessous temp bomb \n");
      while(temp_bomb->next != NULL)
      {
        printf("dans le while \n");
        temp_bomb = temp_bomb->next;
      }
      temp_bomb->next = bomb;
      bomb->prev = temp_bomb;
      printf("avant le free");
    }
    player->bombs_left--;
    printf("Bomb has been planted\n");
  }
}

void  game_tick(t_game* game)
{
  /* remove lingering ambers */
  for (int i = 0; i < MAP_SIZE; ++i)
    if (map_cell_burning(game->map[i]))
      game->map[i] = 0;

  if (!game->bomb)
    return;
  if (--game->bomb->ticks_left)
    return; /* bomb will not explode yet */
  for (int y = -1; y <= 1; ++y)
    for (int x = -1; x <= 1; ++x)
    {
      if (x != 0 && y != 0)
        continue; /* only cross, not 3x3 explosion */
      int checkY = game->bomb->y + y;
      int checkX = game->bomb->x + x;
      char cell = map_get(game->map, checkY, checkX);
      /* kill players */
      for (int i = 0; i < MAX_PLAYERS; ++i)
        if (game->players[i].x_pos == checkX && game->players[i].y_pos == checkY)
          game->players[i].alive = 0;
      if (cell == 0 || map_is_breakable_wall(cell))
        map_set(game->map, checkY, checkX, map_flag_burning);
    }
  map_set(game->map, game->bomb->y, game->bomb->x, map_flag_burning); /* remove bomb */
  free(game->bomb);
  game->bomb = NULL;
}

void  game_init_players(t_game* game)
{
  game->bomb = NULL;
  for (int i = 0; i < MAX_PLAYERS; ++i)
  {
    game->players[i].alive = 1;
    game->players[i].connected = 1;
    game->players[i].bombs_left = 5;
  }
    
  game->players[0].x_pos = game->players[0].y_pos = 1;
  if (MAX_PLAYERS > 1)
  {
    game->players[1].x_pos = MAP_ROW - 1 /* not the end */ - 1 /* 1-indexed */;
    game->players[1].y_pos = 1;
  }
  if (MAX_PLAYERS > 2)
  {
    game->players[2].x_pos = 1;
    game->players[2].y_pos = MAP_COL - 2;
  }
  if (MAX_PLAYERS > 3)
  {
    game->players[3].x_pos = MAP_ROW - 2;
    game->players[3].y_pos = MAP_COL - 2;
  }
}