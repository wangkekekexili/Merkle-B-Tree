//////////////////////////////////////////////////
//
// AuthenticationTree.h
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// 
// for building VO and authencatation
//////////////////////////////////////////////////

#ifndef _AUTHENTICATION_H_
#define _AUTHENTICATION_H_

#include <vector>
#include <string>

class AuthenticationTreeNode {
private:
	std::vector<AuthenticationTreeNode*> childNodes; // if childNodes is empty, it is a leaf
	AuthenticationTreeNode* parentNode; // if it is NULL, it is the root
	bool isNode_; // is "value" contains node value or hash value?
	std::string value;

	std::string digest; // for authentication, contains the digest of all the information in the current node

public:
	AuthenticationTreeNode() {
		parentNode = NULL;
		isNode_ = false;
	};
	AuthenticationTreeNode(AuthenticationTreeNode* parentNode, bool isNode, std::string value) {
		this->parentNode = parentNode;
		this->isNode_ = isNode;
		this->value = value;
	}
	~AuthenticationTreeNode(){}

	const std::vector<AuthenticationTreeNode*>& getChildNodes() {
		return this->childNodes;
	}
	AuthenticationTreeNode* getChildNode(unsigned int index) {
		if (index >= 0 && index < this->childNodes.size()) {
			return this->childNodes[index];
		}
		else {
			return NULL;
		}
	}
	void addChildNode(AuthenticationTreeNode* value) {
		this->childNodes.push_back(value);
	}

	AuthenticationTreeNode* getParentNode() {
		return this->parentNode;
	}
	void setParentNode(AuthenticationTreeNode* value) {
		this->parentNode = value;
	}

	bool isNode() {
		return this->isNode_;
	}
	void setIsNode(bool value) {
		this->isNode_ = value;
	}

	const std::string& getValue() {
		return this->value;
	}
	void setValue(std::string value) {
		this->value = value;
	}

	const std::string& getDigest() {
		return this->digest;
	}
	void setDigest(std::string value) {
		this->digest = value;
	}
};

class AuthenticationTree {
private:
	AuthenticationTreeNode* root;
	std::string hashMethodName;
public:
	AuthenticationTree(std::string hashMethodName = "md5"){
		this->root = NULL;
		this->hashMethodName = hashMethodName;
	};
	~AuthenticationTree(){}

	AuthenticationTreeNode* getRoot() {
		return this->root;
	}
	void setRoot(AuthenticationTreeNode* value) {
		this->root = value;
	}

	bool parseVO(std::string);

	std::string getRootDigest();

	void printDigests();
	void printDigests(AuthenticationTreeNode*);
private:
	void calculateDigest();
	std::string calculateDigest(AuthenticationTreeNode*);
};

#endif // _AUTHENTICATION_H_