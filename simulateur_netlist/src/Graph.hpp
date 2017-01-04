#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>
#include <forward_list>

class Graph
{
public:
    //Créer un graphe vide
    Graph();

    //Créer un graphe possédant i noeuds
    Graph(unsigned int i);

    //Ajoute un noeud au graphe
    void add_node();

    //Relie le point i et le point j
    void add_edge(unsigned int i, unsigned int j);

    //Renvoie true si le graphe possède un cycle
    bool has_cycle() const;

    //Trie topologiquement le graphe
    std::vector<int> topological_sort() const;
private:
    //Des marques pour permettre le calcul du tri topologique et de l'existence de noeuds
    enum mark {eNotVisited, eInProgress, eVisited};

    //Effectue un parcours en profondeur sur le graphe récursivement pour le calcul de l'existence de cycles
    bool dfs_cycle(std::vector<mark> &marks, int elem) const;

    //Effectue un parcours en profondeur sur le graphe récursivement pour le calcul du tri topologique
    void dfs_topological_sort(std::vector<int> &sort, std::vector<mark> &marks, int elem) const;

    //Contient les noeuds du graphe ainsi que les arêtes orientés, sous forme de liste d'adjacence
    std::vector<std::forward_list<int> > _data;
};

#endif
