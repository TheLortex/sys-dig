#include "Program.hpp"

#include "Graph.hpp"
#include "types/Var.hpp"
#include "types/VarConst.hpp"
#include "types/Expression.hpp"
#include "types/ExpressionReg.hpp"
#include "types/ExpressionRam.hpp"
#include "types/ExpressionRom.hpp"
#include "parser/Parser.hpp"

#include <unordered_map>
#include <iostream>
#include <unordered_map>
#include <fstream>

Program::Program()
{

}

Program::~Program()
{
    for(Var *v: _vars)
    {
	delete v;
    }
    for(Var *v: _const_vars)
    {
	delete v;
    }
    for(Expression* e: _expressions)
    {
	delete e;
    }
}

void Program::display()
{
    for(Expression* e: _expressions)
    {
	std::cout << e->get_expression() << std::endl;
    }
}

void Program::load(std::string const &file_name)
{
    Parser parser;
    parser.parse(file_name,this);
    _display_vars = std::vector<Var*>();
}

void Program::schedule()
{
    //Les variables qui vont avoir une affectation
    std::vector<Var*> var_to_calculate;

    for(Var *v: _vars)
    {
        if(!v->has_value())
        {
            var_to_calculate.push_back(v);
        }
    }
    //On ajoute des id aux variables pour les associés aux noeuds d'un graphe qu'on va trier topologiquement
    for(unsigned int i=0; i<var_to_calculate.size(); i++)
    {
        var_to_calculate[i]->set_id(i);
    }

    //On ajoute les arêtes représentant les dépendances
    Graph graph(var_to_calculate.size());
    for(Expression* e: _expressions)
    {
        for(Var* v: e->get_dependencies())
        {
            graph.add_edge(e->get_result()->get_id(), v->get_id());
        }
    }

    //On trie les expressions grace au tri topologique
    std::vector<int> topo_sort = graph.topological_sort();
    std::pair<std::unordered_multimap<int,Expression*>::iterator, std::unordered_multimap<int,Expression*>::iterator> elems;

    std::unordered_multimap<int, Expression*> expressions_map;
    for(Expression* e: _expressions)
    {
	expressions_map.insert(std::make_pair(e->get_result()->get_id(), e));
    }

    std::vector<Expression*> new_expressions;
    for(int i: topo_sort)
    {
        elems = expressions_map.equal_range(i);
	for(std::unordered_multimap<int, Expression*>::iterator it=elems.first; it!=elems.second; ++it)
	{
	    new_expressions.push_back(it->second);
	}
    }
    _expressions = new_expressions;
}

void Program::config(std::string config_string) {
    _display_string = config_string;
    std::string k = "";
    bool invar = false;
    std::cout << _display_vars.size() << std::endl;
    for(char c : config_string) {
        if(invar && c == '|') {
            invar = false;
            for(Var* v : _vars) {
                if(v->get_name() == k) {
                    _display_vars.push_back(v);
                }
            }
        } else if(c == '|') {
            invar = true;
            k = "";
        } else if(invar){
            k += c;
        }
    }
    std::cout << _display_vars.size() << std::endl;
}

void Program::compile(std::string const &name)
{

    std::ofstream cppfile (name,std::ios::out | std::ios::trunc);

    if(!cppfile.is_open())
    {
	throw std::string("Can't open file " + name);
    }
    cppfile << "#include <iostream>\n";
    cppfile << "#include \"stdint.h\"\n";
    cppfile << "#include <fstream>\n";
    cppfile << "#include <bitset>\n";
    cppfile << "#include <thread>\n";
    cppfile << "#include <ctime>\n";
    cppfile << "#include <array>\n\n";
    cppfile << "#include \"afficheur/7seg.hpp\"\n";

    cppfile << "int main(int argc, char** argv)\n";
    cppfile << "{\n";

    //On déclare les variables
    write_declare_variables(cppfile);

    //On lit les fichiers contenant les roms
    write_read_roms(cppfile);

    //On écrit la boucle principale
    write_iterations(cppfile);
    cppfile << "\nx.stop();\nt.join();\n}";


    cppfile.close();
}

std::string Program::get_type(int size)
{
    if(size == 1)
    {
	return "bool";
    }
    else if(size <= 8)
    {
	return "uint8_t";
    }
    else if(size <= 16)
    {
	return "uint16_t";
    }
    else if(size <= 32)
    {
	return "uint32_t";
    }
    else
    {
	return "uint64_t";
    }
}

