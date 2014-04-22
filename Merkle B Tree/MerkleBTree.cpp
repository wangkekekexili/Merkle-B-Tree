//////////////////////////////////////////////////
//
// MerkleBTree.cpp
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// 
//////////////////////////////////////////////////

#include "MerkleBTree.h"
#include "Graph.h"
#include "Node.h"
#include "Crypto.h"
#include "Tools.h"

#include <vector>
#include <string>
#include <iostream>
#include <queue>
#include <sstream>
#include <iomanip>
#include <set>

using namespace std;



MerkleBTree::MerkleBTree(Graph* graph, unsigned int fanout, Node::INDEXMETHOD indexMethod, std::string hashMethod) {
	// some preparation and initialization
	this->graph = graph;
	graph->setTree(this);
	this->fanout = fanout;
	this->root = NULL;
	this->hashMethod = hashMethod;

	// make index on the graph according to the indexMethod
	graph->makeIndex(indexMethod);

	// bulk load data from nodes in the graph
	// The first step is to sort the data entries according to a search key.
	graph->sortNodes();

	// add nodes to the tree
	// use normal insert here
	// should be replaced by bulk load later
	unsigned int numberOfNodes = graph->numberOfNodes();
	for (int i = 0;i != numberOfNodes;i++) {
		this->insert(graph->getNode(i));
	} 

	this->initializeDigest();
}


Node* MerkleBTree::search(unsigned int key) {
	if (this->root == NULL) {
		return NULL;
	}
	else {
		return this->search(key, this->root);
	}
}
Node* MerkleBTree::search(unsigned int key, TreeNode* treeNode) {
	if (treeNode->isLeaf() == true) {
		int l = treeNode->dataItemsSize(); // how many data items in the leaf node
		for (int i = 0;i != l;i++) {
			if (treeNode->getDataItem(i)->key() == key) {
				return treeNode->getDataItem(i);
			}
		}
		return NULL;
	}
	else { // the tree node is not a leaf
		int l = treeNode->getKeys().size();
		int i = 0;
		for (;i != l;i++) {
			if (key < treeNode->getKey(i)) {
				return search(key, treeNode->getChildTreeNode(i));
			}
		}
		return search(key, treeNode->getChildTreeNode(l));
		
	}
}
TreeNode* MerkleBTree::searchLeaf(unsigned int key) {
	if (this->root == NULL) {
		return NULL;
	}
	else {
		return this->searchLeaf(key, this->root);
	}
}
TreeNode* MerkleBTree::searchLeaf(unsigned int key, TreeNode* treeNode) {
	if (treeNode->isLeaf() == true) {
		return treeNode;
	}
	else { // if the tree node is not a leaf
		int l = treeNode->getKeys().size();
		int i = 0;
		for (;i != l;i++) {
			if (key < treeNode->getKey(i)) {
				return searchLeaf(key, treeNode->getChildTreeNode(i));
			}
		}

		return searchLeaf(key, treeNode->getChildTreeNode(l));
		
	}
}
bool MerkleBTree::insert(Node* node) {
	// insert the item(node) into the tree

	// Perform a search to determine what bucket the new record should go into.
	TreeNode* leafToInsert = NULL;
	if (this->root == NULL) {
		TreeNode* rootNode = new TreeNode(true, true, NULL);
		this->root = rootNode;
		leafToInsert = rootNode;
	}
	else {
		leafToInsert = this->searchLeaf(node->key());
	}

	// If the bucket is not full (at most b - 1 entries after the insertion), add the record.
	if (leafToInsert->dataItemsSize() <= this->fanout-2) {
		int position = leafToInsert->addKey(node->key());
		leafToInsert->addDataItem(position,node);
	}
	else{
		// Otherwise, split the bucket.
		// Allocate new leaf and move half the bucket's elements to the new bucket.
		TreeNode* newLeaf = new TreeNode(true,false,NULL);
		leafToInsert->setNextLeaf(newLeaf);
		newLeaf->setFatherNode(leafToInsert->getFatherNode());
		int position = leafToInsert->addKey(node->key());
		leafToInsert->addDataItem(position,node);
		for(int i = this->fanout/2;i != this->fanout;i++) {
			newLeaf->addKey(i - this->fanout/2,leafToInsert->getKey(i));
			newLeaf->addDataItem(i - this->fanout/2,leafToInsert->getDataItem(i));
		}
		for (int i = this->fanout/2;i != this->fanout;i++) {
			leafToInsert->getKeys().pop_back();
			leafToInsert->getDataItems().pop_back();
		}
		// Insert the new leaf's smallest key and address into the parent.
		TreeNode* parent = leafToInsert->getFatherNode();
		// If the parent is full, split it too.
		// Add the middle key to the parent node.
		// Repeat until a parent is found that need not split.
		unsigned int tempKey = newLeaf->getKey(0);
		TreeNode* newTreeNode = newLeaf;
		while (parent != NULL) {
			leafToInsert = parent;
			if (parent->childTreeNodesSize() <= this->fanout - 1){
				int position =  parent->addKey(tempKey);
				parent->addChildTreeNode(position+1, newTreeNode);
				break;
			}
			else{ // the parent is full
				int position = parent->addKey(tempKey);
				parent->addChildTreeNode(position+1, newTreeNode);
				newTreeNode = new TreeNode(false, false, NULL);
				newTreeNode->setFatherNode(parent->getFatherNode());
				for (int i = this->fanout/2+1;i != this->fanout;i++) {
					newTreeNode->getKeys().push_back(parent->getKey(i));
					newTreeNode->getChildTreeNodes().push_back(parent->getChildTreeNode(i));
					parent->getChildTreeNode(i)->setFatherNode(newTreeNode);
				}
				newTreeNode->getChildTreeNodes().push_back(parent->getChildTreeNode(this->fanout));
				parent->getChildTreeNode(this->fanout)->setFatherNode(newTreeNode);
				parent->getChildTreeNodes().pop_back();
				for (int i = this->fanout/2+1;i != this->fanout;i++) {
					parent->getKeys().pop_back();
					parent->getChildTreeNodes().pop_back();
				}
				tempKey = parent->getKeys().back();
				parent->getKeys().pop_back();

				parent = parent->getFatherNode();
			}
		}
		// If the root splits, create a new root which has one key and two pointers. 
		// (That is, the value that gets pushed to the new root gets removed from the original node)
		if (parent == NULL) {
			leafToInsert->setIsRoot(false);
			TreeNode* newRoot = new TreeNode(false, true, NULL);
			this->root = newRoot;
			newRoot->getKeys().push_back(tempKey);
			newRoot->getChildTreeNodes().push_back(leafToInsert);
			newRoot->getChildTreeNodes().push_back(newTreeNode);
			leafToInsert->setFatherNode(newRoot);
			newTreeNode->setFatherNode(newRoot);
		}
	}
	return true;
}
bool MerkleBTree::insert(TreeNode* treeNode, Node* node) {
	return false;
}

