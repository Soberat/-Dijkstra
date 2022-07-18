#include "Edge.h"
#include "Node.h"
#include <limits>

Edge::Edge(Node* w1, Node* w2, double weight) {
        this->node1 = w1;
        this->node2 = w2;
        this->weight = weight;
}

Edge::Edge() {
    this->node1 = nullptr;
    this->node2 = nullptr;
    this->weight = DBL_MAX;
}

bool operator==(const Edge& e1, const Edge& e2) {
    return (e1.weight == e2.weight) && ((e1.node1->name == e2.node1->name && e1.node2->name == e2.node2->name) ||
        (e1.node1->name == e2.node2->name && e1.node2->name == e2.node1->name));
}