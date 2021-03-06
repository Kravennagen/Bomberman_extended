#include "Headers/bomberman.h"
#include "Headers/client.h"

int	main(int argc, char** argv)
{
  if (argc == 4 && !strcmp(argv[1], "client"))
    client(argv[2], atoi(argv[3]));
  else if (argc == 3 && !strcmp(argv[1], "server"))
    return server(atoi(argv[2]));
  else
	printf("Usage:\n  ./bomberman server [port]\n ./bomberman client [host] [port]\n");
}
