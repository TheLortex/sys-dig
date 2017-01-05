#include "ExpressionSlice.hpp"
#include "Var.hpp"
#include <sstream>

ExpressionSlice::ExpressionSlice(Var *r, int begin, int end, Var *v)
    :Expression(r), _begin(begin), _end(end), _var(v)
{
    if(begin < 0 || end < 0 || end - begin < 0)
    {
	throw std::string("The parameters of the SLICE instruction must be nonnegative and shall be in increasing order in instruction\n" + get_expression());
    }
    if(end >= v->get_size())
    {
	throw std::string("Can't slice the wires because the bounds are out of range in expression " + get_expression());
    }
    if(r->get_size() != end-begin+1)
    {
	throw std::string("The result of the slice has not the correct dimensions in the expression" + get_expression());
    }
}

ExpressionSlice::~ExpressionSlice()
{}

std::string ExpressionSlice::get_expression() const
{
    std::ostringstream oss;
    oss <<  _result->get_name() << " = SLICE " << _begin << " " << _end << " " << _var->get_name();
    return oss.str();
}

std::vector<Var*> ExpressionSlice::get_dependencies() const
{
    std::vector<Var*> dependencies;
    if(!_var->has_value())
    {
	dependencies.push_back(_var);
    }
    return dependencies;
}

std::string ExpressionSlice::get_instructions_mid() const
{
    if(_end == _var->get_size()-1)
    {
        return _result->get_cpp_name() + " = " + _var->get_cpp_name() + ";";
    }

    return _result->get_cpp_name() + " = (" + _var->get_cpp_name() + " >> " + std::to_string(_var->get_size() - _begin -1) + ");";
}
