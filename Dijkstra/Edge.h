#pragma once
#include "Node.h"

class Node;
class Edge {
public:
    Node *node1, *node2;
    double weight;

    Edge(Node *w1, Node *w2, double weight);
    Edge();

    friend bool operator==(const Edge& k1, const Edge& k2);
};