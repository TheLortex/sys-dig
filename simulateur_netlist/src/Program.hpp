#ifndef PROGRAM_HPP
#define PROGRAM_HPP

#include <string>
#include <vector>

class Var;
class VarConst;
class Expression;
class Parser;

class Program
{
    friend class Parser;
public:
    Program();
    ~Program();

    //Affiche la netlist triée (ne sert que pour le debuggage)
    void display();

    //Charge une netlist depuis un fichier
    void load(std::string const &file_name);

    //Trie la netlist
    void schedule();

    //Compile la netlist dans un fichier cpp
    void compile(std::string const &file_name);
private:
    //Renvoie le type optimal pour représenter une nappe de fils
    std::string get_type(int size);

    //Fonctions compilant au fur et a mesure le fichier cpp
    void write_declare_variables(std::ofstream &cppfile);
    void write_read_variables(std::ofstream &cppfile);
    void write_read_roms(std::ofstream &cppfile);
    void write_iterations(std::ofstream &cppfile);
    void write_display_output(std::ofstream &cppfile);
    
    std::vector<Var*> _input; //Contient les fils d'input
    std::vector<Var*> _output;//Contient les fils d'output
    std::vector<Var*> _vars;//Contient l'ensemble des variables
    std::vector<Var*> _const_vars;//Contient l'ensemble des constantes (pour réduire la mémoire)
    std::vector<Expression*> _expressions; //Contient l'ensemble des expressions
};

#endif
