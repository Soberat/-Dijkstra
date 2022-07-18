#pragma once
#include <vector>
#include <string>
#include <tuple>
#include "Node.h"
#include "Edge.h"

/*
 * @brief Conversion of graph from simplified DOT format to vectors of Node and Edge objects
 *
 * @param filePath - path to file in simplified DOT format
 *
 * @return tuple:
 *  @return<0> vector of Node pointers that were retrieved from the file
 *  @return<1> vector of Edge objects that were retrieved from the file
 *  @return<2> pointer to Node which was read as the source for the algorithm
 */
std::tuple<std::vector<Node*>, std::vector<Edge>, Node*> readDotFile(std::string filePath);