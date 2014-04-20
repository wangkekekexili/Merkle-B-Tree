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
	Graph* g = new Graph();
	g->loadFromFile("test.node","test.edge");
	g->loadFromFile("cal_test.node","cal_test.txt");
	return 0;
}