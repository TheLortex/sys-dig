#include "Parser.hpp"
#include <iostream>
#include <fstream>

#include "../types/Var.hpp"
#include "../types/VarConst.hpp"
#include "../types/ExpressionBinary.hpp"
#include "../types/ExpressionUnary.hpp"
#include "../types/ExpressionSelect.hpp"
#include "../types/ExpressionSlice.hpp"
#include "../types/ExpressionReg.hpp"
#include "../types/ExpressionRom.hpp"
#include "../types/ExpressionRam.hpp"
#include "../types/ExpressionMux.hpp"
#include "../Program.hpp"

std::set<std::string> Parser::reserved_names = {"INPUT", "OUTPUT", "VAR", "IN", "SELECT", "SLICE", "CONCAT", "REG", "RAM", "ROM", "OR", "XOR", "AND", "NAND", "MUX"};


Parser::Parser()
{
    init_sm();
}

void Parser::init_sm()
{
    lexertl::rules _rules;
    lexertl::state_machine state_machine;

    //A macro to represent authorized names for variable
    _rules.insert_macro("var", "[a-zA-Z_][a-zA-z0-9_]*");
    _rules.insert_macro("const", "\\d+");
    _rules.insert_macro("varconst", "[1|0]+");
    _rules.insert_macro("space", "[ \\t\\r\\n]");
    _rules.insert_macro("var_separator", ",");
    _rules.insert_macro("width_separator", ":");

    //The states of the state machine
    _rules.push_state("INPUT_INIT");
    _rules.push_state("INPUT_FINISHED_VAR");
    _rules.push_state("INPUT_WAIT_VAR");
    _rules.push_state("OUTPUT_INIT");
    _rules.push_state("OUTPUT_FINISHED_VAR");
    _rules.push_state("OUTPUT_WAIT_VAR");
    _rules.push_state("VAR_INIT");
    _rules.push_state("VAR_FINISHED_VAR");
    _rules.push_state("VAR_WAIT_WIDTH");
    _rules.push_state("VAR_FINISHED_VAR_WIDTH");
    _rules.push_state("VAR_WAIT_VAR");

    _rules.push_state("EQ_INIT");
    _rules.push_state("EQ_FINISHED_LVAR");
    _rules.push_state("EQ_WAIT_OP");
    _rules.push_state("EQ_WAIT_VAR_1");
    _rules.push_state("EQ_WAIT_VAR_2");
    _rules.push_state("EQ_WAIT_VAR_3");
    _rules.push_state("EQ_WAIT_CONST_2");
    _rules.push_state("EQ_WAIT_CONST_1");

    _rules.push_state("EQ_RAM_6");
    _rules.push_state("EQ_RAM_5");
    _rules.push_state("EQ_RAM_4");
    _rules.push_state("EQ_RAM_3");
    _rules.push_state("EQ_RAM_2");
    _rules.push_state("EQ_RAM_1");
    _rules.push_state("EQ_ROM_3");
    _rules.push_state("EQ_ROM_2");
    _rules.push_state("EQ_ROM_1");


    //The transition of the state machine, by order of importance
    _rules.push("INITIAL", "\"INPUT\"{space}", eBLOCSEPARATOR, "INPUT_INIT");
    _rules.push("INPUT_FINISHED_VAR,INPUT_INIT","\"OUTPUT\"{space}", eBLOCSEPARATOR, "OUTPUT_INIT");
    _rules.push("OUTPUT_FINISHED_VAR,OUTPUT_INIT","\"VAR\"{space}", eBLOCSEPARATOR, "VAR_INIT");
    _rules.push("VAR_FINISHED_VAR,VAR_INIT,VAR_FINISHED_VAR_WIDTH", "\"IN\"{space}", eBLOCSEPARATOR, "EQ_INIT");
    _rules.push("*","{space}+", _rules.skip(), ".");
    
    _rules.push("INPUT_FINISHED_VAR", "{var_separator}",eVARSEPARATOR, "INPUT_WAIT_VAR");
    _rules.push("INPUT_INIT,INPUT_WAIT_VAR", "{var}", eIN, "INPUT_FINISHED_VAR");
    
    _rules.push("OUTPUT_FINISHED_VAR", "{var_separator}",eVARSEPARATOR, "OUTPUT_WAIT_VAR");
    _rules.push("OUTPUT_INIT,OUTPUT_WAIT_VAR", "{var}", eOUT, "OUTPUT_FINISHED_VAR");
    
    _rules.push("VAR_INIT,VAR_WAIT_VAR", "{var}", eVAR, "VAR_FINISHED_VAR");
    _rules.push("VAR_FINISHED_VAR,VAR_FINISHED_VAR_WIDTH", "{var_separator}",eVARSEPARATOR, "VAR_WAIT_VAR");
    _rules.push("VAR_FINISHED_VAR", "{width_separator}", eVARISMULTIPLE, "VAR_WAIT_WIDTH");
    _rules.push("VAR_WAIT_WIDTH", "\\d+", eVARWIDTH, "VAR_FINISHED_VAR_WIDTH");

    _rules.push("EQ_INIT", "{var}", eEQLVAR, "EQ_FINISHED_LVAR");
    _rules.push("EQ_FINISHED_LVAR", "[=]", _rules.skip(), "EQ_WAIT_OP");
    _rules.push("EQ_WAIT_OP", "\"MUX\"", eOP, "EQ_WAIT_VAR_3");
    _rules.push("EQ_WAIT_OP", "\"OR\"|\"XOR\"|\"AND\"|\"NAND\"|\"CONCAT\"", eOP, "EQ_WAIT_VAR_2");
    _rules.push("EQ_WAIT_OP", "\"NOT\"|\"REG\"", eOP, "EQ_WAIT_VAR_1");
    _rules.push("EQ_WAIT_OP", "\"SLICE\"", eOP, "EQ_WAIT_CONST_2");
    _rules.push("EQ_WAIT_OP", "\"SELECT\"", eOP, "EQ_WAIT_CONST_1");
    _rules.push("EQ_WAIT_OP", "\"RAM\"", eOP, "EQ_RAM_6");
    _rules.push("EQ_WAIT_OP", "\"ROM\"", eOP, "EQ_ROM_3");
    _rules.push("EQ_WAIT_OP", "{var}", eEQRVAR, "EQ_INIT");
    _rules.push("EQ_WAIT_OP", "{varconst}", eEQRCONSTVAR, "EQ_INIT");
    
    _rules.push("EQ_WAIT_VAR_3", "{var}", eEQRVAR, "EQ_WAIT_VAR_2");
    _rules.push("EQ_WAIT_VAR_3", "{varconst}", eEQRCONSTVAR, "EQ_WAIT_VAR_2");
    _rules.push("EQ_WAIT_VAR_2", "{var}", eEQRVAR, "EQ_WAIT_VAR_1");
    _rules.push("EQ_WAIT_VAR_2", "{varconst}", eEQRCONSTVAR, "EQ_WAIT_VAR_1");
    _rules.push("EQ_WAIT_CONST_2", "{const}", eEQRCONST, "EQ_WAIT_CONST_1");
    _rules.push("EQ_WAIT_CONST_1", "{const}", eEQRCONST, "EQ_WAIT_VAR_1");
    _rules.push("EQ_WAIT_VAR_1", "{var}", eEQRVAR, "EQ_INIT");
    _rules.push("EQ_WAIT_VAR_1", "{varconst}", eEQRCONSTVAR, "EQ_INIT");

    _rules.push("EQ_RAM_6", "{const}", eEQRCONST, "EQ_RAM_5");
    _rules.push("EQ_RAM_5", "{const}", eEQRCONST, "EQ_RAM_4");
    _rules.push("EQ_RAM_4", "{var}", eEQRVAR, "EQ_RAM_3");
    _rules.push("EQ_RAM_4", "{varconst}", eEQRCONSTVAR, "EQ_RAM_3");
    _rules.push("EQ_RAM_3", "{var}", eEQRVAR, "EQ_RAM_2");
    _rules.push("EQ_RAM_3", "{varconst}", eEQRCONSTVAR, "EQ_RAM_2");
    _rules.push("EQ_RAM_2", "{var}", eEQRVAR, "EQ_RAM_1");
    _rules.push("EQ_RAM_2", "{varconst}", eEQRCONSTVAR, "EQ_RAM_1");
    _rules.push("EQ_RAM_1", "{var}", eEQRVAR, "EQ_INIT");
    _rules.push("EQ_RAM_1", "{varconst}", eEQRCONSTVAR, "EQ_INIT");

    _rules.push("EQ_ROM_3", "{const}", eEQRCONST, "EQ_ROM_2");
    _rules.push("EQ_ROM_2", "{const}", eEQRCONST, "EQ_ROM_1");
    _rules.push("EQ_ROM_1", "{var}", eEQRVAR, "EQ_INIT");
    _rules.push("EQ_ROM_1", "{varconst}", eEQRCONSTVAR, "EQ_INIT");
    lexertl::generator::build(_rules, _sm);
}

