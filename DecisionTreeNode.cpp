#include "DecisionTreeNode.h"
/** 
* Author: Alperen Öziş
* ID: 21703804
* Section: 1
* Assignment: 2 */
DecisionTreeNode::DecisionTreeNode() {
	id = -1;
	leftChildPtr = nullptr;
	rightChildPtr = nullptr;
}
DecisionTreeNode::DecisionTreeNode(const int id1, DecisionTreeNode* left, DecisionTreeNode* right) {
	id = id1;
	leftChildPtr = left;
	rightChildPtr = right;
}
DecisionTreeNode::~DecisionTreeNode() {
	if (leftChildPtr)
		delete leftChildPtr;
	if (rightChildPtr)
		delete rightChildPtr;
}
int DecisionTreeNode::getId() {
	return id;
}
void DecisionTreeNode::setId(const int id1) {
	id = id1;
}
DecisionTreeNode* DecisionTreeNode::getRightChild() {
	return rightChildPtr;
}
DecisionTreeNode* DecisionTreeNode::getLeftChild() {
	return leftChildPtr;
}
void DecisionTreeNode::setLeftChild(DecisionTreeNode* node) {
	leftChildPtr = node;
}
void DecisionTreeNode::setRightChild(DecisionTreeNode* node) {
	rightChildPtr = node;
}


