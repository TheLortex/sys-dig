#include "ExpressionReg.hpp"
#include "Var.hpp"

std::set<Var*> ExpressionReg::_registers;

ExpressionReg::ExpressionReg(Var *r, Var *v)
    :Expression(r), _var(v)
{
    if(r->get_size() != v->get_size())
    {
	throw std::string(v->get_name() + " doit être de la même taille que " + r->get_name() + " dans la fonction :\n" + get_expression());
    }
    _registers.insert(v);
}

ExpressionReg::~ExpressionReg()
{}


std::string ExpressionReg::get_expression() const
{
    return _result->get_name() + " = REG " + _var->get_name();
}

std::vector<Var*> ExpressionReg::get_dependencies() const
{
    return std::vector<Var*>();
}

std::string ExpressionReg::get_instructions_mid() const
{
    return _result->get_cpp_name() + " = " + _var->get_name() + "_reg;";
}

std::string ExpressionReg::get_instructions_end() const
{
    return _var->get_name() + "_reg = " + _var->get_cpp_name() + ";";
}

std::set<Var*> ExpressionReg::get_registers()
{
    return _registers;
}
