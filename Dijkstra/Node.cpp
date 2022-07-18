#include "Node.h"
#include "Edge.h"

Node::Node(std::string name) {
    this->name = name;
}

Node::Node() {
    this->name = "default";
}

bool operator==(const Node& w1, const Node& w2) {
    return w1.name == w2.name;
};
