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
#include <vector>

#include "Tools.h"
#include "NodeHeap.h"

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
			// the PoI information length are not increasing
			// so we need to change them into the increasing order
			vector<int> categoryVector;
			categoryVector.clear();
			vector<double> lengthVector;
			lengthVector.clear();
			for (int i = 0;i != numberOfPoIs;i++) {
				iss >> category >> lengthToStartNode;
				categoryVector.push_back(category);
				lengthVector.push_back(lengthToStartNode);
			}
			// use selection sort
			for (int i = 0;i != lengthVector.size()-1;i++) {
				int mini = i;
				double minLength = lengthVector[i];
				for (int j = i+1;j != lengthVector.size();j++) {
					if (lengthVector[j] < minLength) { 
						mini = j;
						minLength = lengthVector[j];
					}
				}
				if (mini != i) {
					int tempInt = categoryVector[i];
					categoryVector[i] = categoryVector[mini];
					categoryVector[mini] = tempInt;
					double tempDouble = lengthVector[i];
					lengthVector[i] = lengthVector[mini];
					lengthVector[mini] = tempDouble;
				}
			}
			ostringstream oss;
			for (int i = 0;i != numberOfPoIs;i++) {
				oss << categoryVector[i] << " " << lengthVector[i] <<  " ";
			}
			iss.str(oss.str());

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

void Graph::flushNodes() {
	unsigned int size = this->numberOfNodes();
	for (int i = 0;i != size; i++) {
		Node* currentNode = this->getNode(i);
		currentNode->weight(0);
		currentNode->setState(Node::UNSCANNED);
	}
}

void Graph::makeIndex(Node::INDEXMETHOD indexMethod) {
	switch (indexMethod) {
	case Node::DEFAULT:
		for (vector<Node*>::const_iterator iter = this->nodes.begin();
			iter != this->nodes.end();
			iter++) {
				Node* temp = *iter;
				temp->setIndexId(temp->getNodeId());
		}
	case Node::HILBERT:
	default:
		break;
	}
}


void Graph::sortNodes() {
	// using selection sort
	// should be replaced by faster algorithm like quick sort

	unsigned int s = this->nodes.size();
	for (int i = 0;i != s-1;i++) {
		int min = i;
		int minKey = this->nodes[i]->key();
		for (int j = i+1;j != s;j++) {
			if (this->nodes[j]->key() < minKey) {
				minKey = this->nodes[j]->key();
				min = j;
			}
			if (min != i) {
				Node* temp = this->nodes[i];
				this->nodes[i] = this->nodes[min];
				this->nodes[min] = temp;
			}
		}
	}
}

vector<Node*> Graph::findKNN(unsigned int sourceNodeId, unsigned int k) {
	vector<Node*> resultVector;
	if (k == 0)
		return resultVector;
	Node* sourceNode = NULL;
	if (this->getTree() != NULL) { // the b tree is built, so we could find the node using the tree
		sourceNode = this->getTree()->search(sourceNodeId);
	}
	else { // we must traverse the "nodes" vector in the graph to find the node
		for (int i = 0;i != this->numberOfNodes();i++) {
			if (this->getNode(i)->getNodeId() == sourceNodeId) {
				sourceNode = this->getNode(i);
			}
		}
	}
	if (sourceNode == NULL) {
		cout << "Could not find source node!\n";
	}

	this->flushNodes();
	
	unsigned int PoIsFound = 0;
	NodeHeap* minHeap = new NodeHeap();
	minHeap->push(sourceNode);
	sourceNode->setState(Node::INHEAP);
	while (minHeap->empty() == false) {
		Node* current = minHeap->pop();
		current->setState(Node::SCANNED);
		if (current->getNodeType() == Node::POI) {
			resultVector.push_back(current);
			PoIsFound++;
			if (PoIsFound == k) {
				return resultVector;
			}
		}
		// scan the edges that conntect to the "current"
		for (int i = 0;i != current->getEdges().size();i++) {
			Edge* e = current->getEdge(i);
			Node* n = NULL;
			if (e->getStartNode() == current) {
				n = e->getEndNode();
			}
			else {
				n = e->getStartNode();
			}
			if(n->getState() == Node::UNSCANNED) {
				n->weight(current->weight() + e->getLength());
				n->setState(Node::INHEAP);
				minHeap->push(n);
			}
			else if (n->getState() == Node::INHEAP){
				if (n->weight() > current->weight() + e->getLength()) {
					minHeap->update(n,current->weight() + e->getLength());
				}
			}
		}
	}
	return resultVector;
}


std::vector<Node*> Graph::findKNNAndAllRelatedNodes(unsigned int sourceNodeId, unsigned int k) {
	vector<Node*> resultVector;
	if (k == 0)
		return resultVector;
	Node* sourceNode = NULL;
	if (this->getTree() != NULL) { // the b tree is built, so we could find the node using the tree
		sourceNode = this->getTree()->search(sourceNodeId);
	}
	else { // we must traverse the "nodes" vector in the graph to find the node
		for (int i = 0;i != this->numberOfNodes();i++) {
			if (this->getNode(i)->getNodeId() == sourceNodeId) {
				sourceNode = this->getNode(i);
			}
		}
	}
	if (sourceNode == NULL) {
		cout << "Could not find source node!\n";
	}

	this->flushNodes();
	
	double lastWeight = 0; // to prevent several lengths(weigths) be the same, need to scan more when the weights are equal even if KNN are found
	unsigned int PoIsFound = 0;
	NodeHeap* minHeap = new NodeHeap();
	minHeap->push(sourceNode);
	sourceNode->setState(Node::INHEAP);
	while (minHeap->empty() == false) {
		Node* current = minHeap->pop();
		if (PoIsFound == k && current->weight() > lastWeight)
			break;
		current->setState(Node::SCANNED);
		resultVector.push_back(current);
		if (current->getNodeType() == Node::POI) {
			PoIsFound++;
			if (PoIsFound == k) {
				lastWeight = current->weight();
			}
		}
		// scan the edges that conntect to the "current"
		for (int i = 0;i != current->getEdges().size();i++) {
			Edge* e = current->getEdge(i);
			Node* n = NULL;
			if (e->getStartNode() == current) {
				n = e->getEndNode();
			}
			else {
				n = e->getStartNode();
			}
			if(n->getState() == Node::UNSCANNED) {
				n->weight(current->weight() + e->getLength());
				n->setState(Node::INHEAP);
				minHeap->push(n);
			}
			else if (n->getState() == Node::INHEAP){
				if (n->weight() > current->weight() + e->getLength()) {
					minHeap->update(n,current->weight() + e->getLength());
				}
			}
		}
	}
	return resultVector;
}
