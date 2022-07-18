#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <limits>
#include <tuple>
#include "DotFileReader.h"
#include "Dijkstra.h"


/*
 * @brief Reading input file and calculating "which edge from a graph should you remove, such that the sum of distances will be the smallest"
 * 
 * @param argv[1] - path to file in simplified DOT format
 */
int main(int argc, char* argv[]) {

    if (argc == 1) {
        std::cerr << "Usage: " << argv[0] << " <path to file in .gv format>\n";
        exit(0);
    }

    // Read file in simplified DOT format
    auto graph = readDotFile(argv[1]);

    std::vector<Node*> nodes = std::get<0>(graph);
    std::vector<Edge> edges = std::get<1>(graph);
    Node* source = std::get<2>(graph);

    std::cout << "Source is " << source->name << std::endl;

    // For each edge we track what is the sum of the distances to the source after removing this edge
    std::vector<std::pair<Edge, double>> sumForEdge;

    // Iterate over edges and remove it from the graph (as it is being used as a runway)
    for (const Edge& e : edges) {
        std::vector<Edge> edgesCopy = edges;

        // Create new graph without 'e' Edge
        edgesCopy.erase(std::remove_if(edgesCopy.begin(), edgesCopy.end(), [e](const Edge& k2) {
            return e == k2;
        }));

        // Execute the Dijkstra algorithm on the new graph
        std::pair<std::vector<std::pair<Node*, double>>, std::vector<Edge>> result = dijkstra(edgesCopy, nodes, source);

        // Display information about path costs to each node
        std::cout << "Result (" << e.node1->name << " - " << e.weight << " - " << e.node2->name << " as runway):\n";
        for (std::pair<Node*, double> p : result.first) {
            std::cout << "\t" << p.first->name << ": " << p.second << "\n";
        }

        // Calculate sum of all paths. If it is 'inf', it means that the deleted edge was the only edge leading to one of the nodes
        double sum = sumAllPaths(result.first);
        if (sum == std::numeric_limits<double>::infinity()) {
            std::cout << "\tPath to one of more cities was not availablem, ignoring result\n";
        } else {
            std::cout << "\tTotal path costs: " << sum << "\n";
            // Save the result for comparison later on
            sumForEdge.push_back(std::pair<Edge, double> { e, sum });
        }   
    }

    if (sumForEdge.size() == 0) {
        std::cout << "From given graph no edge could be used as a runway." << std::endl;
    } else {
        // Sort the sum list, to find the one with smallest value
        std::sort(sumForEdge.begin(), sumForEdge.end(), [](const std::pair<Edge, double> p1, const std::pair<Edge, double> p2) {
            return p1.second < p2.second;
            });
        double minimumSum = sumForEdge[0].second;

        // Find all such edges, for which the sum is at minimum
        std::vector<std::pair<Edge, double>> minimumDistancePairs;
        std::copy_if(sumForEdge.begin(), sumForEdge.end(), std::back_inserter(minimumDistancePairs), [minimumSum](const std::pair<Edge, double> p) {
            return p.second == minimumSum;
            });

        std::cout << "Minimum sum of distances to sources is " << minimumSum << " and happens when using:\n";

        for (std::pair<Edge, double> pair : minimumDistancePairs) {
            std::cout << "  " << pair.first.node1->name << " - " << pair.first.weight << " - " << pair.first.node2->name << std::endl;
        }

        std::cout << "as a runway.";
    }
}