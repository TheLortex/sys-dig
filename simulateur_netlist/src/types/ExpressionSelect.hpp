#ifndef EXPRESSIONSELECT_HPP
#define EXPRESSIONSELECT_HPP

#include "Expression.hpp"

class ExpressionSelect: public Expression
{
public:
    ExpressionSelect(Var *r, int indice, Var *v);
    virtual ~ExpressionSelect();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
private:
    int _indice;
    Var *_var;
};

#endif

 
