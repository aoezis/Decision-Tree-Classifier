#include "DecisionTree.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
/** 
* Author: Alperen Öziş
* ID: 21703804
* Section: 1
* Assignment: 2 */
using namespace std;

double DecisionTree::calculateEntropy(const int * classCounts, const int numClasses) {
	double sumClass = 0;
	for (int i = 0; i < numClasses; i++) {
		sumClass += classCounts[i];
	}

	double sum = 0;
	for (int i = 0; i < numClasses; i++) {
		if (classCounts[i] != 0) {
			sum += (classCounts[i] / sumClass)*(log2(classCounts[i] / sumClass));
		}
	}
	return -sum;
}

double DecisionTree::calculateInformationGain(const bool** data, const int* labels, const int numSamples, const int numFeatures, const bool* usedSamples, const int featureId) {
	double initialEntropy = 0.0;
	double splitEntropy = 0.0;
	double leftEntropy = 0.0;
	double rightEntropy = 0.0;
	double leftProb = 0.0;
	double rightProb = 0.0;

	int numClasses = 0;
	int leftSize = 0; //for the calculation of leftProb
	int rightSize = 0; //for the calculation of rightProb

	for (int i = 0; i < numSamples; i++) {
		if (labels[i] > numClasses)
			numClasses = labels[i];
	}
	int* classCounts = new int[numClasses];
	int* leftClassCounts = new int[numClasses];
	int* rightClassCounts = new int[numClasses];

	for (int i = 0; i < numClasses; i++) {
		classCounts[i] = 0;
		leftClassCounts[i] = 0;
		rightClassCounts[i] = 0;
	}

	for (int i = 0; i < numSamples; i++) {
		if (!usedSamples[i]) {
			if (data[i][featureId] == 1) {
				rightClassCounts[labels[i] - 1]++;
			}
			else {
				leftClassCounts[labels[i] - 1]++;
			}
			classCounts[labels[i] - 1]++;
		}
	}

	for (int i = 0; i < numClasses; i++) {
		leftSize += leftClassCounts[i];
		rightSize += rightClassCounts[i];
	}

	initialEntropy = calculateEntropy(classCounts, numClasses);
	leftEntropy = calculateEntropy(leftClassCounts, numClasses);
	rightEntropy = calculateEntropy(rightClassCounts, numClasses);
	//cout << "left and right" << leftEntropy << "  " << rightEntropy << endl;
	leftProb =(double)leftSize / numSamples;
	rightProb = (double)rightSize / numSamples;
	//cout << "left and right size" << leftProb << "  " << rightProb << endl;
	splitEntropy = (double)leftProb * leftEntropy + rightProb * rightEntropy;
	delete[] classCounts;
	delete[] leftClassCounts;
	delete[] rightClassCounts;
	//cout << "initialEntropy=" << initialEntropy<<endl;
	//cout << "split=" << splitEntropy << endl;
	return initialEntropy - splitEntropy;
}

