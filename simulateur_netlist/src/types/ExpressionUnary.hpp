#ifndef EXPRESSIONUNARY_HPP
#define EXPRESSIONUNARY_HPP

#include "Expression.hpp"

class ExpressionUnary: public Expression
{
public:
    ExpressionUnary(Var *r, Var *v, bool equality);
    virtual ~ExpressionUnary();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
private:
    Var *_var;
    bool _equality;
};

#endif
