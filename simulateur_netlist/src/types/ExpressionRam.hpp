#ifndef EXPRESSIONRAM_HPP
#define EXPRESSIONRAM_HPP

#include "Expression.hpp"
#include <unordered_map>

class ExpressionRam: public Expression
{
public:
    ExpressionRam(
	Var *r,
	int addr_size,
	int word_size,
	Var *read_addr,
	Var* write_enable,
	Var* write_addr,
	Var* data);
    virtual ~ExpressionRam();
    virtual std::string get_expression() const;
    virtual std::vector<Var*> get_dependencies() const;
    virtual std::string get_instructions_mid() const;
    virtual std::string get_instructions_end() const;

    static std::unordered_map<int,int> get_rams_size();
private:
    int _addr_size;
    int _word_size;
    Var *_read_addr;
    Var *_write_enable;
    Var *_write_addr;
    Var *_data;

    static std::unordered_map<int,int> _rams_size;
};

#endif