void MerkleBTree::printKeys() {
	TreeNode* current = this->root;
	if (current == NULL)
		return;
	this->printKeys(current);
}
void MerkleBTree::printKeys(TreeNode* node) {
	unsigned int currentLevelTreeNodes = 1;
	unsigned int nextLevelTreeNodes = 0;
	queue<TreeNode*> q;
	q.push(node);
	while (q.empty() == false) {
		TreeNode* current = q.front();
		q.pop();
		for (int i = 0;i != current->getKeys().size();i++) {
			cout << current->getKey(i) << " ";
		}
		for (int i = 0;i != current->getChildTreeNodes().size();i++) {
			q.push(current->getChildTreeNode(i));
			nextLevelTreeNodes++;
		}
		currentLevelTreeNodes--;
		if (currentLevelTreeNodes == 0){ // the current level is finished 
			currentLevelTreeNodes = nextLevelTreeNodes;
			nextLevelTreeNodes = 0;
			cout << "\n";
		}
		else {
			cout << "\t";
		}

	}
}

void MerkleBTree::printDigests() {
	if (this->root == NULL) {
		return; 
	}
	this->printDigests(this->root);
}
void MerkleBTree::printDigests(TreeNode* node) {
	unsigned int currentLevelTreeNodes = 1;
	unsigned int nextLevelTreeNodes = 0;
	queue<TreeNode*> q;
	q.push(node);
	while (q.empty() == false) {
		TreeNode* current = q.front();
		q.pop();
		for (int i = 0;i != current->getDigests().size();i++) {
			Crypto::printHashValue(current->getDigest(i));
			cout << " ";
		}
		for (int i = 0;i != current->getChildTreeNodes().size();i++) {
			q.push(current->getChildTreeNode(i));
			nextLevelTreeNodes++;
		}
		currentLevelTreeNodes--;
		if (currentLevelTreeNodes == 0){ // the current level is finished 
			currentLevelTreeNodes = nextLevelTreeNodes;
			nextLevelTreeNodes = 0;
			cout << "\n";
		}
		else {
			cout << "\t\t";
		}

	}

}

void MerkleBTree::initializeDigest() {
	if (this->root == NULL) {
		return;
	}
	else {
		this->calculateDigest(this->root);
	}
}