void Parser::parse(std::string const &file_name, Program *program)
{
    _p = program;
    _p->_output.clear();
    _p->_input.clear();
    _p->_vars.clear();
    _p->_expressions.clear();

    std::set<std::string> temp_input, temp_output;
    std::unordered_map<std::string, Var*> values_map;
    
    std::ifstream ifi(file_name.c_str());
    if(!ifi.is_open())
    {
	throw std::string("Erreur lors de l'ouverture du fichier ") + (file_name.c_str());
    }
    lexertl::stream_shared_iterator iter(ifi);
    lexertl::stream_shared_iterator end;
    
    lexertl::match_results<lexertl::stream_shared_iterator> results(iter,end);
    lexertl::lookup(_sm, results);


    //On lit les tokens un à un
    while (results.id != 0)
    {
        switch (results.id)
        {
	case eIN:
	    if(temp_input.find(results.str()) == temp_input.end())
	    {
		temp_input.insert(results.str());
	    }
	    else
	    {
		throw std::string("Erreur : Deux variables ont le même nom dans le bloc INPUT.");
	    }
	    break;
	case eOUT:
	    if(temp_output.find(results.str()) == temp_output.end())
	    {
		temp_output.insert(results.str());
	    }
	    else
	    {
		throw std::string("Erreur : Deux variables ont le même nom dans le bloc OUTPUT");
	    }
	    break;
	case eVAR:
	    parse_var(results.str(),results);
	    break;
	case eEQLVAR:
	    parse_expr(results.str(),results);
	    break;
	case eBLOCSEPARATOR:
	    break;
	case eVARSEPARATOR:
	    break;
	default:
	    throw std::string("Erreur inconnue lors du parsing du fichier.");
	    break;
        }

        lexertl::lookup(_sm, results);
    }

    for(std::string const &s: temp_input)
    {
        _p->_input.push_back(get_var(s));
    }
    for(std::string const &s: temp_output)
    {
	_p->_output.push_back(get_var(s));
    }
}

