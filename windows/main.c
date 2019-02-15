#include "bomberman.h"
#include "client.h"

int	main(int argc, char** argv)
{

  SDL_SetMainReady();
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    printf("SDL init error: %s", SDL_GetError());
    return (0);
  }


  #ifdef _WIN32
    static WSADATA data;
      if (0 != WSAStartup(MAKEWORD(2, 2), &data))
          ERR_MSG("WSAStartup failed!\n");
  #endif
  if (argc == 4 && !strcmp(argv[1], "client"))
    client(argv[2], atoi(argv[3]));
  else if (argc == 3 && !strcmp(argv[1], "server"))
    return server(atoi(argv[2]));
  else
	printf("Usage:\n  ./bomberman server [port]\n ./bomberman client [host] [port]\n");

  return (0);
}
