#pragma once
#include <vector>
#include "Node.h"
#include "Edge.h"

/*
 * @brief Sum of path costs from arguments
 *
 * @param result - result of Dijkstra's algorithm, containing Nodes and distances to them from the capital
 *
 * @return sum of all costs
 */
double sumAllPaths(std::vector<std::pair<Node*, double>> result);

/*
 * @brief Dijkstra's algorithm that finds the shortest paths to Nodes in a graph or a multigraph
 * Based on pseudocode from Wikipedia
 * @see https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
 *
 * @param edges - vector with Edge objects, which are in the graph
 * @param nodes - vector with Node objects, which are in the graph
 * @param source - pointer to Node (from vector in @param nodes), from which the algorithm starts 
 * @param target - optional pointer to a Node, if given, algorithm will stop execution
 *
 * @return pair:
 *  @return<0> vector of pairs Node-double, containing shortest distance to this node calculated by the algorithm
 *  @return<1> vector of Edge objects, marking the shortest path calculated by the algorithm
 */
std::pair<std::vector<std::pair<Node*, double>>, std::vector<Edge>> dijkstra(std::vector<Edge> edges, std::vector<Node*> nodes, Node* source, Node* target = nullptr);
