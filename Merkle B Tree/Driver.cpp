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

#include "Node.h"

using namespace std;

int main() {
	Node* n = new Node(0,1,2,Node::POI);
	cout << n->getLongitude();
	n->getEdges();
	return 0;
}