void Program::write_declare_variables(std::ofstream &cppfile)
{
    cppfile << "//Declaration of variables\n\n";
    cppfile << "Segments x(\"" << _display_string << "\");\n";
    cppfile << "std::thread t(&Segments::run, &x);\nSDL_Delay(300);\n";

    for(Var* v: _vars)
    {
	cppfile << get_type(v->get_size()) << " " << v->get_cpp_name() << " = 0;\n";
    }

    cppfile << "\n//Declaration of registers\n\n";

    for(Var* v: ExpressionReg::get_registers())
    {
	cppfile << get_type(v->get_size()) << " " << v->get_name() << "_reg = 0;\n";
    }

    cppfile << "\n//Declaration of roms\n\n";

    for(std::pair<int,int> e: ExpressionRom::get_roms_size())
    {
	cppfile << "std::array<" << get_type(e.second) << "," << (1 << e.first) << "> rom" << e.first << ";";
    }

    cppfile << "\n//Declaration of rams\n\n";

    for(std::pair<int,int> e: ExpressionRam::get_rams_size())
    {
	if(e.first <= 16)
	{
	    cppfile << "std::array<" << get_type(e.second) << "," << (1 << e.first) << "> ram" << e.first << ";\n";
	    cppfile << "ram" << e.first << ".fill(0);\n";
	}
	else
	{
	    cppfile << "std::unordered_map<" << get_type(e.first) << "," << get_type(e.second) << "> ram" << e.first << ";\n";
	}
    }
}

void Program::write_read_roms(std::ofstream &cppfile)
{
    cppfile << "std::string temp;\n";
    cppfile << "std::ifstream romfile;\n";
    for(std::pair<int,int> e: ExpressionRom::get_roms_size())
    {
	cppfile << "std::cout << \"Nom du fichier contenant la rom adressée sur " << e.first << " bits :\" << std::endl;\n";
	cppfile << "std::cin >> temp;\n";

	cppfile << "romfile.open(temp);\n";

	cppfile << "if(!romfile.is_open()){std::cout << \"Erreur lors de l'ouverture du fichier \" << temp << \".\" << std::endl; return 1;}\n";

	cppfile << "for(int i = 0; i<" << (1 << e.first) << "; ++i)\n{\n";
	cppfile << "if(!getline(romfile,temp)){break;}\n";
	cppfile << "rom" << e.first << "[i] = std::stoll(temp, nullptr, 2);\n";
	cppfile << "}\n";

	cppfile << "romfile.close();\n";
    }
}

void Program::write_read_variables(std::ofstream &cppfile)
{
    cppfile << "//Read variables\n\n";
    cppfile << "std::string tempvar;\n";

    for(Var *v: _input)
    {
      if(v->get_name() == "clock") {
        cppfile << v->get_cpp_name() << " = " << "time(NULL) % 2; ";
      } else {
        cppfile << "std::cout << \"" << v->get_name() << " (taille de la nappe : " << v->get_size() << ") : \";\n";
      	cppfile << "std::cin >> tempvar;\n";
      	cppfile << v->get_cpp_name() << " = " << "std::stoll(tempvar,nullptr,2);\n";
      }
    }

    cppfile << "\n";
}

void Program::write_iterations(std::ofstream &cppfile)
{
    cppfile << "int VAR;\nstd::cout << \" Number of cycles to calculate \" << std::endl;\nstd::cin>>VAR;\nfor(;VAR>0; --VAR)\n{\n";


    //On lit les variables d'input
    write_read_variables(cppfile);

    for(Expression *e: _expressions)
    {
        try
        {
            cppfile << e->get_instructions_beginning() << "\n";
        }
        catch(std::string e){}
    }
    for(Expression *e: _expressions)
    {
        try
        {
            cppfile << e->get_instructions_mid() << "\n";
        }
        catch(std::string e){}
    }
    for(Expression *e: _expressions)
    {
        try
        {
            cppfile << e->get_instructions_end() << "\n";
        }
        catch(std::string e){}
    }

    //On affiche le résultat
    write_display_output(cppfile);
    cppfile << "std::cout << std::endl << std::endl;\n";
    cppfile << "}\n\n";
}

void Program::write_display_output(std::ofstream &cppfile)
{
    cppfile << "//Displaying results\n\n";

    for(Var *v: _output)
    {
        cppfile << "std::cout << \"" << v->get_name() << " : \" << std::bitset<" << v->get_size() << ">(" << v->get_cpp_name();
        if(v->get_size() < 64)
        {
            cppfile << " % " << (uint64_t)((uint64_t)(1) << (uint64_t)(v->get_size()));
        }
        cppfile << ") << std::endl;\n";
    }

    cppfile << "if(x.stopped()) {t.join();return 0;}\n";
    for(Var* v : _display_vars) {
      cppfile << "x.update(\"" << v->get_name() << "\"," << v->get_cpp_name() << ");\n";
    }
    cppfile << "\n";
}
