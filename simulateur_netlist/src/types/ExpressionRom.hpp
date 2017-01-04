#ifndef EXPRESSIONROM_HPP
#define EXPRESSIONROM_HPP

#include "Expression.hpp"
#include <unordered_map>

class ExpressionRom: public Expression
{
public:
    ExpressionRom(Var *r, int addr_size, int word_size, Var *read_addr);
    virtual ~ExpressionRom();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;

    static std::unordered_map<int,int> get_roms_size();
private:
    int _addr_size;
    int _word_size;
    Var *_read_addr;

    static std::unordered_map<int,int> _roms_size;
};

#endif
