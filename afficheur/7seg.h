#include <SDL2/SDL.h>

class Segments {
  public:
    Segments(int,int,int,int,SDL_Renderer*);
    ~Segments();

    void set_value(bool[7]);
    void draw();

  private:
    bool value[7];
    SDL_Renderer* win;
    int x;
    int y;
    SDL_Rect positions[7];
};
