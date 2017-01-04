#include "ExpressionRom.hpp"
#include "Var.hpp"
#include <sstream>

std::unordered_map<int,int> ExpressionRom::_roms_size;

ExpressionRom::ExpressionRom(Var *r, int addr_size, int word_size, Var *read_addr)
    :Expression(r), _addr_size(addr_size), _word_size(word_size), _read_addr(read_addr)
{
    if(word_size != r->get_size())
    {
	throw std::string("Le deuxième paramètre doit être de la taille de la sortie dans la fonction \n" + get_expression());
    }
    if(addr_size != read_addr->get_size() )
    {
	throw std::string("Le troisième paramètre doit avoir la taille égale au second paramètre dans la fontion \n" + get_expression());
    }

    std::unordered_map<int,int>::iterator it = _roms_size.find(addr_size);
    if(it == _roms_size.end())
    {
	_roms_size.insert(std::make_pair(addr_size, word_size));
    }
    else if(it->second < word_size)
    {
	it->second = word_size;
    }
}

ExpressionRom::~ExpressionRom()
{}

std::string ExpressionRom::get_expression() const
{
    std::ostringstream oss;
    oss << _result->get_name() << " = ROM " << _addr_size << " " << _word_size << " " << _read_addr->get_name();
    return oss.str();
}

std::vector<Var*> ExpressionRom::get_dependencies() const
{
    std::vector<Var*> dependencies;
    if(!_read_addr->has_value())
    {
	dependencies.push_back(_read_addr);
    }

    return dependencies;
}

std::string ExpressionRom::get_instructions_mid() const
{
    return _result->get_cpp_name() + " = rom" + std::to_string(_addr_size) + "[" + _read_addr->get_cpp_name() + "% (1 << " + std::to_string(_addr_size) + ")];";
}

std::unordered_map<int,int> ExpressionRom::get_roms_size()
{
    return _roms_size;
}
