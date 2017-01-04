#include "ExpressionMux.hpp"
#include "Var.hpp"
#include <sstream>

ExpressionMux::ExpressionMux(Var *r, Var *vc,Var *v1, Var *v2)
    :Expression(r), _varc(vc), _var1(v1), _var2(v2)
{
    if(v1->get_size() != v2->get_size())
    {
	throw std::string(v1->get_name() + " doit être de la même taille que " + v2->get_name() + " dans la fonction :\n" + get_expression());
    }
    if(v1->get_size() != r->get_size())
    {
	throw std::string(r->get_name() + " doit être de la même taille que " + v1->get_name() + " dans la fonction :\n" + get_expression());
    }
    if(vc->get_size() != 1)
    {
	throw std::string(vc->get_name() + " ne doit pas être une nappe de fils dans la fonction :\n" + get_expression());
    }
}

ExpressionMux::~ExpressionMux()
{}

std::string ExpressionMux::get_expression() const
{
    return _result->get_name() + " = MUX " + _varc->get_name() + " " + _var1->get_name() + " " + _var2->get_name();
}

std::vector<Var*> ExpressionMux::get_dependencies() const
{
    std::vector<Var*> dependencies;
    if(!_var1->has_value())
    {
	dependencies.push_back(_var1);
    }
    if(!_var2->has_value())
    {
	dependencies.push_back(_var2);
    }
    if(!_varc->has_value())
    {
	dependencies.push_back(_varc);
    }
    return dependencies;
}

std::string ExpressionMux::get_instructions_mid() const
{
    return _result->get_cpp_name() + " = " + _varc->get_cpp_name() + " ? " + _var1->get_cpp_name() + " : " + _var2->get_cpp_name() + ";\n";
}
