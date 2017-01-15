#include <SDL2/SDL.h>
#include <iostream>
#include "7seg.hpp"
#include <string>
#include <map>
#include <ctime>

#include <thread>


using namespace std;






int main() {
  Segments x("|bjr||orvoir|\n  |ici|");
  thread t(&Segments::run, &x);
  cout << "par la" << endl;
  vector<bool> oui = {1,0,1,0,1,1,1};
  SDL_Delay(300);
  x.update("bjr",oui);
  x.update("orvoir",oui);
  x.update("ici",oui);
  SDL_Delay(2000);
  x.stop();
  t.join();

}
