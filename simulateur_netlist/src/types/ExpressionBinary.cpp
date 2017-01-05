#include "ExpressionBinary.hpp"
#include "Var.hpp"
#include <sstream>

ExpressionBinary::ExpressionBinary(Var *r, Var *v1, Var *v2, Operation op)
    :Expression(r), _var1(v1), _var2(v2), _op(op)
{
    if(_op != CONCAT)
    {
	if(v1->get_size() != v2->get_size())
	{
	    throw std::string(v1->get_name() + " doit être de la même taille que " + v2->get_name() + " dans la fonction :\n" + get_expression());
	}
	if(v1-> get_size() != r->get_size())
	{
	    throw std::string(r->get_name() + " doit être de la même taille que " + v1->get_name() + " dans la fonction :\n" + get_expression());
	}
    }
    else if(r->get_size() != v1->get_size() + v2->get_size())
    {
	throw std::string(r->get_name() + " doit être de la même taille que la somme de la taille de " + v1->get_name() + " et de " + v2->get_name() + " dans la fonction :\n" + get_expression());
    }
}

ExpressionBinary::~ExpressionBinary()
{}

std::string ExpressionBinary::get_expression() const
{
    std::ostringstream oss;
    std::string name_operation;
    switch(_op)
    {
    case OR:
        name_operation = "OR";
        break;
    case AND:
        name_operation = "AND";
        break;
    case NAND:
        name_operation = "NAND";
        break;
    case XOR:
        name_operation = "XOR";
        break;
    case CONCAT:
	name_operation = "CONCAT";
	break;
    default:
        break;
    }
    oss << _result->get_name() << " = " << name_operation << " " << _var1->get_name() << " " << _var2->get_name();
    return oss.str();
}

std::vector<Var*> ExpressionBinary::get_dependencies() const
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
    return dependencies;
}

std::string ExpressionBinary::get_instructions_mid() const
{
    switch(_op)
    {
    case OR:
        return _result->get_cpp_name() + " = " + _var1->get_cpp_name() + " | " + _var2->get_cpp_name() + ";";
	break;
	
    case AND:
	return _result->get_cpp_name() + " = " + _var1->get_cpp_name() + " & " + _var2->get_cpp_name() + ";";
	break;
	
    case XOR:
        return _result->get_cpp_name() + " = " + _var1->get_cpp_name() + " ^ " + _var2->get_cpp_name() + ";";
	break;
	
    case NAND:
	if(_result->get_size() != 1)
	{
	    return _result->get_cpp_name() + " = " + "~(" + _var1->get_cpp_name() + " & " + _var2->get_cpp_name() + ");";
	}
	return _result->get_cpp_name() + " = " + "!(" + _var1->get_cpp_name() + " && " + _var2->get_cpp_name() + ");";
	break;
	
    case CONCAT:
	return _result->get_cpp_name() + " = " + _var1->get_cpp_name() + ";\n"
	    + _result->get_cpp_name() + " = " + _result->get_cpp_name() + " << " + std::to_string(_var2->get_size()) + ";\n"
	    + _result->get_cpp_name() + " |= " + _var2->get_cpp_name() + " % " + std::to_string(1 << _var2->get_size()) + ";";
	break;
	
    default:
	return "";
	break;
    }
}
