#include <iostream>
#include "Program.hpp"
#include <vector>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
	std::cout << "Le programme doit prendre 2 paramètres, le nom de la netlist a compiler et le nom du programme qui va simuler la netlist" << std::endl;
	return 1;
    }
    Program prog;
    try
    {
	//On charge la netlist
        prog.load(argv[1]);
	prog.schedule();

	//On la compile
	prog.compile(std::string(argv[2] + std::string(".cpp")));

	//On compile le fichier c++
	std::string command = "g++ -std=c++11 -O3 -o " + std::string(argv[2]) + " " + std::string(argv[2]) + ".cpp";
	system(command.c_str());
    }
    catch(std::string &s)
    {
	std::cout << s << std::endl;
    }
    
    return 0;
}
