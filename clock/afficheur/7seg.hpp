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

class Character {
public:
  Character(int,int,int,int,char,SDL_Renderer*);
  void draw();
private:
  SDL_Renderer* _win;
  int _x;
  int _y;
  int _l;
  int _w;
  int _c;

  SDL_Texture* texture;
};

Character::Character(int x, int y, int length, int width, char c, SDL_Renderer* win) {
  _win = win;
  _x = x;
  _y = y;
  _c = c;
  _l = length;
  _w = width;
  texture = SDL_CreateTexture(win, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 256, 256);
  SDL_SetRenderTarget(win, texture);
  SDL_SetRenderDrawColor(win, 183, 186, 18, 255 );

  SDL_RenderClear(win);
  SDL_SetRenderTarget(win, NULL);
}

void Character::draw() {
  int of = _w/2;
  SDL_Rect p1;
  SDL_Rect source = {0,0,4*_l,_w};

  switch (_c) {
    case '/':

      p1.x = _x+_w;
      p1.y = _y;
      p1.h = 4*_l;
      p1.w = _w;

      SDL_RenderCopyEx( _win, texture, &source, &p1, 12, NULL,SDL_FLIP_NONE );
      break;
    case ':':
      p1.x = _x + _l/2 - of;
      p1.y = _y + _l - of;
      p1.h = _w;
      p1.w = _w;
      SDL_RenderFillRect(_win, &p1);
      p1.x = _x + _l/2 - of;
      p1.y = _y + 3*_l - of;
      p1.h = _w;
      p1.w = _w;
      SDL_RenderFillRect(_win, &p1);
      break;
  }
}

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
  std::vector<Character*> chars;
  std::map<std::string,int> mots;
  std::string m_config;

  bool continuer;
  bool _quick;
  std::mutex vars_mutex;
  std::mutex stop_mutex;
};

Segments::Segments(std::string config) {
  m_config = config;
  _quick = false;
}


Segments::Segments(const Segments& in) {
  window = in.window;
  renderer = in.renderer;
  affichages = in.affichages;
  chars = in.chars;
  mots = in.mots;
  continuer = in.continuer;
  _quick = in._quick;
}

Segments::~Segments() {
  for(int i=0;i<affichages.size();i++)
    delete affichages[i];

  for(int i=0;i<chars.size();i++)
    delete chars[i];


  SDL_DestroyWindow(window);
  SDL_Quit();
}

void Segments::run() {
  continuer = true;

  std::string c="";
  bool inmot=false;


  int position_x=0;
  int position_y=0;

  int taille=30;
  int largeur=5;

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
      SeptSeg* sg = new SeptSeg(50+position_x*(taille+10),50+position_y*(4*taille+50),2*taille,largeur,renderer);
      affichages.push_back(sg);
      mots[c] = k;
      k++;
      position_x += 2;
    } else if(m_config[i] == '|') {
      inmot = true;
      c = "";
    } else if(m_config[i] == '\n'){
      position_y++;
      position_x = 0;
    } else {
      if(inmot) {
        c += m_config[i];
      } else {
        if(m_config[i] == '/' || m_config[i] == ':') {
          chars.push_back(new Character(50+position_x*(taille+10),50+position_y*(4*taille+50),taille,largeur,m_config[i],renderer));
        }
        position_x++;
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
    for(unsigned int i=0;i<chars.size();i++) {
      chars[i]->draw();
    }
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
