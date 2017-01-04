#ifndef VAR_HPP
#define VAR_HPP

#include <string>

class Var
{
public:
    Var(std::string const &name, int size = 1, bool has_value = false);
    Var(std::string const &name, int size, bool has_value, int id);
    virtual ~Var();

    //renvoie le nom de la varible telle que définie par la netlist
    std::string get_name() const;
    //renvoie le nom de la variable telle que définie dans le fichier cpp
    virtual std::string get_cpp_name() const;

    //Renvoie l'id de la variable (utile pour trier la netlist)
    int get_id() const;
    void set_id(unsigned int id);

    //Renvoie la taille de la variable (1 si la variable est un fil)
    int get_size() const;
    
    //Renvoie true si la variable est une constante, ou si elle est une valeur d'input (utile pour trier la netlist)
    bool has_value() const;
    void set_has_value(bool has_value);
    
private:

    std::string _name; //Nom de la variable
    int _size; //Taille de la variable
    bool _has_value; //Si la variable contient une valeur ou non
    int _id; //Donne un id a la variable
};

#endif
