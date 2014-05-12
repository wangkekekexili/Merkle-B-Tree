//////////////////////////////////////////////////
//
// Driver.cpp
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// This file contains some tests
//
//////////////////////////////////////////////////

#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

#include "Crypto.h"
#include "Graph.h"
#include "Node.h"
#include "MerkleBTree.h"
#include "NodeHeap.h"
#include "Tools.h"
#include "AuthenticationTree.h"

#ifdef _WIN32
#include <Windows.h>
#pragma comment(lib,"winmm")
#endif // _WIN32

using namespace std;

#define TIMESOFTEST 100

int main(int argc, char** argv) {
	
	// for construction time
#ifdef _WIN32
	DWORD begin =  timeGetTime();
#endif
	Graph* g = new Graph();
	g->loadFromFile(argv[1],argv[2]);
	MerkleBTree* tree = new MerkleBTree(g,3);
#ifdef _WIN32
	DWORD end = timeGetTime();
#endif
	fprintf(stdout, "Time Used: %.3lf\n", (end-begin)*1.0/1000);


	/*
	if (argc < 4) {
		cout << "Usage: " << argv[0] << " nodeFilename edgeFilename outputFilename fanout\n";
		exit(1);
	}

	cout << "Read In Graph.\n";
	Graph* g = new Graph();
	if (g->loadFromFile(argv[1],argv[2]) == false) {
		cout << "Error!" << endl;
		exit(1);
	}
	
	fstream outputFileStream(argv[3],ios::out|ios::trunc);
	outputFileStream << "NodeId,KNN,VOSize\n";
	cout << "Build Merkle B Tree.\n";
	MerkleBTree* tree = new MerkleBTree(g,atoi(argv[4]));
	cout << "Root Digest: ";
	Crypto::printHashValue(tree->calculateRootDigest());
	cout << endl;
	
	
	for (int i = 0;i != TIMESOFTEST;i ++) {
		cout << "Choose a random node.\n";
		int size = g->numberOfNodes();
		int randomNodeIndex = rand() % (size+1);
		cout << "Node IndexId: " << randomNodeIndex << "\n";

		for (int j = 1;j <= 128;j*=2) {
			cout << "Find KNN: " << j << "\n";
			vector<Node*> result = g->findKNNAndAllRelatedNodes(randomNodeIndex,j);

			cout << "Generate VO.\n";
			string VO = tree->generateVO(result);

			cout << "VO Size: ";
			cout << VO.length() << endl;
			outputFileStream << randomNodeIndex << "," << j << "," << VO.length() << "\n";

			cout << "Build Authentication Tree.\n";
			AuthenticationTree* authenticationTree = new AuthenticationTree();
			cout << authenticationTree->parseVO(VO) << "\n";

			cout << "Root Digest: ";
			Crypto::printHashValue(authenticationTree->getRootDigest());
			cout << endl;

			delete authenticationTree;
		}
		cout << "\n\n";
	}
	outputFileStream.close();
	*/
	return 0;
}
