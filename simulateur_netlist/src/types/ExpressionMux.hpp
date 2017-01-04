#ifndef EXPRESSIONMUX_HPP
#define EXPRESSIONMUX_HPP

#include "Expression.hpp"

class ExpressionMux: public Expression
{
public:
    ExpressionMux(Var *r,Var *vc, Var *v1, Var *v2);
    virtual ~ExpressionMux();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
private:
    Var *_varc;
    Var *_var1;
    Var *_var2;
};

#endif
