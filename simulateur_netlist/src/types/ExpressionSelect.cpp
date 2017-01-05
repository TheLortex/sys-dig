#include "ExpressionSelect.hpp"
#include "Var.hpp"
#include <sstream>

ExpressionSelect::ExpressionSelect(Var *r, int indice, Var *v)
    :Expression(r), _indice(indice), _var(v)
{
    if(r->get_size() != 1)
    {
	throw std::string(r->get_name() + " must be a single wire because of the instruction \n" + get_expression());
    }
    if(indice >= _var->get_size())
    {
	throw std::string("Element selected in " + v->get_name() + " is out of bound in the instruction \n" + get_expression()); 
    }
}

ExpressionSelect::~ExpressionSelect()
{}

std::string ExpressionSelect::get_expression() const
{
    std::ostringstream oss;
    oss <<  _result->get_name() << " = SELECT " <<  _indice << " " << _var->get_name();
    return oss.str();
}

std::vector<Var*> ExpressionSelect::get_dependencies() const
{
    std::vector<Var*> dependencies;
    if(!_var->has_value())
    {
	dependencies.push_back(_var);
    }
    return dependencies;
}

std::string ExpressionSelect::get_instructions_mid() const
{
    if(_var->get_size() == _indice+1)
    {
        return _result->get_cpp_name() + " = " + _var->get_cpp_name() + " % 2;";
    }
    return _result->get_cpp_name() + " = (" + _var->get_cpp_name() + " >> " + std::to_string(_var->get_size() - _indice -1) + ") % 2;";
}
