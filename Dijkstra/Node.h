#pragma once
#include <vector>
#include <string>

class Edge;
class Node {
public:
	std::string name;

	Node(std::string name);
	Node();
	friend bool operator==(const Node& w1, const Node& w2);
};