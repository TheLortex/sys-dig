#include <iostream>
#include "Program.hpp"
#include <vector>

int main(int argc, char* argv[])
{
    if(argc < 4)
    {
	std::cout << "Le programme doit prendre 3 paramètres, le nom de la netlist a compiler, le nom du programme qui va simuler la netlist et la chaine de configuration de l'affichage." << std::endl;
	return 1;
    }
    Program prog;
    try
    {
	//On charge la netlist
      prog.load(argv[1]);
      prog.schedule();
      prog.config(argv[3]);
	//On la compile
      prog.compile(std::string(argv[2] + std::string(".cpp")));

	//On compile le fichier c++
      std::string command = "g++ -o " + std::string(argv[2]) + " " + std::string(argv[2]) + ".cpp -lSDL2 -lpthread -std=c++0x -Ofast -march=native -frename-registers";
      system(command.c_str());
    }
    catch(std::string &s)
    {
	std::cout << s << std::endl;
    }

    return 0;
}
