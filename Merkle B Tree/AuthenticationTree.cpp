//////////////////////////////////////////////////
//
// AuthenticationTree.cpp
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// 
// for building VO and authencatation
//////////////////////////////////////////////////

#include "AuthenticationTree.h"
#include "Tools.h"
#include "Crypto.h"

#include <vector>
#include <iostream>
#include <sstream>
#include <queue>

using namespace std;


bool AuthenticationTree::parseVO(std::string s) {	
	if (s.length() == 0) {
		return false;
	}

	// initially the root is NULL
	// the root will be created during the parse of "s"
	this->root = NULL;
	AuthenticationTreeNode* currentTreeNode = NULL;

	unsigned int index = 0;

	char nextChar;
	while(index < s.length()) {
		nextChar = s[index];
		string tempValue = "";
		switch (nextChar) {
		case '[':
			if (currentTreeNode == NULL) {
				currentTreeNode = new AuthenticationTreeNode();
				this->root = currentTreeNode;
			}
			else {
				AuthenticationTreeNode* newNode = new AuthenticationTreeNode(currentTreeNode,false,"");
				currentTreeNode->addChildNode(newNode);
				currentTreeNode = newNode;
			}
			break;
		case ']':
			currentTreeNode = currentTreeNode->getParentNode();
			break;
		case ',':
			break;
		case 'h':
			index += 5;
			if (index >= s.length()) {
				cout << "Authentication Failed.\n";
				return false;
			}
			while(index < s.length() && s[index] != ']'){
				tempValue += s[index];
				index++;
			}
			index--; // the current char is probably ']', so go back one char
			currentTreeNode->setValue(tempValue);
			break;
		case 'n':
			index += 5;
			if (index >= s.length()) {
				cout << "Authentication Failed.\n";
				return false;
			}
			while(index < s.length() && s[index] != ']'){
				tempValue += s[index];
				index++;
			}
			index--; // the current char is probably ']', so go back one char
			currentTreeNode->setValue(tempValue);
			currentTreeNode->setIsNode(true);
			break;
		default:
			Tools::warning("The program should not go here.\n");
			exit(1);
		}
		index++;
	}

	// the authentication tree is initialized
	// next, calculate the digests
	this->calculateDigest();
	return true;
}


string AuthenticationTree::getRootDigest() {
	return this->root->getDigest();
}

void AuthenticationTree::calculateDigest() {
	if (this->root == NULL) {
		return;
	}
	this->root->setDigest(this->calculateDigest(this->root));
}

string AuthenticationTree::calculateDigest(AuthenticationTreeNode* treeNode) {
	// if the treeNode contains hash value(not a node and has value), then just return the value
	// elseif the treeNode is a node, then calculate the digest and return the value
	// else, calculate the digest recursively

	if (treeNode->isNode() == false && treeNode->getValue() != "") {
		treeNode->setDigest(Tools::hexToString(treeNode->getValue()));
		return treeNode->getDigest();
	}
	else if (treeNode->isNode() == true) {
		// TODO:
		// the nodes and edges could be extracted here
		string nodeInformation = treeNode->getValue();
		treeNode->setDigest(Crypto::hash(nodeInformation,this->hashMethodName));
		return treeNode->getDigest();
	}
	else {
		string contentToDigest = "";
		for (int i = 0;i != treeNode->getChildNodes().size();i++) {
			contentToDigest += this->calculateDigest(treeNode->getChildNode(i));
		}
		treeNode->setDigest(Crypto::hash(contentToDigest,this->hashMethodName));
		return treeNode->getDigest();
	}

}

void AuthenticationTree::printDigests() {
	if (this->root == NULL) {
		return; 
	}
	this->printDigests(this->root);
}
void AuthenticationTree::printDigests(AuthenticationTreeNode* node) {
	unsigned int currentLevelTreeNodes = 1;
	unsigned int nextLevelTreeNodes = 0;
	queue<AuthenticationTreeNode*> q;
	q.push(node);
	while (q.empty() == false) {
		AuthenticationTreeNode* current = q.front();
		q.pop();
		Crypto::printHashValue(current->getDigest());
		cout << " ";
		for (int i = 0;i != current->getChildNodes().size();i++) {
			q.push(current->getChildNode(i));
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