Var* Parser::get_var(std::string const &name)
{
    	std::unordered_map<std::string,Var*>::const_iterator it = _values_map.find(name);
	if(it != _values_map.end())
	{
	    return it->second;
        }
	else
	{
            throw std::string("Erreur : La valeur " + name  + " n'a pas été déclarée dans la section VAR");
        }
}

Var* Parser::get_var_const(std::string const &value)
{
    std::unordered_map<std::string,Var*>::const_iterator it = _values_map.find(value);
    if(it != _values_map.end())
    {
	return it->second;
    }
    else
    {
	VarConst* varc = new VarConst(value);
	_values_map.insert(std::make_pair(value,varc));
	_p->_const_vars.push_back(varc);
	return varc;
    }
}


void Parser::parse_var(std::string const &name,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    if(reserved_names.find(name) != reserved_names.end())
    {
	throw std::string("Erreur : Le nom " + name + " ne peut pas être le nom d'une variable");
    }
    lexertl::lookup(_sm,results);
    if(results.id == eVARSEPARATOR || results.id == eBLOCSEPARATOR)
    {
	if(_values_map.find(name) == _values_map.end())
	{
	    Var* temp = new Var(name,1);
	    _values_map.insert(make_pair(name,temp));
	    _p->_vars.push_back(temp);
        }
	else
	{
	    throw std::string("Erreur : La variable " + name+ " a déjà été déclarée dans la section VAR");
        }
    }
    else if(results.id == eVARISMULTIPLE)
    {
	lexertl::lookup(_sm, results);
	if(results.id == eVARWIDTH)
	{
	    int width = std::stoi(results.str());
	    if(width > 64)
	    {
	        throw std::string("Erreur : La nappe de fils " + name + " a une longueur supérieure à 64");
            }
	    else if(width < 0)
	    {
	        throw std::string("Erreur : La nappe de fils " + name + " ne peut pas être de longueur négative");
            }
	    else if(width == 1)
	    {
		Var* temp = new Var(name,1);
		_values_map.insert(make_pair(name,temp));
		_p->_vars.push_back(temp);
	    }
	    else
	    {
                Var* temp = new Var(name,width);
	        _values_map.insert(make_pair(name,temp));
	        _p->_vars.push_back(temp);
	    }
        }
	else
	{
	    throw std::string("Erreur : Un nombre de fils est attendu après la déclaration de la variable " + name);
        }
    }
}

