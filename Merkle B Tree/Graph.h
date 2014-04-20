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
	Node* getNode(int index) {
		return this->nodes[index];
	}
	unsigned int nextNodeId() {
		return this->nodes.size();
	}
	void addNode(Node* value) {
		this->nodes.push_back(value);
	}

	const std::vector<Edge*>& getEdges() {
		return this->edges;
	}
	Edge* getEdge(int index) {
		return this->edges[index];
	}
	unsigned int nextEdgeId() {
		return this->edges.size();
	}
	void addEdge(Edge* value) {
		this->edges.push_back(value);
	}

	bool loadFromFile(std::string nodeFilename, std::string edgeFilename);
	void clear(){
		this->nodes.clear();
		this->edges.clear();
	}
};

#endif // _GRAPH_H_