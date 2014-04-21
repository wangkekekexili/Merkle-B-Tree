//////////////////////////////////////////////////
//
// Driver.cpp
//
// Ke Wang
// 10300240013@fudan.edu.c
//
// This file contains some tests
//
//////////////////////////////////////////////////


#include <iostream>
#include <algorithm>

#include "Crypto.h"
#include "Graph.h"
#include "Node.h"
#include "MerkleBTree.h"
#include "NodeHeap.h"

using namespace std;

int main() {
	Graph* g = new Graph();
	//g->loadFromFile("test.node","test.edge");
	g->loadFromFile("cal_test.node","cal_test.txt");
	MerkleBTree* tree = new MerkleBTree(g,10);

	//tree->printKeys();
	vector<Node*> result = g->findKNNAndAllRelatedNodes(2,8);
	sort(result.begin(),result.end(),Node::NodeCompare());
	return 0;
}