void DecisionTree::train(const string fileName, const int numSamples, const int numFeatures) {
	//cout << " in train 1" << endl;
	ifstream file;
	string line;		
	file.open(fileName);	

	int* labels = new int[numSamples];

	bool** temp = new bool*[numSamples];	
	for (int i = 0; i < numSamples; i++)
	{
		temp[i] = new bool[numFeatures];
	}
	int index = 0;
	while (!(file.eof()))		
	{
		int index2 = 0;
		int position = 0;
		getline(file, line);	
		while (position = line.find(" ") != string::npos) {	
			string selected = line.substr(0, position);
			temp[index][index2] = stoi(selected); 
			line.erase(0, position + 1);
			index2++;
		}
		if (index != numSamples)
			labels[index] = stoi(line);	
		
		index++;
		
	}

	file.close();

	const bool* const* data = temp;

	train((const bool**)data, labels, numSamples, numFeatures);
	delete[] labels;
	for (size_t i = 0; i < numSamples; i++)
	{
		//delete[] data[i];
		delete[] temp[i];
	}
	delete[] temp;

}
void DecisionTree::train(const bool** data, const int* labels, const int numSamples, const int numFeatures) {
	//cout << " in train 2" << endl;
	bool* usedSamples;
	bool* usedFeatures;
	usedSamples = new bool[numSamples];
	usedFeatures = new bool[numFeatures];

	for (int i = 0; i < numFeatures; i++) {
		usedFeatures[i] = 0; ////////////////de?i?tir
	}

	for (int i = 0; i < numSamples; i++) {
		usedSamples[i] = 0; //////////////////////////////de?i?tir
	}
	root = new DecisionTreeNode();
	train(data, labels, numSamples, numFeatures, root, usedFeatures, usedSamples);
	delete[] usedFeatures;
	delete[] usedSamples;
}
void DecisionTree::train(const bool** data, const int* labels, const int numSamples, const int numFeatures, DecisionTreeNode *node, bool* usedFeatures, bool* usedSamples) {
	//First check if the split is homogen or not if homogen return leafnode.
	//cout << " in train 3" << endl;
	bool isHomogenous = true;
	int firstClass = -1;
	for (int i = 0; i < numSamples && isHomogenous; i++) {
		if (!usedSamples[i]) {
			if (firstClass == -1) {
				firstClass = labels[i];
			}
			else {
				if (firstClass != labels[i]) {
					isHomogenous = false;
				}
			}
		}
	}
	if (isHomogenous && firstClass != -1) {
		node->setId(firstClass);
		node->setLeftChild(nullptr);
		node->setRightChild(nullptr);
		cout << " class" << endl;
		return;
	}

	//Secondly check if there are no features left to use. if so create a leaf node based on the majority.
	bool noFeaturesLeft = true;
	for (int i = 0; i < numFeatures; i++) {
		if (!usedFeatures[i])
			noFeaturesLeft = false;
	}
	if (noFeaturesLeft) {
		cout << " nofeature" << endl;
		int numClasses = 0;
		for (int i = 0; i < numSamples; i++) {
			if (!usedSamples[i]) {
				if (numClasses < labels[i]) {
					numClasses = labels[i];
				}
			}
		}

		int* classCounts = new int[numClasses];

		for (int i = 0; i < numClasses; i++) {
			classCounts[i] = 0;
		}
		for (int i = 0; i < numSamples; i++) {
			if (!usedSamples[i]) {
				classCounts[labels[i] - 1]++;
			}
		}
		int classId = 0;
		for (int i = 0; i < numClasses; i++) {
			if (classCounts[classId] < classCounts[i])
				classId = i;
		}
		classId++;
		node->setId(classId);
		node->setLeftChild(nullptr);
		node->setRightChild(nullptr);
		delete[] classCounts;
		return;
	}
	double bestGain = 0;
	
	int selectedFeature = -1;
	for (size_t  i = 0; i < numFeatures; i++) {
		if (!usedFeatures[i]) {

			double currentGain = calculateInformationGain(data, labels, numSamples, numFeatures, usedSamples, i);
		//	cout << currentGain << endl;
			if (currentGain > bestGain) {
				bestGain = currentGain;
				selectedFeature = i;
			}
			//cout << "bestgain=" << bestGain << endl;
		}
	}
	//cout << "selectedFeatrue=" << selectedFeature << endl;
	usedFeatures[selectedFeature] = true;

	bool* usedSamplesLeft = new bool[numSamples];
	bool* usedSamplesRight = new bool[numSamples];

	for (int i = 0; i < numSamples; i++) {
		if (usedSamples[i]) {
			usedSamplesLeft[i] = true;
			usedSamplesRight[i] = true;
		}
		else {
			if (data[i][selectedFeature] == 0) {
				usedSamplesLeft[i] = false;
				usedSamplesRight[i] = true;
			}
			else {
				usedSamplesLeft[i] = true;
				usedSamplesRight[i] = false;
			}
		}

	}
	node->setId(selectedFeature);
	node->setLeftChild(new DecisionTreeNode());
	node->setRightChild(new DecisionTreeNode());
	//cout <<selectedFeature << endl;
	train(data, labels, numSamples, numFeatures, node->getLeftChild(), usedFeatures, usedSamplesLeft);
	train(data, labels, numSamples, numFeatures, node->getRightChild(), usedFeatures, usedSamplesRight);
	delete[] usedSamplesLeft;
	delete[] usedSamplesRight;
	//cout << "keep go" << endl;
}
int DecisionTree::predict(const bool* sample) {
	DecisionTreeNode* node = root;
	while (node->getLeftChild() != nullptr && node->getRightChild() != nullptr) {
		if (!sample[node->getId()])
			node = node->getLeftChild();
		else
			node = node->getRightChild();

	}
	return node->getId();

}
double DecisionTree::test(const bool** data, const int* labels, const int numSamples) {
	int trueGuesses = 0;
	for (int i = 0; i < numSamples; i++) {
		if (labels[i] == predict(data[i])) {
			trueGuesses++;
		}
	}
	cout << "truees= " << trueGuesses << endl;
	return (double)trueGuesses / numSamples;
}
double DecisionTree::test(const string fileName, const int numSamples) {
	
	ifstream file;
	string line;
	file.open(fileName);
	streampos begin = file.tellg(); 
	string firstline;
	getline(file, firstline);	
	file.seekg(begin);	
	int numFeatures = 0;	
	while (firstline.find(" ") != string::npos) {
		firstline.erase(0, firstline.find(" ") + 1);
		numFeatures++;
	}
	bool** temp = new bool*[numSamples];
	for (int i = 0; i < numSamples; i++)
	{
		temp[i] = new bool[numFeatures];	// create array in feature size(j)
	}
	int* labels = new int[numSamples];

	
	int index = 0;
	while (!(file.eof()))
	{
		getline(file, line);
		int position = 0;
		int index2 = 0;
		while (position = line.find(" ") != string::npos) {
			string selected = line.substr(0, position);
			temp[index][index2] = stoi(selected);
			line.erase(0, position + 1);
			index2++;
		}
		if (index != numSamples)
			labels[index] = stoi(line);
		index++;
	}
	const bool* const* data = temp;

	file.close();

	double accuracy = test((const bool**)data, labels, numSamples);

	for (int i = 0; i < numSamples; i++)
	{
		delete[] data[i];
	}
	delete[] data;
	delete[] labels;
	return accuracy;
}

void DecisionTree::print() {
	print(root, 0);
}
DecisionTree::DecisionTree() {
	root = NULL;
}
DecisionTree::~DecisionTree() {
	delete root;
}
void DecisionTree::print(DecisionTreeNode* node, int level) {
	if (node == nullptr) {
		return;
	}
	for (int i = 0; i < level; i++) {
		cout << "\t";
	}
	if (node->getLeftChild() == nullptr && node->getRightChild() == nullptr) {
		cout << "class=" << node->getId() << endl;
	}
	else {
		cout << node->getId() << endl;
	}
	print(node->getLeftChild(), level + 1);
	print(node->getRightChild(), level + 1);
}