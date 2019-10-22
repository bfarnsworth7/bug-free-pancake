#ifndef LINKEDNODE_H
#define LINKEDNODE_H
using namespace std;
#include <iostream>
#include <sstream>

template<typename T>
class LinkedNode {
private:
	T value;
	LinkedNode<T>* nextValue;
public:
	/** Constructor*/
	LinkedNode(T newValue, LinkedNode<T>* newNextValue) {
		value = newValue;
		nextValue = newNextValue;
	}

	/** Sets the value stored in the current node*/
	void setValue(T newValue) {
		value = newValue;
	}

	/** Sets a pointer to the next value in a linked list*/
	void setNextValue(LinkedNode<T>* newNextValue) {
		nextValue = newNextValue;
	}

	/** Returns the value stored in the current node*/
	T getValue() {
		return value;
	}

	/** Returns a pointer to the next value in a linked list*/
	LinkedNode<T>* getNextValue() {
		return nextValue;
	}

	/** Returns a single node as a string */
	string toString() {
		stringstream output;
		output << value;
		return output.str();
	}

	/** Grants friendship to the << insertion operator*/
	friend ostream& operator<< (ostream& os, const LinkedNode<T>& myNode) {
		os << myNode.toString();
		return os;
	};
};
#endif