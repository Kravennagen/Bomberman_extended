#include <SDL.h>
#include "Headers/win_lose.h"

/*
Display image if a player wins
*/
int	win_display(){
	int quit = 1;
	SDL_Event event;
 
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	SDL_Window * window = SDL_CreateWindow("Win display",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
 
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Surface * image = IMG_Load("win.jpg");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
 
	while (quit != 0)
	{
		SDL_WaitEvent(&event);
 
		switch (event.type)
		{
			case SDL_QUIT:
				quit = 0;
				break;
		}
 
		//SDL_Rect dstrect = { 5, 5, 320, 240 };
		//SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
 
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
 
	return 0;
}

/*
Display image if a player loses
*/
int	lose_display(){
	int quit = 1;
	SDL_Event event;
 
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_JPG);
	SDL_Window * window = SDL_CreateWindow("Lose display",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
 
	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Surface * image = IMG_Load("lose.png");
	SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);
 
	while (quit != 0)
	{
		SDL_WaitEvent(&event);
 
		switch (event.type)
		{
			case SDL_QUIT:
				quit = 0;
				break;
		}
 
		//SDL_Rect dstrect = { 5, 5, 320, 240 };
		//SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
 
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
 
	return 0;
}
