//////////////////////////////////////////////////
//
// NodeHeap.h
//
// Ke Wang
// 10300240013@fudan.edu.cn
//
// used as a min heap, and the item is "Node" 
//
//////////////////////////////////////////////////

#ifndef _NODEHEAP_H_
#define _NODEHEAP_H_

#include <vector>

#include "Node.h"

class NodeHeap
{
public:
	NodeHeap(void);
	~NodeHeap(void);

	void push(Node*);
	Node* pop();
	void update(Node* node, double weight);
	bool empty();
	int size();

private:
	std::vector<Node*> nodes_;
};

#endif // _NODEHEAP_H_