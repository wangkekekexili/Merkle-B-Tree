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

#include "Crypto.h"
#include "Graph.h"
#include "Node.h"
#include "MerkleBTree.h"

using namespace std;

int main() {
	Graph* g = new Graph();
	//g->loadFromFile("test.node","test.edge");
	g->loadFromFile("cal_test.node","cal_test.txt");
	MerkleBTree* tree = new MerkleBTree(g,10);
	for (int i = 0;i != g->getNodes().size();i++){
		tree->insert(g->getNode(i));
		//tree->printKeys();
	}
	tree->printKeys();
	TreeNode* result = tree->searchLeaf(4);
	return 0;
}