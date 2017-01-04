#include "Expression.hpp"
#include "Var.hpp"

/*Expression::name_memory = "memory";
Expression::name_ram = "ram";
Expression::name_rom = "rom";*/

Expression::Expression(Var *result)
    :_result(result)
{
    if(_result->has_value())
    {
	throw std::string("Can't affect the result of an operation to a variable that already has a value");
    }
}

Expression::~Expression()
{}

std::string Expression::get_instructions_beginning() const
{
    throw std::string("Pas d'instructions à écrire.");
}

std::string Expression::get_instructions_mid() const
{
    throw std::string("Pas d'instructions à écrire.");
}

std::string Expression::get_instructions_end() const
{
    throw std::string("Pas d'instructions à écrire.");
}


Var* Expression::get_result()
{
    return _result;
}
