#include <SDL2/SDL.h>
#include <iostream>
#include "7seg2.hpp"
#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <thread>


using namespace std;


Segments::Segments(string config) {
  m_config = config;
  value = new vector<vector<bool> >();
}

Segments::Segments(const Segments& in) {
  window = in.window;
  renderer = in.renderer;
  mots = in.mots;
  continuer = in.continuer;
  value  = in.value;
}

Segments::~Segments() {
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Segments::run() {

  int length=150;
  int width=15;

  int of = width/2;

  vector<SDL_Rect> positions = vector<SDL_Rect>(7);
  vector<int> offset_x;
  vector<int> offset_y;

  positions[0].x = 0;
  positions[0].y = of;
  positions[0].w = width;
  positions[0].h = length;

  positions[1].x = of;
  positions[1].y = length;
  positions[1].w = length;
  positions[1].h = width;

  positions[2].x = 0;
  positions[2].y = length+2*of;
  positions[2].w = width;
  positions[2].h = length;

  positions[3].x = of;
  positions[3].y = 2*length+of;
  positions[3].w = length;
  positions[3].h = width;

  positions[4].x = length;
  positions[4].y = 2*of+length;
  positions[4].w = width;
  positions[4].h = length;

  positions[5].x = length;
  positions[5].y = of;
  positions[5].w = width;
  positions[5].h = length;

  positions[6].x = of;
  positions[6].y = 0;
  positions[6].w = length;
  positions[6].h = width;

  continuer = true;

  string c="";
  bool inmot=false;


  int position_x=0;
  int position_y=0;


  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
      std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
      SDL_Quit();
  }

  window = SDL_CreateWindow("oui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

  if(window == 0){
    std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int k = 0;
  for(unsigned int i=0;i<m_config.size();i++) {
    if(m_config[i] == '|' && inmot) {
      inmot = false;
      value->push_back(vector<bool>(7,true));
      offset_x.push_back(position_x*(length+50)+50);
      offset_y.push_back(position_y*(2*length+50)+50);
      mots[c] = k;
      k+=1;
      position_x++;
      cout << c << endl;
    } else if(m_config[i] == '|') {
      inmot = true;
      c = "";
    } else if(m_config[i] == '\n'){
      position_y++;
      position_x = 0;
    } else {
      position_x++;
      if(inmot) {
        c += m_config[i];
      }
    }

  }


  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );
  SDL_RenderClear(renderer);

  stop_mutex.lock();
  while(continuer) {
    stop_mutex.unlock();
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    vars_mutex.lock();

    for(unsigned int i=0;i<value->size();i++) {
      for(int k=0;k<7;k++) {
        if(value->at(i)[k]) {
          SDL_Rect inrect = positions[k];
          inrect.x += offset_x[i];
          inrect.y += offset_y[i];
          SDL_RenderFillRect(renderer, &inrect);
        }
      }
    }
    vars_mutex.unlock();

    SDL_RenderPresent(renderer);
    stop_mutex.lock();

    SDL_Delay(1000/2.);
    //if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
      //continuer = false;
  }
}

void Segments::stop() {
  stop_mutex.lock();
  continuer=false;
  stop_mutex.unlock();
}

void Segments::update(string str, vector<bool> val) {
  vars_mutex.lock();
  int k = mots[str];
  cout << "update " << k << endl;
  for(int i=0;i<7;i++) {
    cout << &value << endl;
    value->at(k)[i] = val[i];
  }
  vars_mutex.unlock();
}



int main() {
  Segments *x = new Segments("|bjr||orvoir|\n  |ici|");
  thread t(&Segments::run, x);
  cout << "par la" << endl;
  vector<bool> oui = {1,1,1,0,1,1,1};
  SDL_Delay(300);
  x->update("bjr",oui);
  x->update("orvoir",oui);
  x->update("ici",oui);
  SDL_Delay(2000);
  x->stop();
  delete x;
  t.join();

}
