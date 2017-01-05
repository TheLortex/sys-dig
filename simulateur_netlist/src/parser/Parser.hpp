#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexertl/stream_shared_iterator.hpp"
#include "lexertl/generator.hpp"
#include "lexertl/lookup.hpp"
#include <vector>
#include <unordered_map>
#include <set>
#include <string>

class Var;
class Expression;
class Program;

class Parser
{
public:
    Parser();

    //parse un fichier contenant une netlsit
    void parse(std::string const &file_name, Program* program);

private:
    //initialise l'automate qui va reconnaitre la netlist
    void init_sm();

    //renvoie un pointeur sur la variable name
    Var* get_var(std::string const &name);

    //renvoie un pointeur sur la variable possédant la valeur name (en binaire)
    Var* get_var_const(std::string const &name);

    //Ces fonctions parse les fonctions de la netlist. Elles sont séparées pour avoir plus de lisibilité
    void parse_var(std::string const &name,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_expr(std::string const &name_result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_binary_expr(Var* result,int operator_type,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_mux_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_not_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_select_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_slice_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_reg_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_ram_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);
    void parse_rom_expr(Var* result,lexertl::match_results<lexertl::stream_shared_iterator> &results);

    //Les tokens
    enum {eEOI=0, eBLOCSEPARATOR, eIN, eOUT, eVARSEPARATOR, eVAR, eVARISMULTIPLE, eVARWIDTH, eEQLVAR, eOP, eEQRVAR, eEQRCONST, eEQRCONSTVAR};

    //les règles de l'automate ainsi que l'automate
    lexertl::rules _rules;
    lexertl::state_machine _sm;

    //Un pointeur sur le programme, qui va contenir toutes les données parsées
    Program* _p;

    //une map liant le nom des variables à un pointeur sur leur objet
    std::unordered_map<std::string, Var*> _values_map;

    //Un set contenant les noms de variables exclus (comme VAR, OR, etc..)
    static std::set<std::string> reserved_names;
};

#endif
