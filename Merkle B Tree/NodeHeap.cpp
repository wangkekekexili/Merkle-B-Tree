//////////////////////////////////////////////////
//
// NodeHeap.cpp
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// 
//////////////////////////////////////////////////

#include "NodeHeap.h"


NodeHeap::NodeHeap(void)
{
}


NodeHeap::~NodeHeap(void)
{
}

void NodeHeap::push(Node* n){
	this->nodes_.push_back(n);
	int index = this->nodes_.size() - 1;
	while (index > 0) {
		int fatherIndex = (index-1)/2;
		if (this->nodes_[index]->weight() < this->nodes_[fatherIndex]->weight()) {
			Node* temp = this->nodes_[index];
			this->nodes_[index] = this->nodes_[fatherIndex];
			this->nodes_[fatherIndex] = temp;
		}
		index = fatherIndex;
	}
}

Node* NodeHeap::pop() {
	
	if (this->nodes_.size() == 0) {
		return NULL;
	}

	Node* returnNode = this->nodes_[0];
	this->nodes_[0] = this->nodes_[this->nodes_.size() - 1];
	this->nodes_.erase(this->nodes_.end() - 1);
	int index = 0, left = 1, right = 2;
	int size = this->nodes_.size()-1;
	while (left <= size) {
		int smallerIndex;
		if (left == size)
			smallerIndex = left;
		else if (this->nodes_[left]->weight() <= this->nodes_[right]->weight()) {
			smallerIndex = left;
		}
		else {
			smallerIndex = right;
		}
		if (this->nodes_[index]->weight() <= this->nodes_[smallerIndex]->weight()) {
			break;
		}
		else {
			Node* temp = this->nodes_[index];
			this->nodes_[index] = this->nodes_[smallerIndex];
			this->nodes_[smallerIndex] = temp;
			index = smallerIndex;
			left = 2*index + 1;
			right = 2*index + 2;
		}
	}

	return returnNode;
}

void NodeHeap::update(Node* node, double weight) {
	int index = -1;
	for (std::vector<Node*>::const_iterator iter = this->nodes_.begin();
		iter != this->nodes_.end();iter++) {
			if (*iter == node)
				index = iter - this->nodes_.begin();
	}
	node->weight(weight);
	while (index > 0) {
		int fatherIndex = (index-1)/2;
		if (this->nodes_[index]->weight() < this->nodes_[fatherIndex]->weight()) {
			Node* temp = this->nodes_[index];
			this->nodes_[index] = this->nodes_[fatherIndex];
			this->nodes_[fatherIndex] = temp;
		}
		index = fatherIndex;
	}
}

bool NodeHeap::empty() {
	return this->nodes_.empty();
}

int NodeHeap::size() {
	return this->nodes_.size();
}