//////////////////////////////////////////////////
//
// MerkleBTree.h
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// 
// the tree is similar to B+ tree
// and it corporates hash method to provide
// authentication
//////////////////////////////////////////////////

#ifndef _MERKLEBTREE_H_
#define _MERKLEBTREE_H_

#include <vector>
#include <string>
#include <set>

#include "Node.h"
#include "Graph.h"

class Graph;

#define DEFAULT_FANOUT 11
#define DEFAULT_HASH_METHOD "md5"

class MerkleBTreeNode{
private:
	bool isLeaf_; // if the node is a leaf node
	bool isRoot_; // if the node is the root of the tree
	MerkleBTreeNode* fatherNode;
	std::vector<unsigned int> keys;
	std::vector<MerkleBTreeNode*> childTreeNodes; // used when the node is not a leaf
	std::vector<Node*> dataItems; // used when the node is a leaf
	MerkleBTreeNode* nextLeaf; // used when the node is a leaf, to find the next sibling
	std::vector<std::string> digests; // the digest of the child nodes or date items
public:
	MerkleBTreeNode(bool isLeaf = false, bool isRoot = false, MerkleBTreeNode* fatherNode = NULL){
		this->isLeaf_= isLeaf;
		this->isRoot_ = isRoot;
		this->fatherNode = fatherNode;
		this->nextLeaf = NULL;
	};
	~MerkleBTreeNode(){};

	bool isLeaf() {
		return this->isLeaf_;
	}
	void setIsLeaf(bool value) {
		this->isLeaf_ = value;
	}
	bool isRoot() {
		return this->isRoot_;
	}
	void setIsRoot(bool value) {
		this->isRoot_ = value;
	}
	
	MerkleBTreeNode* getFatherNode() {
		return this->fatherNode;
	}
	void setFatherNode(MerkleBTreeNode* value) {
		this->fatherNode = value;
	}

	std::vector<unsigned int>& getKeys() {
		return this->keys;
	}
	unsigned int getKey(unsigned int index) {
		if (index >= 0 && index < this->keys.size()) {
			return this->keys[index];
		}
		return NULL;
	}
	int addKey(unsigned int key) { // return the position to which the key is added
		if (this->keys.size() == 0) {
			this->keys.push_back(key);
			return 0;
		}
		else{
			int i = 0;
			for (;i != this->keys.size();i++) {
				if (key < this->keys[i]){
					this->keys.insert(this->keys.begin()+i,key);
					return i;
				}
			}
			this->keys.push_back(key);
			return i;
		}
	}
	void addKey(int index, unsigned int key) {
		this->keys.insert(this->keys.begin()+index,key);
	}
	
	std::vector<Node*>& getDataItems() {
		return this->dataItems;
	}
	Node* getDataItem(unsigned int index) {
		if (index >= 0 && index < this->dataItems.size()) {
			return this->dataItems[index];
		}
		return NULL;
	}
	unsigned int dataItemsSize() {
		return this->dataItems.size();
	}
	void addDataItem(int index, Node* node) {
		this->dataItems.insert(this->dataItems.begin()+index,node);
	}
	
	MerkleBTreeNode* getChildTreeNode(unsigned int index) {
		if (index >= 0 && index < this->childTreeNodes.size()) {
			return this->childTreeNodes[index];
		}
		return NULL;
	}
	std::vector<MerkleBTreeNode*>& getChildTreeNodes() {
		return this->childTreeNodes;
	}
	unsigned int childTreeNodesSize() {
		return this->childTreeNodes.size();
	}
	void addChildTreeNode(int index, MerkleBTreeNode* treeNode) {
		this->childTreeNodes.insert(this->childTreeNodes.begin()+index,treeNode);
	}

	void setNextLeaf(MerkleBTreeNode* treeNode) {
		this->nextLeaf = treeNode;
	}

	void addDigest(std::string value) {
		this->digests.push_back(value);
	}
	const std::vector<std::string>& getDigests() {
		return this->digests;
	}
	std::string getDigest(int index) {
		return this->digests[index];
	}
};

typedef MerkleBTreeNode TreeNode;

class MerkleBTree {
private:
	Graph* graph;
	unsigned int fanout;
	TreeNode* root;
	std::string hashMethod;
public:
	MerkleBTree(Graph* graph, unsigned int fanout = DEFAULT_FANOUT, Node::INDEXMETHOD indexMethod = Node::DEFAULT, std::string hashMethod = DEFAULT_HASH_METHOD);
	~MerkleBTree(){}

	unsigned int getFanout() {
		return this->fanout;
	}
	std::string getHashMethodName() {
		return this->hashMethod;
	}


	Node* search(unsigned int key);
	Node* search(unsigned int key, TreeNode* treeNode);
	TreeNode* searchLeaf(unsigned int key); // search for the leaf that contains the data which has the key 
	TreeNode* searchLeaf(unsigned int key, TreeNode* treeNode);
	bool insert(Node*);


	TreeNode* leftestLeafNode();
	unsigned int numberOfTreeNodes();
	unsigned int numberOfLeaves();
	unsigned int numberOfDataItems();

	void printKeys();
	void printKeys(TreeNode* treeNode);
	void printDigests();
	void printDigests(TreeNode* treeNode);

	std::string generateVO(std::vector<Node*>& nodes);
	std::string calculateRootDigest();
private:
	bool insert(TreeNode*,Node*);
	void initializeDigest();
	void calculateDigest(MerkleBTreeNode*);
	std::string generateVO(MerkleBTreeNode* treeNode, std::set<int>& indexSet);
};

#endif // _MERKLEBTREE_H_