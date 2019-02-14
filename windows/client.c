#include "bomberman.h"
#include "client.h"

#ifdef _WIN32
SOCKET
#else
int
#endif
 connect_to_server(const char* addr, int port)
{
  #ifdef _WIN32
    SOCKET sockfd;
  #else
    int sockfd;
  #endif
  struct sockaddr_in serv;

  printf("Connecting to %s:%i\n", addr, port);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  #ifdef _WIN32
    if (INVALID_SOCKET == sockfd)
      ERR_MSG("Could not open socket, er==\n", WSAGetLastError());
  #else
    if (sockfd < 0)
      ERR_MSG("Could not open socket, sockfd=%d\n", sockfd);
  #endif 
  serv.sin_family = AF_INET;
  serv.sin_port = htons(port);
  serv.sin_addr.s_addr = inet_addr(addr);

  #ifdef _WIN32
    if (SOCKET_ERROR == connect(sockfd, (SOCKADDR*)&serv, sizeof serv))
      ERR_MSG("Connect returned while trying to connect to %s:%d -1. errno=%d\n", addr, port, WSAGetLastError());
    return sockfd;
  #else
    if (-1 == connect(sockfd, (struct sockaddr*)&serv, sizeof serv))
      ERR_MSG("Connect returned -1. errno=%d\n", errno);
    return (sockfd);
  #endif

}


static int handle_event(SDL_Event* event, int sockfd, t_player_info* infos)
{
  if (event->type == SDL_QUIT)
    return (0);
  else if (event->type == SDL_KEYDOWN)
    handle_action(&event->key, sockfd, infos);
  return (1);
}

SDL_Window *window(void)
{
  SDL_Window* pWindow = NULL;
  pWindow = SDL_CreateWindow("Bomberman", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      MAP_ROW * TILE_WIDTH,
      MAP_COL * TILE_HEIGHT,
      SDL_WINDOW_SHOWN);

  return (pWindow);
}

static t_game*  read_game(int sockfd)
{
  char* buffer = malloc(sizeof(t_game));
  char* buffLeft = buffer;
  size_t left = sizeof(t_game);
  while (left > 0)
  {
    #ifdef _WIN32
      int count = recv(sockfd, buffLeft, left, 0);
    #else
      int count = read(sockfd, buffLeft, left);
    #endif    
    
    if (count == -1 && errno == EAGAIN)
      continue; /* try again */
    if (count == 0)
      return NULL; /* TODO signal disconnect */
    left -= count;
    buffLeft += count;
  }
  return (t_game*)buffer;
}


int game_is_finish(t_game *game, int userIndex)
{
  int total_alive = 0;
  if(!game->players[userIndex].connected)
  {
    return 1;
  }
  if (game->players[userIndex].alive) {
    for (int i = 0; i < MAX_PLAYERS; i++)
      {
        total_alive += game->players[i].alive;
      }
    return total_alive <= 1;
  } else {    
    return 1;
  }
}

void  client(char* host, int port)
{
  t_game  *game;
  #ifdef _WIN32
    SOCKET sockfd = connect_to_server(host, port);
  #else
    int sockfd = connect_to_server(host, port);
  #endif

  SDL_Window* pWindow = window();
  if (!pWindow)
    return;

  int userIndex = 0, readSize;
  printf("Logging in...\n");

  #ifdef _WIN32
    if ((readSize = recv(sockfd, (char *)&userIndex, sizeof userIndex, 0)) < (long)sizeof userIndex)
      ERR_MSG("Unable to read user index. errno=%d,size=%d\n", errno, readSize);
  #else
    if ((readSize = read(sockfd, &userIndex, sizeof userIndex)) < (long)sizeof userIndex)
      ERR_MSG("Unable to read user index. errno=%d,size=%d\n", errno, readSize);
  #endif

  printf("Logged in as player #%d\n", userIndex);
  SDL_Event event;
  while (1)
  {
    game = read_game(sockfd);
    if (!game)
    {
      sleep(0.5);
      continue;
    }
    if (game_is_finish(game, userIndex))
      break;
    while (SDL_PollEvent(&event))
      if (!handle_event(&event, sockfd, game->players + userIndex))
        return; /* user exited */
    display(SDL_GetWindowSurface(pWindow), game/*, game->players + userIndex*/);
    free(game);
    if (SDL_UpdateWindowSurface(pWindow) < 0)
      ERR_MSG("Unable to update window surface\n");
  }
  if (!game->players[userIndex].alive)
    printf("You are dead you lose\n");
  else
    printf("You win\n GG WP\n");
}