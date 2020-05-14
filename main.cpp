#include <iostream>
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
int main() {
	DecisionTree tree;
	tree.train("train_data.txt", 498, 21);
	cout << "Accuracy of the test: " << tree.test("test_data.txt", 473) << endl;
	tree.print();
	return 0;
}
