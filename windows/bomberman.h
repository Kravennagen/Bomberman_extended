#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#ifdef _WIN32
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #include <winsock2.h>
    #include <windows.h>
    #include <io.h>
    #define _WINSOCKAPI_
    #include <stdint.h>
    #include <WS2tcpip.h>
    typedef int socklen_t;
    typedef SOCKET socket_holder;
    typedef HANDLE thread_t;
    typedef void(*thread_fn)(void* data);
#else
    #include <sys/types.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <pthread.h>
    typedef pthread_t thread_t;
#endif

#define MAX_PLAYERS 2
#define MAP_COL 9 /* y | */
#define MAP_ROW 9 /* x --- */
#define MAP_SIZE (MAP_COL * MAP_ROW)
#define TILE_HEIGHT 16 /* y */
#define TILE_WIDTH 16 /* x */
#ifdef _WIN32
    #define BOMB_TICKS 20
#else
    #define BOMB_TICKS 1500 /*50000 pour 0.1 rapide sans explosion*/
#endif
#define ERR_MSG(...) { fprintf(stderr, __VA_ARGS__); exit(1); }

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
    int   current_dir;
    int   current_speed;
    int   max_speed;
    int   bombs_left;
    int   bombs_capacity;
    int   frags;
}               t_player_info;

typedef struct  s_client_request
{
    unsigned int  magic;                  /* Un magic number commun entre le client et le serveur, ou l'identifiant d'un type de structure */
    int           x_pos;                  /* La position x souhait?e par le client */
    int           y_pos;                  /* La position y souhait?e par le client */
    int           dir;                    /* La direction souhait?e par le client */
    int           command;                /* Une commande du client (0 : Ne rien faire / 1 : Poser une bombe) */
    int           speed;                  /* La vitesse du joueur */
    int           ckecksum;               /* Un checksum simple */
}               t_client_request;

typedef char t_map[MAP_SIZE];

typedef struct          s_game
{
    t_player_info        players[MAX_PLAYERS];
    t_map                map;
    t_bomb* bomb;
}t_game;

typedef struct s_server {
    thread_t tid;
    t_game  game;
    int     fds[MAX_PLAYERS];
    struct sockaddr *sock_ptr;
    struct sockaddr_in      sock_serv;
    socklen_t       len;
    #ifdef _WIN32
        SOCKET sockfd;
    #else
        int sockfd;
    #endif
    int port;
    int running;
} t_server;

typedef enum {
    OPTION_QUIT,
    OPTION_HOST,
    OPTION_JOIN,
} menu_option;

void  game_process(t_server*, t_client_request*, int userIndex);
void    game_tick(t_game*);
void    game_init_players(t_game*);
void  bomb_things(t_game* game);
int     server(int host);

#endif
