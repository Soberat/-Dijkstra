#include "DotFileReader.h"
#include <iostream>
#include <fstream>

/* 
 * @brief processing of a statement in graph definition
 * 
 * @param statement - ';' terminated string, defining an Edge or source
 * @param nodes - pointer to vector of Node pointers, where a new Node pointer can be added
 * @param edges - pointer to vector of Edges, where a new Node pointer can be added
 * @param sourceName - pointer to string being the name of the source Node. Set at the very end, since at the time of defining the source the Node may not exist yet
 */
static void processStatement(const std::string statement, std::vector<Node*>* nodes, std::vector<Edge>* edges, std::string* sourceName);

std::tuple<std::vector<Node*>, std::vector<Edge>, Node*> readDotFile(std::string filePath) {
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "Could not read file from path " << filePath << "\n";
		exit(-0xF2);
	}

	// Read the whole file into a buffer 
	std::string buffer = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

	if (buffer.find("graph ") == std::string::npos) {
		std::cerr << "Could not find graph definition in file (definition begins with 'graph')\n";
		exit(-0xF3);
	} else {
		int openingBracketIndex = buffer.find("{");
		int closingBracketIndex = buffer.find("}");

		if (openingBracketIndex > closingBracketIndex || openingBracketIndex == std::string::npos || closingBracketIndex == std::string::npos) {
			std::cerr << "Syntax error (no } or {, or index of '{' is larger than index of '}')\n";
			exit(-0xF4);
		} else {
			std::string graphDefinition = buffer.substr(openingBracketIndex, closingBracketIndex);

			// Skip redundant characters - spaces will be removed from node names
			std::string processedDefinition = "";
			for (const char c : graphDefinition) {
				if (c == *"\n" || c == *"\t" || c == *" " || c == *"{" || c == *"}") continue;
				processedDefinition += c;
			}

			std::vector<Node*> nodes;
			std::vector<Edge> edges;
			std::string sourceName;

			int endOfPreviousStatement = 0;
			int endOfCurrentStatement = processedDefinition.find(";");

			while (endOfCurrentStatement != std::string::npos) {
				std::string statement = processedDefinition.substr(endOfPreviousStatement, endOfCurrentStatement - endOfPreviousStatement);
				processStatement(statement, &nodes, &edges, &sourceName);

				endOfPreviousStatement = endOfCurrentStatement + 1;
				endOfCurrentStatement = processedDefinition.find(";", endOfPreviousStatement);
			}

			// Find the Node with the same name as the source name in the file
			Node* source = *std::find_if(nodes.begin(), nodes.end(), [sourceName](const Node* w) {
				return w->name == sourceName;
			});

			return std::tuple<std::vector<Node*>, std::vector<Edge>, Node*> {nodes, edges, source};
		}
	}
}

static void processStatement(const std::string statement, std::vector<Node*>* nodes, std::vector<Edge>* edges, std::string* sourceName) {
	int connectionIndex = statement.find("--");
	int argumentIndex = statement.find("[weight=");

	// If statement contains '--' and '[weight=', it is treated as an edge definition
	if (connectionIndex != std::string::npos && argumentIndex != std::string::npos) {
		std::string firstNodeName = statement.substr(0, connectionIndex);
		std::string secondNodeName = statement.substr(connectionIndex + 2, argumentIndex - connectionIndex - 2);

		// If Node with given name is not in nodes, create and add it to the vector
		if (std::find_if(nodes->begin(), nodes->end(), [firstNodeName](const Node* w) {
			return w->name == firstNodeName;
			}) == nodes->end()) {
			Node* node1 = new Node;
			node1->name = firstNodeName;
			nodes->push_back(node1);
		}

		// Repeat for second node in statement
		if (std::find_if(nodes->begin(), nodes->end(), [secondNodeName](const Node* w) {
			return w->name == secondNodeName;
			}) == nodes->end()) {
			Node* node2 = new Node;
			node2->name = secondNodeName;
			nodes->push_back(node2);
		}

		// Find nodes of the edge in statement
		auto node1 = std::find_if(nodes->begin(), nodes->end(), [firstNodeName](const Node* w) {
			return w->name == firstNodeName;
			});
		auto node2 = std::find_if(nodes->begin(), nodes->end(), [secondNodeName](const Node* w) {
			return w->name == secondNodeName;
			});

		if (node1 != nodes->end() && node2 != nodes->end()) {
			// Find the edge weight in the statemend
			double weight = std::stod(statement.substr(argumentIndex + 6, statement.find("]") - argumentIndex - 6));

			Edge k(*node1, *node2, weight);
			edges->push_back(k);
			return;
		}
	}
	else {
		// Otherwise look for a definition of source name
		int sourceNameIndex = statement.find("source=");
		if (sourceNameIndex != std::string::npos) {
			*sourceName = statement.substr(sourceNameIndex + 8);
			return;
		}
		else {
			std::cerr << "Syntax error near '" << statement << "' (no edge weight argument '[weight=X]' or no connection indication '--', or incorrect source name definition)\n";
		}
	}
}
