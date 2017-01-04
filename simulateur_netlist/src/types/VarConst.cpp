#include "VarConst.hpp"
#include <iostream>

VarConst::VarConst(std::string const &value)
    :Var(value,value.size(),true)
{}

VarConst::~VarConst()
{}

int VarConst::get_size() const
{
    return 1;
}

std::string VarConst::get_cpp_name() const
{
    return std::to_string(std::stoll(get_name(), nullptr,2));
}
