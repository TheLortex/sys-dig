#ifndef EXPRESSIONSLICE_HPP
#define EXPRESSIONSLICE_HPP

#include "Expression.hpp"

class ExpressionSlice: public Expression
{
public:
    ExpressionSlice(Var *r, int begin, int end, Var *v);
    virtual ~ExpressionSlice();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
private:
    int _begin;
    int _end;
    Var *_var;
};

#endif
