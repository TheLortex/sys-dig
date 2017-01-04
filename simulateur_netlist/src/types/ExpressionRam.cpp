#include "ExpressionRam.hpp"
#include "Var.hpp"
#include <sstream>

std::unordered_map<int,int> ExpressionRam::_rams_size;

ExpressionRam::ExpressionRam(Var *r, int addr_size, int word_size, Var *read_addr, Var* write_enable, Var* write_addr, Var* data)
    :Expression(r), _addr_size(addr_size), _word_size(word_size), _read_addr(read_addr), _write_enable(write_enable), _write_addr(write_addr), _data(data)
{
    if(word_size != r->get_size() || word_size != data->get_size())
    {
	throw std::string("Le second paramètre doit être de la taille de la sortie et du sixième paramètre dans la fonction \n" + get_expression());
    }
    if(addr_size != read_addr->get_size() || addr_size != write_addr->get_size())
    {
	throw std::string("Le deuxième paramètre doit avoir la même taille que le troisième et le cinquième dans la fonction \n" + get_expression());
    }
    if(write_enable->get_size() != 1)
    {
	throw std::string("Le quatrième paramètre doit être un fil simple dans la fonction \n" + get_expression());
    }

    std::unordered_map<int,int>::iterator it = _rams_size.find(addr_size);
    if(it == _rams_size.end())
    {
	_rams_size.insert(std::make_pair(addr_size, word_size));
    }
    else if(it->second < word_size)
    {
	it->second = word_size;
    }
}

ExpressionRam::~ExpressionRam()
{}

std::string ExpressionRam::get_expression() const
{
    std::ostringstream oss;
    oss << _result->get_name() << " = RAM " << _addr_size << " " << _word_size << " " << _read_addr->get_name() << " " << _write_enable->get_name() << " " << _write_addr->get_name() << " " << _data->get_name();
	return oss.str();
}

std::vector<Var*> ExpressionRam::get_dependencies() const
{
    std::vector<Var*> dependencies;
    if(!_read_addr->has_value())
    {
	dependencies.push_back(_read_addr);
    }
    return dependencies;
}

std::string ExpressionRam::get_instructions_mid() const
{
    if(_addr_size <= 16)
    {
	return _result->get_cpp_name() + " = ram" + std::to_string(_addr_size) + "[" + _read_addr->get_cpp_name() + " % " + std::to_string(1 << _addr_size) + "];";
    }
    return "";
}

std::string ExpressionRam::get_instructions_end() const
{
    if(_addr_size <= 16)
    {
	return "if(" +_write_enable->get_cpp_name() + "){ram" + std::to_string(_addr_size) + "[" + _write_addr->get_cpp_name() + " % " + std::to_string(1 << _addr_size) + "] = " + _data->get_cpp_name()  + ";}";
    }
    return "";
}

std::unordered_map<int,int> ExpressionRam::get_rams_size()
{
    return _rams_size;
}