void Parser::parse_expr(std::string const &name_result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    Var* result = get_var(name_result);

    lexertl::lookup(_sm, results);

    if(results.id == eOP)
    {
	std::string temp(results.str());
	if(temp == "OR")
	{
	    parse_binary_expr(result,ExpressionBinary::OR,results);
	}
	else if(temp == "AND")
	{
	    parse_binary_expr(result,ExpressionBinary::AND,results);
	}
	else if(temp == "XOR")
	{
	    parse_binary_expr(result,ExpressionBinary::XOR,results);
	}
	else if(temp == "NAND")
	{
	    parse_binary_expr(result,ExpressionBinary::NAND,results);
	}
	else if(temp == "MUX")
	{
	    parse_mux_expr(result,results);
	}
	else if(temp == "CONCAT")
	{
	    parse_binary_expr(result,ExpressionBinary::CONCAT,results);
	}
	else if(temp == "NOT")
	{
	    parse_not_expr(result,results);
	}
	else if(temp == "SELECT")
	{
	    parse_select_expr(result,results);
	}
	else if(temp == "SLICE")
	{
	    parse_slice_expr(result,results);
	}
	else if(temp == "REG")
	{
	    parse_reg_expr(result,results);
	}
	else if(temp == "ROM")
	{
	    parse_rom_expr(result,results);
	}
	else if(temp == "RAM")
	{
	    parse_ram_expr(result,results);
	}
	else
	{
	    throw std::string("Erreur : " + temp + " n'est pas un nom d'opérateur existant.");
	}
    }
    else if(results.id == eEQRVAR)
    {
	_p->_expressions.push_back(new ExpressionUnary(result,get_var(results.str()), true));
    }
    else if(results.id == eEQRCONSTVAR)
    {
	_p->_expressions.push_back(new ExpressionUnary(result,get_var_const(results.str()), true));
    }
    else
    {
        throw std::string("Erreur inconnue lors du parsing du fichier.");
    }
}

void Parser::parse_not_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm,results);
    Var* var;
    if(results.id == eEQRVAR)
    {
	std::string name = results.str();
	var = get_var(name);
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var = get_var_const(results.str());
    }
    else
    {
        throw std::string("Erreur : Un nom de variable ou une constante est attendue après l'opération NOT.");
    }
    _p->_expressions.push_back(new ExpressionUnary(result,var,false));
}



void Parser::parse_binary_expr(Var *result, int operator_type,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm, results);
    Var* var1;
    Var* var2;
    
    if(results.id == eEQRVAR)
    {	
        std::string name = results.str();
	var1 = get_var(name);
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var1 = get_var_const(results.str());
    }
    else
    {
        throw std::string("Erreur : Les fonctions OR/AND/XOR/NAND/CONCAT attendent une variable ou une constante en premier et second argument");
    }

    lexertl::lookup(_sm, results);
    if(results.id == eEQRVAR)
    {
        std::string name = results.str();
	var2 = get_var(name);
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var2 = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Les fonctions OR/AND/XOR/NAND/CONCAT attendent une variable ou une constante en premier et second argument");
    }
    _p->_expressions.push_back(new ExpressionBinary(result,var1,var2,static_cast<ExpressionBinary::Operation>(operator_type)));
}

void Parser::parse_mux_expr(Var *result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm, results);
    Var* varc;
    Var* var1;
    Var* var2;
    
    if(results.id == eEQRVAR)
    {	
	varc = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	varc = get_var_const(results.str());
    }
    else
    {
        throw std::string("Erreur : Les paramètres de la fonctions mux sont des constantes écrites sous formes binaires ou des variables");
    }

    lexertl::lookup(_sm, results);
    if(results.id == eEQRVAR)
    {
	var1 = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var1 = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Les paramètres de la fonctions mux sont des constantes écrites sous formes binaires ou des variables");
    }

    lexertl::lookup(_sm, results);
    if(results.id == eEQRVAR)
    {
	var2 = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var2 = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Les paramètres de la fonctions mux sont des constantes écrites sous formes binaires ou des variables");
    }
    
    _p->_expressions.push_back(new ExpressionMux(result,varc,var1,var2));
}

