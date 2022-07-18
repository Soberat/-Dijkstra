#include <algorithm>
#include <iostream>
#include <set>
#include "Dijkstra.h"

double sumAllPaths(std::vector<std::pair<Node*, double>> result) {
    double sum = 0.0;
    double* ptr = &sum;

    std::for_each(result.begin(), result.end(), [ptr](std::pair<Node*, double> pair) {
        *ptr += pair.second;
        });

    return sum;
}

std::pair<std::vector<std::pair<Node*, double>>, std::vector<Edge>> dijkstra(std::vector<Edge> edges, std::vector<Node*> nodes, Node* source, Node* target) {
    std::vector<std::pair<Node*, double>> distances;
    std::vector<Edge> previous;
    std::vector<Node*> queue;

    // Set all node distances to inf and source to 0
    for (Node* w : nodes) {
        std::pair<Node*, double> distancePair;
        Edge pairPrevious;
        if (w == source) {
            // Source path to itself has cost 0 
            distancePair = { w, 0 };
            pairPrevious = { w, w, 0 };
        } else {
            // All other nodes have infinite cost and unknown path via nullptr
            distancePair = { w, std::numeric_limits<double>::infinity() };
            pairPrevious = { w, nullptr, std::numeric_limits<double>::infinity() };
        }

        distances.push_back(distancePair);
        previous.push_back(pairPrevious);
        queue.push_back(w);
    }

    while (queue.size() > 0) {
        Node* node = nullptr;

        // Find the node with shortest distance, which hasn't been visited yet
        std::sort(distances.begin(), distances.end(), [](const std::pair<Node*, double> p1, std::pair<Node*, double> p2) {
            return p1.second < p2.second;
            });

        for (auto i = distances.begin(); i != distances.end(); ++i) {
            if (std::find(queue.begin(), queue.end(), i->first) != queue.end()) {
                node = i->first;
                break;
            }
        }

        // Algorithm can stop execution here if we only care about the path to the target
        if (target != nullptr && node == target) return std::pair<std::vector<std::pair<Node*, double>>, std::vector<Edge>> {distances, previous};

        // Remove node from queue
        auto it = std::remove(queue.begin(), queue.end(), node);
        if (it != queue.end()) queue.erase(it);

        // Create a list of neighboring nodes, which haven't been visited yet
        std::set<Node*> neighbors ;
        for (Edge e : edges) {
            // If not in neighbor list yet
            if (std::find(neighbors.begin(), neighbors.end(), node) == neighbors.end()) {
                // If current node is part of an Edge, then the second node will be a neighbor 
                if (e.node1 == node) {
                    // If neighbor hasn't been visited yet
                    if (std::find(queue.begin(), queue.end(), e.node2) != queue.end()) neighbors.insert(e.node2);
                } else if (e.node2 == node) {
                    if (std::find(queue.begin(), queue.end(), e.node1) != queue.end()) neighbors.insert(e.node1);
                }
            }
        }

        double nodeDistance = (std::find_if(distances.begin(), distances.end(), [node](const std::pair<Node*, double> p) {
            return p.first == node;
            }))->second;

        for (Node* neighbor : neighbors) {
            // Add the edge weight of neighbor to its' distances¹siada
            double edgeWeight = nodeDistance + std::find_if(edges.begin(), edges.end(), [node, neighbor](const Edge& k) {
                return (k.node1 == node && k.node2 == neighbor) || (k.node1 == neighbor && k.node2 == node);
                })->weight;

            // Find previously known distance to this neighbor
            double distanceNeighbor = (std::find_if(distances.begin(), distances.end(), [neighbor](const std::pair<Node*, double> p) {
                return p.first == neighbor;
                }))->second;

            // If new path is shorter than the known path, assign the new information about distance and path to this node
            if (edgeWeight < distanceNeighbor) {
                std::find_if(distances.begin(), distances.end(), [neighbor](const std::pair<Node*, double> p) {
                    return p.first == neighbor;
                    })->second = edgeWeight;
                std::find_if(previous.begin(), previous.end(), [neighbor](const Edge e) {
                    return e.node1 == neighbor;
                    })->node2 = node;
            }
        }
    }
    return std::pair<std::vector<std::pair<Node*, double>>, std::vector<Edge>> {distances, previous};
}