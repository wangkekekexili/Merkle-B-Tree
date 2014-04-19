//////////////////////////////////////////////////
//
// Graph.h
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
//////////////////////////////////////////////////

#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <vector>
#include <string>

#include "Node.h"
#include "Edge.h"

class Node;
class Edge;

class Graph {

private:
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;

public:
	const std::vector<Node*>& getNodes() {
		return this->nodes;
	}
	void addNode(Node* value) {
		this->nodes.push_back(value);
	}
	const std::vector<Edge*>& getEdges() {
		return this->edges;
	}
	void addEdge(Edge* value) {
		this->edges.push_back(value);
	}

	bool loadFromFile(std::string nodeFilename, std::string edgeFilename);
};

#endif // _GRAPH_H_