#ifndef DecisionTree_h
#define DecisionTree_h

#include "DecisionTreeNode.h"
#include <string>   
/** 
* Author: Alperen Öziş
* ID: 21703804
* Section: 1
* Assignment: 2 */
using namespace std;
class DecisionTree {
public:
	void train(const bool** data, const int* labels, const int numSamples, const int numFeatures);
	void train(const string fileName, const int numSamples, const int numFeatures);
	int predict(const bool* sample);
	double test(const bool** data, const int* labels, const int numSamples);
	double test(const string fileName, const int numSamples);
	void print();
	void print(DecisionTreeNode* node, int level);
	~DecisionTree();
	DecisionTree();

private:

	DecisionTreeNode* root;

protected:
	void train(const bool** data, const int* labels, const int numSamples, const int numFeatures, DecisionTreeNode *node, bool* usedFeatures, bool* usedSamples);
	static double calculateEntropy(const int* classCounts, const int numClasses);
	static double calculateInformationGain(const bool** data, const int* labels, const int numSamples, const int numFeatures, const bool* usedSamples, const int featureId);
};

#endif