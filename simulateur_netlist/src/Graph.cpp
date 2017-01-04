#include "Graph.hpp"
#include <algorithm>

#include <iostream>

Graph::Graph()
{}

Graph::Graph(unsigned int i)
{
    _data = std::vector<std::forward_list<int> >(i, std::forward_list<int>());
}

void Graph::add_node()
{
    _data.emplace_back(std::forward_list<int>());
}

void Graph::add_edge(unsigned int i, unsigned int j)
{
    if(i >= _data.size() || j >= _data.size())
    {
	throw std::string("Tried to add edges to nodes that do not exists");
    }
    if(find(_data[i].begin(), _data[i].end(), j) == _data[i].end())
    {
	_data[i].push_front(j);
    }
}

bool Graph::has_cycle() const
{
    std::vector<mark> marks = std::vector<mark>(_data.size(),eNotVisited);
    for(unsigned int i = 0; i<_data.size(); i++)
    {
	if(dfs_cycle(marks,i))
	{
	    return true;
	}
    }
    return false;
}

bool Graph::dfs_cycle(std::vector<mark> &marks, int elem) const
{
    if(marks[elem] == eVisited)
    {
	return false;
    }
    if(marks[elem] == eInProgress)
    {
	return true;
    }
    marks[elem] = eInProgress;
    for(int child: _data[elem])
    {
	if(dfs_cycle(marks,child))
	{
	    return true;
	}
    }
    marks[elem] = eVisited;
    return false;
}

std::vector<int> Graph::topological_sort() const
{
    if(has_cycle())
    {
	throw std::string("Can't sort topolically a graph that has cycles !");
    }
    std::vector<int> sort;
    sort.reserve(_data.size());
    std::vector<mark> marks = std::vector<mark>(_data.size(), eNotVisited);
    for(unsigned int i = 0; i<_data.size(); i++)
    {
	dfs_topological_sort(sort,marks,i);
    }
    return sort;
}

void Graph::dfs_topological_sort(std::vector<int> &sort, std::vector<mark> &marks, int elem) const
{
    if(marks[elem] == eVisited)
    {
	return;
    }
    for(int child: _data[elem])
    {
	dfs_topological_sort(sort, marks, child);
    }
    sort.push_back(elem);
    marks[elem] = eVisited;
}
