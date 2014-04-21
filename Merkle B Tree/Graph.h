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
#include "MerkleBTree.h"
class Node;
class Edge;
class MerkleBTree;
class Graph {

private:
	std::vector<Node*> nodes;
	std::vector<Edge*> edges;
	MerkleBTree* tree;
public:
	MerkleBTree* getTree() {
		return this->tree;
	}
	void setTree(MerkleBTree* value) {
		this->tree = value;
	}

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
	unsigned int numberOfNodes() {
		return this->nodes.size();
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
	unsigned int numberOfEdges() {
		return this->edges.size();
	}

	bool loadFromFile(std::string nodeFilename, std::string edgeFilename);
	void clear(){
		this->nodes.clear();
		this->edges.clear();
	}
	void flushNodes(); // for dijkstra algorithm, set the node state to UNSCANNED and clear the weight
	void makeIndex(Node::INDEXMETHOD); // set indexId in all the nodes
	void sortNodes(); // sort the nodes according to indexId of the nodes

	std::vector<Node*> findKNN(unsigned int sourceNodeId, unsigned int k);

	// for authentication use
	// the nodes scanned during the dijkstra algorithm will be contained in the stack
	std::vector<Node*> findKNNAndAllRelatedNodes(unsigned int sourceNodeId, unsigned int k);
};

#endif // _GRAPH_H_