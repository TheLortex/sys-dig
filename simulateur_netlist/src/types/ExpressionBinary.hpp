#ifndef EXPRESSIONBINARY_HPP
#define EXPRESSIONBINARY_HPP

#include "Expression.hpp"

class ExpressionBinary: public Expression
{
public:
    //contient les odifférents opérations
    enum Operation {OR, AND, XOR, NAND, CONCAT};
    ExpressionBinary(Var *r,Var *v1, Var *v2, Operation op);
    virtual ~ExpressionBinary();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
private:
    Var *_var1; //représente le premier paramètre
    Var *_var2; //représente le second paramètre
    Operation _op; //Représente l'opération
};

#endif
