#include "bomberman.h"
#include "client.h"

static SDL_Rect* sdlh_rect(SDL_Rect* rect, int x, int y, int w, int h) {
  if (!rect)
    return NULL;
  rect->x = x;
  rect->y = y;
  rect->w = w;
  rect->h = h;
  return rect;
}

static t_tile get_tile(t_game *game, int x, int y)
{
  t_tile ret = {16, 208}; /* grass tile */
  for (int i = 0; i < MAX_PLAYERS; ++i)
  {
    t_player_info* player = game->players + i;
    if (player->alive && player->x_pos == x && player->y_pos == y)
      return (ret.x = 0, ret.y = 224, ret);
  }
  char cell = map_get(game->map, y, x);
  if (map_cell_burning(cell))
    return (ret.x = 32, ret.y = 288, ret);
  if (map_is_breakable_wall(cell))
    return (ret.x = 160, ret.y = 272, ret);
  if (map_is_wall(cell))
    return (ret.x = 160, ret.y = 288, ret);
  if (map_has_bomb(cell))
    return (ret.x = 64, ret.y = 288, ret);
  return ret;
}

static SDL_Surface *getSpriteMap(void)
{
  static SDL_Surface *spriteMap;
  if (!spriteMap)
  {
    spriteMap = IMG_Load("bomb_party_v4.png");
    if (!spriteMap)
    {
      fprintf(stderr, "error loading sprite map : '%s'\n", IMG_GetError());
      exit(1);
    }
  }
  return spriteMap;
}

void  display(SDL_Surface *screen, t_game *game/*, t_player_info *player*/)
{
  SDL_Rect* rect;
  SDL_Rect* rect2;
  rect = malloc(sizeof(SDL_Rect));
  rect2 = malloc(sizeof(SDL_Rect));
  for (int y = 0; y < MAP_COL; ++y)
  {
    for (int x = 0; x < MAP_ROW; ++x)
    {
      t_tile tile = get_tile(game, x, y);

      SDL_BlitSurface(
        getSpriteMap(),
        sdlh_rect(rect, tile.x, tile.y, TILE_WIDTH, TILE_HEIGHT),
        screen,
        sdlh_rect(rect2, x * TILE_WIDTH, y * TILE_HEIGHT, 0 /* unused */, 0 /* unused */)
      );
    }
  }
  free(rect);
  free(rect2);
}