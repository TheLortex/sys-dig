#include <SDL2/SDL.h>
#include <vector>
#include <mutex>
#include <string>
#include <map>

class Segments {
public:
  Segments(const Segments&);
  Segments(std::string config);
  ~Segments();

  void update(std::string,std::vector<bool>);
  void stop();
  void run();

private:
  SDL_Window* window;
  SDL_Renderer* renderer;
  std::vector<std::vector<bool> > *value;
  std::map<std::string,int> mots;
  std::string m_config;

  bool continuer;
  std::mutex vars_mutex;
  std::mutex stop_mutex;
};
 
