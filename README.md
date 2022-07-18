# Dijkstra
A repository containing my C++ implementation of Dijkstra's algorithm, as well as a surrounding task

## The task
The additional task the program is doing is calculating which edge from a graph you can remove so that the sum of all distances to the source will be the smallest.

## Dijkstra
The algorithm is based on pseudocode from Wikipedia, and the only files concering the algorithm are Dijkstra, Edge and Node .h/.cpp files.

## Simplified DOT format
The program supports parsing and reading a simplified version of a DOT file. In this format:
- the name of the graph does not matter,
- only the first definition is read,
- a source definition is required with statement "source=source node name",
- the only, and required attribute of an edge definition is weight, which is a numeric value (not necessarily a integer),
- multigraphs are supported, but not digraphs

