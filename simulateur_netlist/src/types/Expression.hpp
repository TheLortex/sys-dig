#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <vector>

class Var;

class Expression
{
public:
    Expression(Var *result);
    virtual ~Expression();

    //Renvoie un pointeur sur le résultat de l'opération
    Var* get_result();

    //Renvoie l'expression correspondante sous forme de fonction netlist (AND x y par exemple).
    //Sert uniquement pour le debug
    virtual std::string get_expression() const = 0;

    //Renvoie les variables dont le calcul est nécessaire pour le calcul de _result
    virtual std::vector<Var*> get_dependencies() const = 0;

    //Renvoie les instructions cpp pour effectuer l'opération correspondante
    virtual std::string get_instructions_beginning() const;
    virtual std::string get_instructions_mid() const;
    virtual std::string get_instructions_end() const;
    
protected:
    //Pointeur sur la variable recevant le résultat de l'opération
    Var *_result;
};

#endif
