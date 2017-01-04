#ifndef EXPRESSIONREG_HPP
#define EXPRESSIONREG_HPP

#include "Expression.hpp"
#include <set>

class ExpressionReg: public Expression
{
public:
    ExpressionReg(Var *r, Var *v1);
    virtual ~ExpressionReg();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
    virtual std::string get_instructions_end() const;

    static std::set<Var*> get_registers();
    
private:
    const Var *_var;

    static std::set<Var*> _registers;
};

#endif
