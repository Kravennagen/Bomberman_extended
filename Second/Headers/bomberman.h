#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

#define MAX_PLAYERS 4
#define MAP_COL 50 /* y | */
#define MAP_ROW 50 /* x --- */
#define MAP_SIZE (MAP_COL * MAP_ROW)
#define TILE_HEIGHT 16 /* y */
#define TILE_WIDTH 16 /* x */
#define BOMB_TICKS 1500
#define ERR_MSG(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

#pragma pack(2)

typedef struct s_bomb t_bomb;

typedef struct s_bomb
{
        double  ticks_left;
        int y;
        int x;
        t_bomb* prev;
        t_bomb* next;
}s_bomb;

typedef struct s_player_info
{
        char  connected;
        char  alive;
        int   x_pos;
        int   y_pos;
        int   bombs_left;
}               t_player_info;

typedef struct  s_client_request
{
        int           x_pos;                  /* La position x souhait?e par le client */
        int           y_pos;                  /* La position y souhait?e par le client */
        int           command;                /* Une commande du client (0 : Ne rien faire / 1 : Poser une bombe) */
}t_client_request;

typedef char t_map[MAP_SIZE];

typedef struct  s_game
{
        t_player_info           players[MAX_PLAYERS];
        t_map                   map;
        t_bomb*                 bomb;
}t_game;

typedef struct s_server {
        pthread_t               tid;
        t_game                  game;
        int                     fds[MAX_PLAYERS];
        struct sockaddr_in      sock_serv;
        struct sockaddr         *sock_ptr;
        socklen_t               len;
        int                     sockfd;
        int                     port;
        int                     running;
} t_server;


void    game_process(t_server*, t_client_request*, int userIndex);
void    game_tick(t_game*);
void    game_init_players(t_game*);
void    bomb_things(t_game* game);
int     server(int host);

#endif
