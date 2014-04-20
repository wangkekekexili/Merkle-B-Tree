//////////////////////////////////////////////////
//
// Graph.cpp
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
//////////////////////////////////////////////////

#include "Graph.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Tools.h"

using namespace std;

bool Graph::loadFromFile(string nodeFilename, string edgeFilename) {
	// open the node file and edge file
	// the files follow format from Lifeifei's data

	// make the graph to the empty state
	this->clear();

	// open the node file and read in node data
	// not including PoIs
	fstream inputFileStream(nodeFilename,ios::in);
	if (inputFileStream.fail() == true) {
		Tools::warning("Node File Opening Error!");
		return false;
	}
	int nodeId;
	double longitude, latitude;
	while(inputFileStream >> nodeId >> longitude >> latitude) {
		Node* n = new Node(nodeId, longitude, latitude);
		this->addNode(n);
	}
	inputFileStream.close();

	// open the edge file and read in edge data
	// including PoIs, so we need to add some additional nodes
	// and split the corresponding edge to two or more
	inputFileStream.open(edgeFilename,ios::in);
	if (inputFileStream.fail() == true) {
		Tools::warning("Edge File Opening Error!");
		return false;
	}
	int edgeCount = 0; // count the number of edges and work as edgeId as well
	int startNodeId, endNodeId, numberOfPoIs;
	double length;
	while(inputFileStream >> startNodeId >> endNodeId >> length >> numberOfPoIs) {
		if (numberOfPoIs == 0){ // if there are no pois on the edge
			Node* startNode = this->getNode(startNodeId);
			Node* endNode = this->getNode(endNodeId);

			// create a new edge
			Edge* e = new Edge(this->nextEdgeId(), startNode, endNode, length);

			// add the edge to the graph
			this->addEdge(e);

			// add the edge to the startNode and endNode
			startNode->addEdge(e);
			endNode->addEdge(e);
		}
		else { // there are pois on the edge
			Node* startNode = this->getNode(startNodeId);
			Node* endNode = this->getNode(endNodeId);

			// read next line
			// this line will contain PoI information
			string line;
			getline(inputFileStream, line);
			getline(inputFileStream, line);
			istringstream iss(line);
			int category;
			double lengthToStartNode;
			Node* lastNode = startNode; // the Node that will act as the start node of the new edge
			double lastLength = 0;
			for (int i = 0;i != numberOfPoIs;i++) {
				iss >> category >> lengthToStartNode;
				
				// create a new PoI node
				Node* poi = new Node(this->nextNodeId(),
					startNode->getLongitude()+(endNode->getLongitude()-startNode->getLongitude())*(lengthToStartNode/length),
					startNode->getLatitude()+(endNode->getLatitude()-startNode->getLatitude())*(lengthToStartNode/length),
					Node::POI);

				// add the new PoI node to the graph
				this->addNode(poi);

				// create a new corresponding edge
				Edge* edge = new Edge(this->nextEdgeId(),lastNode,poi,lengthToStartNode-lastLength);

				// add the edge to the graph
				this->addEdge(edge);

				// add the edge to the lastNode and PoI Node
				lastNode->addEdge(edge);
				poi->addEdge(edge);

				// assign PoI Node to lastNode
				lastNode = poi;

				// assign current length to last length
				lastLength = lengthToStartNode;

			}
			// create the last edge connecting the last poi on the edge and the endNode
			// create the edge
			Edge* lastEdge = new Edge(this->nextEdgeId(),lastNode,endNode,length-lastLength);

			// add the edge to the graph
			this->addEdge(lastEdge);

			// add the edge to the last poi and endNode
			lastNode->addEdge(lastEdge);
			endNode->addEdge(lastEdge);
		}
	}
	inputFileStream.close();

	return true;
}
