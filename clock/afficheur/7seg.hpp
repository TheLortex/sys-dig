#include <SDL2/SDL.h>
#include <vector>
#include <mutex>
#include <string>
#include <map>

class SeptSeg {
  public:
    SeptSeg(int,int,int,int,SDL_Renderer*);
    ~SeptSeg();

    void set_value(uint8_t);
    void draw();

  private:
    uint8_t value;
    SDL_Renderer* win;
    int x;
    int y;
    std::vector<SDL_Rect> positions;
};


class Segments {
public:
  Segments(const Segments&);
  Segments(std::string config);
  ~Segments();

  void update(std::string,uint8_t val);
  void stop();
  void run();
  bool stopped() {
    return !continuer;
  }
  bool quick() {
    return _quick;
  }

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  std::vector<SeptSeg*> affichages;
  std::map<std::string,int> mots;
  std::string m_config;

  bool continuer;
  bool _quick;
  std::mutex vars_mutex;
  std::mutex stop_mutex;
};

SeptSeg::SeptSeg(int x_, int y_, int length, int width, SDL_Renderer* win_) {
  win = win_;
  x = x_;
  y = y_;

  int of = width/2;

  positions = std::vector<SDL_Rect>(7);

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

  for(int i=0;i<7;i++) {
    positions[i].x += x;
    positions[i].y += y;
  }

  value = 255;
}
SeptSeg::~SeptSeg() {}

void SeptSeg::set_value(uint8_t val) {
  value = val;
}

void SeptSeg::draw() {
  for(int i=0;i<7;i++)
    if(value & (1 << i))
      SDL_RenderFillRect(win, &positions[6-i]);

}


Segments::Segments(std::string config) {
  m_config = config;
  _quick = false;
}


Segments::Segments(const Segments& in) {
  window = in.window;
  renderer = in.renderer;
  affichages = in.affichages;
  mots = in.mots;
  continuer = in.continuer;
  _quick = in._quick;
}

Segments::~Segments() {
  for(int i=0;i<affichages.size();i++)
    delete affichages[i];


  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Segments::run() {
  continuer = true;

  std::string c="";
  bool inmot=false;


  int position_x=0;
  int position_y=0;

  int taille=80;
  int largeur=8;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
      std::cout << "Erreur lors de l'initialisation de la SDL : " << SDL_GetError() << std::endl;
      SDL_Quit();
  }

  window = SDL_CreateWindow("oui", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

  if(window == 0){
    std::cout << "Erreur lors de la creation de la fenetre : " << SDL_GetError() << std::endl;
    SDL_Quit();
  }

  renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  int k = 0;
  for(unsigned int i=0;i<m_config.size();i++) {
    if(m_config[i] == '|' && inmot) {
      inmot = false;
      SeptSeg* sg = new SeptSeg(50+position_x*(taille+50),50+position_y*(2*taille+50),taille,largeur,renderer);
      affichages.push_back(sg);
      mots[c] = k;
      k++;
      position_x++;
    } else if(m_config[i] == '|') {
      inmot = true;
      c = "";
    } else if(m_config[i] == '\n'){
      position_y++;
      position_x = 0;
    } else {
      if(inmot) {
        c += m_config[i];
      }
    }

  }


  SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
  SDL_RenderClear(renderer);

  SDL_Event evenements;

  while(continuer) {
    stop_mutex.unlock();
    SDL_SetRenderDrawColor( renderer, 41, 22, 61, 255 );
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor( renderer, 183, 186, 18, 255 );
    vars_mutex.lock();
    for(unsigned int i=0;i<affichages.size();i++) {
      affichages[i]->draw();
    }
    vars_mutex.unlock();

    SDL_RenderPresent(renderer);

    while(SDL_PollEvent(&evenements)) {
      if ( evenements.window.event == SDL_WINDOWEVENT_CLOSE ) {
          continuer = false;
      } else if(evenements.type == SDL_KEYDOWN) {
        if (evenements.key.keysym.sym == SDLK_q) {
          _quick = true;
        } else if (evenements.key.keysym.sym == SDLK_s) {
          _quick = false;
        }
      }
    }


    SDL_Delay(1000/128.);
  }
}



void Segments::stop() {
  continuer=false;
}

void Segments::update(std::string str, uint8_t val) {
  vars_mutex.lock();
  affichages[mots[str]]->set_value(val);
  vars_mutex.unlock();
}
