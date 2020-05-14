#ifndef DecisionTreeNode_h
#define DecisionTreeNode_h
/** 
* Author: Alperen Öziş
* ID: 21703804
* Section: 1
* Assignment: 2 */
class DecisionTreeNode {
private:
	// instance variables
	int id;
	DecisionTreeNode *leftChildPtr;
	DecisionTreeNode *rightChildPtr;
	// default constructor


	// Decision Tree contains DTnodes. It must be able to access private parts of the DTnode.
	//friend class DecisionTree;
public:
	DecisionTreeNode();
	DecisionTreeNode(const int id, DecisionTreeNode* leftPtr = nullptr, DecisionTreeNode* rightPtr = nullptr);
	~DecisionTreeNode();
	void setId(int id);
	void setLeftChild(DecisionTreeNode* node);
	void setRightChild(DecisionTreeNode* node);
	DecisionTreeNode* getRightChild();
	DecisionTreeNode* getLeftChild();
	int getId();
};

#endif 