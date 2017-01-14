#include <SDL2/SDL.h>
#include <iostream>
#include "7seg.h"

Segments::Segments(int x_, int y_, int length, int width, SDL_Renderer* win_) {
  win = win_;
  x = x_;
  y = y_;


  positions[0].x = 0;
  positions[0].y = 0;
  positions[0].w = width;
  positions[0].h = length;

  positions[1].x = 0;
  positions[1].y = length;
  positions[1].w = length;
  positions[1].h = width;

  positions[2].x = 0;
  positions[2].y = length;
  positions[2].w = width;
  positions[2].h = length;

  positions[3].x = 0;
  positions[3].y = 2*length;
  positions[3].w = length+width;
  positions[3].h = width;

  positions[4].x = length;
  positions[4].y = length;
  positions[4].w = width;
  positions[4].h = length;

  positions[5].x = length;
  positions[5].y = 0;
  positions[5].w = width;
  positions[5].h = length;

  positions[6].x = 0;
  positions[6].y = 0;
  positions[6].w = length;
  positions[6].h = width;

  for(int i=0;i<7;i++) {
    positions[i].x += x;
    positions[i].y += y;
  }


}
Segments::~Segments() {}
void Segments::set_value(bool val[7]) {
  for(int i=0;i<7;i++)
    value[i] = val[i];
}

void Segments::draw() {
  for(int i=0;i<7;i++)
    if(value[i])
      SDL_RenderFillRect(win, &positions[i]);

}


int main(int argc, char **argv)
{
    // Initialisation de la SDL

    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
        SDL_Quit();

        return -1;
    }

    SDL_Window* fenetre(0);
    fenetre = SDL_CreateWindow("oui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if(fenetre == 0){
      std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
      SDL_Quit();
      return -1;
    }

    SDL_Renderer* renderer =  SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);

    Segments s(50,50,150,15,renderer);
    Segments s2(220,50,150,15,renderer);
    bool tab[7];
    for(int i=0;i<7;i++)
      tab[i] = true;
    s.set_value(tab);
    s2.set_value(tab);
    bool terminer = false;
    SDL_Event evenements;
    while(!terminer){
      SDL_WaitEvent(&evenements);
      SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
      SDL_RenderClear(renderer);
      SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
      s.draw();
      s2.draw();
      SDL_RenderPresent(renderer);
      if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
        terminer = true;
    }


    SDL_DestroyWindow(fenetre);
    SDL_Quit();
    return 0;
}
