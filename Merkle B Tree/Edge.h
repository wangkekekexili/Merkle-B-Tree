//////////////////////////////////////////////////
//
// Edge.h
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
//////////////////////////////////////////////////

#ifndef _EDGE_H_
#define _EDGE_H_

#include <vector>

#include "Node.h"

class Node; // Node.h

class Edge {

private:
	unsigned int edgeId; // the id for the edge (from the input file normally)
	unsigned int indexId; 
	Node* startNode; 
	Node* endNode;
	double length;

public:
	Edge(Node* startNode, Node* endNode, double length) {
		this->edgeId = 0;
		this->indexId = 0;
		this->startNode = startNode;
		this->endNode = endNode;
		this->length = length;
	}
	Edge(unsigned int edgeId, Node* startNode, Node* endNode, double length) {
		this->edgeId = edgeId;
		this->indexId = 0;
		this->startNode = startNode;
		this->endNode = endNode;
		this->length = length;
	}
	~Edge(){}

	unsigned int getEdgeId() {
		return this->edgeId;
	}
	void setEdgeId(unsigned int value) {
		this->edgeId = edgeId;
	}

	unsigned int getIndexId() {
		return this->indexId;
	}
	void setIndexId(unsigned int value) {
		this->indexId = value;
	}

	Node* getStartNode() {
		return this->startNode;
	}

	Node* getEndNode() {
		return this->endNode;
	}

	double getLength() {
		return this->length;
	}

};

#endif //_EDGE_H_