#ifndef VARCONST_HPP
#define VARCONST_HPP

#include "Var.hpp"

class VarConst : public Var
{
public:
    VarConst(std::string const &value);
    virtual ~VarConst();
    virtual int get_size() const;
    virtual std::string get_cpp_name() const;
};

#endif
