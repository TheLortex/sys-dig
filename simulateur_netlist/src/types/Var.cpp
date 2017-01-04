#include "Var.hpp"

Var::Var(std::string const &name, int size, bool has_value)
    :_name(name), _size(size), _has_value(has_value), _id(0)
{}

Var::Var(std::string const &name, int size, bool has_value, int id)
    :_name(name), _size(size), _has_value(has_value),_id(id)
{}

Var::~Var()
{}

std::string Var::get_name() const
{
    return _name;
}

std::string Var::get_cpp_name() const
{
    return _name+"_var";
}

int Var::get_id() const
{
    return _id;
}

void Var::set_id(unsigned int id)
{
    _id = id;
}

int Var::get_size() const
{
    return _size;
}

bool Var::has_value() const
{
    return _has_value;
}

void Var::set_has_value(bool has_value)
{
    _has_value = has_value;
}