void Parser::parse_select_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm, results);
    int select_indice;
    Var* var;

    if(results.id == eEQRCONST)
    {
	select_indice = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le premier paramètre de la fontion SELECT doit être une constante");
    }

    lexertl::lookup(_sm, results);

    if(results.id == eEQRVAR)
    {
	var = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le second paramètre de la fonction SELECT doit être une variable ou une constante écrite sous forme binaire");
    }

    _p->_expressions.push_back(new ExpressionSelect(result, select_indice, var));
}

void Parser::parse_slice_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm, results);
    int slice_begin;
    int slice_end;
    Var* var;

    if(results.id == eEQRCONST)
    {
	slice_begin = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le premier paramètre de la fontion SLICE doit être une constante");
    }
    
    lexertl::lookup(_sm, results);

    if(results.id == eEQRCONST)
    {
	slice_end = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le second paramètre de la fontion SLICE doit être une constante");
    }
    
    lexertl::lookup(_sm, results);

    if(results.id == eEQRVAR)
    {
	std::string name = results.str();
	var = get_var(name);
    }
    else if(results.id == eEQRCONSTVAR)
    {
	var = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le troisième paramètre de la fontion SLICE doit être une variable ou une constante écrite en binaire");
    }
    if(slice_end == slice_begin)
    {
	_p->_expressions.push_back(new ExpressionSelect(result, slice_begin, var));
    }
    _p->_expressions.push_back(new ExpressionSlice(result, slice_begin, slice_end, var));
}

void Parser::parse_reg_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm,results);
    Var* var;
    if(results.id == eEQRVAR)
    {
	var = get_var(results.str());
    }
    else
    {
        throw std::string("Erreur : La fonction REG attend une variable en argument");
    }
    _p->_expressions.push_back(new ExpressionReg(result,var));
}

void Parser::parse_rom_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm,results);
    int addr_size,word_size;
    Var* read_addr;
    if(results.id == eEQRCONST)
    {
	addr_size = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le premier paramètre de la fontion ROM doit être une constante");
    }
    lexertl::lookup(_sm,results);
    if(results.id == eEQRCONST)
    {
	word_size = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le second paramètre de la fontion ROM doit être une constante");
    }
    lexertl::lookup(_sm,results);
    if(results.id == eEQRVAR)
    {
	read_addr = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	read_addr = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le troisième paramètre de la fontion ROM doit être une variable ou une constante écrite en binaire");
    }
    _p->_expressions.push_back(new ExpressionRom(result, addr_size, word_size, read_addr));
}

void Parser::parse_ram_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results)
{
    lexertl::lookup(_sm,results);
    int addr_size,word_size;
    Var* read_addr;
    Var* write_enable;
    Var* write_addr;
    Var* data;
    if(results.id == eEQRCONST)
    {
	addr_size = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le premier paramètre de la fontion RAM doit être une constante");
    }
    lexertl::lookup(_sm,results);
    if(results.id == eEQRCONST)
    {
	word_size = std::stoi(results.str());
    }
    else
    {
	throw std::string("Erreur : Le second paramètre de la fontion RAM doit être une constante");
    }
    lexertl::lookup(_sm,results);
    if(results.id == eEQRVAR)
    {
	read_addr = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	read_addr = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le troisième paramètre de la fontion RAM doit être une variable ou une constante écrite sous forme binaire");
    }

    lexertl::lookup(_sm,results);
    if(results.id == eEQRVAR)
    {
        write_enable = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	write_enable = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le quatrième paramètre de la fontion RAM doit être une variable ou une constante écrite sous forme binaire");
    }
    lexertl::lookup(_sm,results);
    if(results.id == eEQRVAR)
    {
        write_addr = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	write_addr = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le cinquième paramètre de la fontion RAM doit être une variable ou une constante écrite sous forme binaire");
    }
        lexertl::lookup(_sm,results);
    if(results.id == eEQRVAR)
    {
        data = get_var(results.str());
    }
    else if(results.id == eEQRCONSTVAR)
    {
	data = get_var_const(results.str());
    }
    else
    {
	throw std::string("Erreur : Le sixième paramètre de la fontion RAM doit être une variable ou une constante écrite sous forme binaire");
    }
    _p->_expressions.push_back(new ExpressionRam(result, addr_size, word_size, read_addr, write_enable, write_addr, data));
}
