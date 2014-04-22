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

#include <string>
#include <iostream>
#include <algorithm>

#include "Crypto.h"
#include "Graph.h"
#include "Node.h"
#include "MerkleBTree.h"
#include "NodeHeap.h"
#include "Tools.h"
#include "AuthenticationTree.h"

using namespace std;

int main() {


	Graph* g = new Graph();
	//g->loadFromFile("test.node","test.edge");
	g->loadFromFile("cal_test.node","cal_test.txt");
	//g->loadFromFile("Dataset/CAL.cnode","Dataset/calmap");
	MerkleBTree* tree = new MerkleBTree(g,3);
	Crypto::printHashValue(tree->calculateRootDigest());
	cout << endl;
	//tree->printKeys();
	//tree->printDigests();
	vector<Node*> result = g->findKNNAndAllRelatedNodes(20,10);
	string VO = tree->generateVO(result);
	cout << VO.length() << endl;
	AuthenticationTree* authenticationTree = new AuthenticationTree();
	cout << authenticationTree->parseVO(VO) << "\n";
	//authenticationTree->printDigests();
	Crypto::printHashValue(authenticationTree->getRootDigest());
	cout << endl;
	return 0;

}