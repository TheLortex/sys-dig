#include "ExpressionUnary.hpp"
#include "Var.hpp"
#include <sstream>

ExpressionUnary::ExpressionUnary(Var *r, Var *v, bool equality)
    :Expression(r), _var(v), _equality(equality)
{
    if(r->get_size() != v->get_size())
    {
	throw std::string(v->get_name() + " must have the same size than " + r->get_name() + " in the instruction :\n" + get_expression());
    }
}

ExpressionUnary::~ExpressionUnary()
{}

std::string ExpressionUnary::get_expression() const
{
    std::ostringstream oss;
    std::string op = _equality ? "" : "NOT ";
    oss << _result->get_name() << " = " << op << _var->get_name();
    return oss.str();
}

std::vector<Var*> ExpressionUnary::get_dependencies() const
{
    std::vector<Var*> dependencies;
    if(!_var->has_value())
    {
	dependencies.push_back(_var);
    }
    return dependencies;
}

std::string ExpressionUnary::get_instructions_mid() const
{
    if(_equality)
    {
	return _result->get_cpp_name() + " = " + _var->get_cpp_name() + ";";
    }
    else
    {
	return _result->get_cpp_name() + " = " + (_var->get_size() == 1 ? "!" : "~") + _var->get_cpp_name() + ";";
    }
}
