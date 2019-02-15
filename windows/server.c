#include "bomberman.h"
#include "client.h"

static void	bind_clients(t_server *server)
{
	printf("Binding clients (total %d)\n", MAX_PLAYERS);
	for (int i = 0; i < MAX_PLAYERS; ++i)
	{
		server->fds[i] = -1;
		while (-1 == server->fds[i])
			server->fds[i] = accept(server->sockfd, server->sock_ptr,
					&server->len);
		#ifdef _WIN32
			send(server->fds[i], (const char *)&i, sizeof i, 0);
		#else
			write(server->fds[i], &i, sizeof i, 0);
		#endif
		printf("Connected player #%d\n", i + 1);
	}
}

/* receives a command from the client
 * returns 0 on read length 0 (dc) */
static int	handle_client(t_server *server, fd_set *readfs, int userIndex)
{
	if (!server->game.players[userIndex].alive)
		return 1;
	int sockfd = server->fds[userIndex];
	char	buffer[sizeof(t_client_request)];

	if (FD_ISSET(sockfd, readfs))
	{
		size_t left = sizeof(t_client_request);
		char *buffLeft = buffer;
		while (left > 0)
		{
			#ifdef _WIN32
		      int count = recv(sockfd, buffLeft, left, 0);
		    #else
		      int count = read(sockfd, buffLeft, left);
		    #endif   
			if (count == 0)
			{
				FD_CLR(sockfd, readfs);
				return (0);
			}
			left -= count;
			buffLeft += count;
		}
		FD_CLR(sockfd, readfs);
		game_process(server, (t_client_request*)buffer, userIndex);
	}
	return (1);
}

static
#ifdef _WIN32
void
#else
void*
#endif
game_start(void* _server)
{
	t_server *server = _server;
	struct timeval	tv;
	fd_set	readfs;

	bind_clients(server);
	map_init(server->game.map);
	game_init_players(&server->game);
	while (server->running)
	{
		FD_ZERO(&readfs);
		for (int i = 0; i < MAX_PLAYERS; ++i)
			if (-1 == server->fds[i])
                #ifdef _WIN32
                    return;
                #else
                    return NULL;
                #endif
			else
				FD_SET(server->fds[i], &readfs);
		tv.tv_sec = tv.tv_usec = 0;
		select(server->fds[MAX_PLAYERS - 1] + 1, &readfs, NULL, NULL, &tv);
		/* read actions for each player */
		for (int i = 0; i < MAX_PLAYERS; ++i)
			if (server->fds[i] > 0 && !handle_client(server, &readfs, i))
				#ifdef _WIN32
                    return;
                #else
                    return NULL;
                #endif
		game_tick(&server->game);
		for (int i = 0; i < MAX_PLAYERS; ++i)
	    {
			if(server->game.players[i].connected)
			{
			    if (server->game.players[i].alive) {
			    	#ifdef _WIN32
						send(server->fds[i], (const char*)&server->game, sizeof server->game, 0);
					#else
						write(server->fds[i], &server->game, sizeof server->game);
					#endif
				} else
				{
					server->game.players[i].connected = 0;
					#ifdef _WIN32
						send(server->fds[i], (const char*)&server->game, sizeof server->game, 0);
					#else
						write(server->fds[i], &server->game, sizeof server->game);
					#endif
				}
			}
		}
		sleep(1);
	}

	#ifdef _WIN32
        return;
    #else
        return NULL;
    #endif
}

//Configure server socket
static int  prepare_server(t_server* server)
{
	server->sock_serv.sin_family = AF_INET;
#ifdef _WIN32
    server->sock_serv.sin_port = htons((u_short) server->port);
#else
    server->sock_serv.sin_port = htons(server->port);
#endif
    server->sock_serv.sin_addr.s_addr = INADDR_ANY;
	server->len = sizeof(server->sock_serv);
    server->sockfd = socket(AF_INET, SOCK_STREAM, 0);
#ifdef _WIN32
    if (INVALID_SOCKET == server->sockfd)
        ERR_MSG("sockfd is INVALID_SOCKET: %d\n", WSAGetLastError());
#else
    if (-1 == server->sockfd)
        ERR_MSG("sockfd is -1\n");
#endif
    server->sock_ptr = (struct sockaddr*)&server->sock_serv;

    if (
#ifdef _WIN32
        INVALID_SOCKET
#else
        -1
#endif
            == bind(server->sockfd, server->sock_ptr, server->len))
        ERR_MSG("could not bind, errno=%d\n", errno);
#ifdef _WIN32
    u_long mode = 1; /* nonblocking mode */
    ioctlsocket(server->sockfd, FIONBIO, &mode);
#endif
    listen(server->sockfd, MAX_PLAYERS);
    return (1);
}

#ifdef _WIN32
struct windows_thread_wrapper
{
    thread_fn fn;
    void* data;
};

/* wraps the thread function to return 0 */
static unsigned __stdcall wrap_thread_create(void* tw)
{
    struct windows_thread_wrapper* w = tw;
    w->fn(w->data);
    free(tw);
    return 0;
}

void thread_create(thread_t* t, thread_fn fn, void* data)
{
    struct windows_thread_wrapper *tw;
    tw = malloc(sizeof *tw);
    if (!tw)
        ERR_MSG("Unable to alloc thread wrapper");
    tw->fn = fn;
    tw->data = data;
    *t = (HANDLE)_beginthreadex(
            NULL /* security */,
            0 /* stack size */,
            wrap_thread_create,
            tw,
            0 /* initflag */,
            NULL /* thread id */
    );
    if (0 == *t)
    ERR_MSG("Unable to _beginthreadex");
}

void thread_join(thread_t tid)
{
    WaitForSingleObject(tid, INFINITE);
}
#endif

SDL_Window *window(void);
int	server(int port)
{
	t_server server;
	void* discard_return;

	server.running = 1;
    server.port = port;
    window();

	if (!prepare_server(&server))
		return (1);

	#ifdef _WIN32
		thread_create(&server.tid, game_start, &server);
		client("127.0.0.1", server.port);

		server.running = 0;
		thread_join(server.tid);
    #else
    	pthread_create(&server.tid, NULL, game_start, &server);
    	client("127.0.0.1", server.port);

    	server.running = 0; /* TODO mutex */
    	pthread_join(server.tid, &discard_return);
	#endif

	return (0);
}