void MerkleBTree::calculateDigest(TreeNode* treeNode) {
	if (treeNode->isLeaf() == true) {
		unsigned int size = treeNode->dataItemsSize();
		for (int i = 0;i != size;i++) {
			// "oss" has the string that will be digested
			// it has the format:
			// nodeId, longitude, latitude, nodeType, numberOfEdges, {edgeId, length, startNodeId, endNodeId}
			ostringstream oss;
			oss << setprecision(10);
			Node* current = treeNode->getDataItem(i);
			oss << current->getNodeId() << "," << current->getLongitude() << "," << current->getLatitude() << ","
				<< current->getNodeType() << "," << current->getEdges().size();
			for (int j = 0;j != current->getEdges().size();j++) {
				Edge* currentEdge = current->getEdge(j);
				oss << "," << currentEdge->getEdgeId() << "," << currentEdge->getLength() << ","
					<< currentEdge->getStartNode()->getNodeId() << "," << currentEdge->getEndNode()->getNodeId();
			}
			treeNode->addDigest(Crypto::hash(oss.str(),this->getHashMethodName()));
		}
	}
	else { // tree node is not a leaf
		unsigned int size = treeNode->childTreeNodesSize();
		// first, calculate the digests of the children
		for (int i = 0;i != size;i++) {
			this->calculateDigest(treeNode->getChildTreeNode(i));
		}
		// second, calculate the digest in the current treeNode
		for (int i = 0;i != size;i++) {
			ostringstream oss;
			oss.clear();
			TreeNode* currentChildNode = treeNode->getChildTreeNode(i);
			for (int j = 0;j != currentChildNode->getDigests().size();j++) {
				oss << currentChildNode->getDigest(j);
			}
			treeNode->addDigest(Crypto::hash(oss.str(),this->getHashMethodName()));
		}
	}
}


string MerkleBTree::generateVO(std::vector<Node*>& nodes) {
	set<int> indexSet;
	for (vector<Node*>::const_iterator iter = nodes.begin();iter != nodes.end();iter++) {
		indexSet.insert((*iter)->getIndexId());
	}
	return this->generateVO(this->root,indexSet);
}

string MerkleBTree::calculateRootDigest() {
	if (this->root == NULL)
		return "";
	ostringstream oss;
	for (int j = 0;j != this->root->getDigests().size();j++) {
		oss << this->root->getDigest(j);
	}
	return Crypto::hash(oss.str(), this->getHashMethodName());
}


string MerkleBTree::generateVO(MerkleBTreeNode* treeNode, std::set<int>& indexSet) {
	string s = "";
	if (treeNode->isLeaf() == true) {
		s += "[";
		for (int i = 0;i != treeNode->dataItemsSize();i++) {
			Node* currentNode = treeNode->getDataItem(i);
			if (indexSet.find(currentNode->getIndexId()) != indexSet.end()) { // the node's index is found in the indexSet
				ostringstream oss;
				oss.clear();
				oss << setprecision(10);
				oss << "[node," << currentNode->getNodeId() << "," << currentNode->getLongitude() << ","
					<< currentNode->getLatitude() << "," << currentNode->getNodeType() << "," << currentNode->getEdges().size();
				for (int j = 0;j != currentNode->getEdges().size();j++) {
					Edge* currentEdge = currentNode->getEdge(j);
					oss << "," << currentEdge->getEdgeId() << "," << currentEdge->getLength() << "," 
						<< currentEdge->getStartNode()->getNodeId() << "," << currentEdge->getEndNode()->getNodeId();
				}
				oss << "]";
				s += oss.str();
			}
			else { // the node is not needed, so the hash value is includeed
				ostringstream oss;
				oss.clear();
				oss << "[hash," << Tools::stringToHex(treeNode->getDigest(i)) << "]";
				s += oss.str();
			}
		}
		s += "]";
	}
	else { // the treeNode is not a leaf
		s += "[";
		int numberOfKeys = treeNode->getKeys().size();
		for (int i = 0;i!= numberOfKeys;i++) {
			int key = treeNode->getKey(i);
			set<int> originalIndexSet = indexSet;
			set<int> newIndexSet;
			for (set<int>::const_iterator iter = originalIndexSet.begin(); iter != originalIndexSet.end(); iter++) {
				if ((*iter) < key) {
					newIndexSet.insert((*iter));
					indexSet.erase(*iter);
				}
			}
			if (newIndexSet.size() == 0) {
				ostringstream oss;
				oss.clear();
				oss << "[hash," << Tools::stringToHex(treeNode->getDigest(i)) << "]";
				s += oss.str();
			}
			else {
				s += this->generateVO(treeNode->getChildTreeNode(i), newIndexSet);
			}
		}
		if (indexSet.size() == 0) {
			ostringstream oss;
			oss.clear();
			oss << "[hash," << Tools::stringToHex(treeNode->getDigest(numberOfKeys)) << "]";
			s += oss.str();
		}
		else {
			s += this->generateVO(treeNode->getChildTreeNode(numberOfKeys), indexSet);
		}
		s += "]";
	}

	return s;
}
