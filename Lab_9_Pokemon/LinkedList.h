#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "LinkedListInterface.h"
#include "LinkedNode.h"
#include <typeinfo>
using namespace std;

template<typename T>
class LinkedList : public LinkedListInterface<T> {
private:
	LinkedNode<T> *firstNode = NULL;
public:
	LinkedList() {	   // constructor

	}
	~LinkedList() {    // destructor

	}

	/** Insert Node at beginning of linked list (no duplicates) */
	virtual bool insertHead(T value) {
		if (firstNode == NULL) {
			LinkedNode<T>* tmp = firstNode;
			firstNode = new LinkedNode<T>(value, tmp);
			return true;
		}
		LinkedNode<T> *lastNode = firstNode;
		while (lastNode->getNextValue() != NULL) {
			lastNode = lastNode->getNextValue();
			if (lastNode->getValue() == value) {
				return false;
			}
		}
		if (firstNode->getValue() != value) {
			LinkedNode<T>* tmp = firstNode;
			firstNode = new LinkedNode<T>(value, tmp);
			return true;
		}
		return false;
	}

	/** Insert Node at end of linked list (no duplicates) */
	virtual bool insertTail(T value) {
		LinkedNode<T> *lastNode = firstNode;
		if (firstNode == NULL) {
			LinkedNode<T>* tmp = firstNode;
			firstNode = new LinkedNode<T>(value, tmp);
			return true;
		}
		LinkedNode<T> *tester = firstNode;
		while (tester->getNextValue() != NULL) {
			tester = tester->getNextValue();
			if (tester->getValue() == value) {
				return false;
			}
		}
		while (lastNode->getNextValue() != NULL) {
			lastNode = lastNode->getNextValue();
		}
		if (lastNode->getValue() != value) {
			LinkedNode<T>* newNode = new LinkedNode<T>(value, lastNode->getNextValue());
			lastNode->setNextValue(newNode);
			return true;
		}
		return false;
	}

	/** Insert node after matchNode (no duplicates) */
	virtual bool insertAfter(T matchNode, T node) {
		LinkedNode<T> *tester = firstNode;
		while (tester->getNextValue() != NULL) {
			tester = tester->getNextValue();
			if (tester->getValue() == node) {
				return false;
			}
		}
		if (tester->getValue() == node) {
			return false;
		}
		LinkedNode<T> *lastNode = firstNode;
		while (lastNode->getValue() != matchNode && lastNode->getNextValue() != NULL) {
			lastNode = lastNode->getNextValue();
		}
		if (lastNode->getNextValue() != NULL && lastNode->getNextValue()->getValue() == node) {
			return false;
		}
		if (lastNode->getNextValue() != NULL || lastNode->getValue() == matchNode) {
			LinkedNode<T>* newNode = new LinkedNode<T>(node, lastNode->getNextValue());
			lastNode->setNextValue(newNode);
			return true;
		}
		return false;
	}

	/** Remove Node from linked list */
	virtual bool remove(T value) {
		if (firstNode == NULL) {
			return false;
		}
		LinkedNode<T> *lastNode = firstNode;
		if (lastNode->getValue() == value) {
			LinkedNode<T> *tmp2 = lastNode->getNextValue();
			delete lastNode;
			firstNode = tmp2;
			return true;
		}
		while (lastNode->getNextValue()->getValue() != value) {
			lastNode = lastNode->getNextValue();
			if (lastNode->getNextValue()->getNextValue() == NULL && lastNode->getNextValue()->getValue() != value) {
				return false;
			}
		}
		LinkedNode<T> *tmp = lastNode->getNextValue();
		lastNode->setNextValue(lastNode->getNextValue()->getNextValue());
		delete tmp;
		return true;
	}

	/** Remove all Nodes from linked list */
	virtual bool clear() {
		if (firstNode == NULL) {
			return true;
		}
		LinkedNode<T> *lastNode = firstNode;
		LinkedNode<T> *tmp = firstNode->getNextValue();
		while (lastNode->getNextValue() != NULL) {
			delete lastNode;
			lastNode = tmp;
			tmp = lastNode->getNextValue();
		}
		firstNode = NULL;
		delete tmp;
		delete lastNode;
		return true;
	}

	/** Return Node at index (0 based) via value parameter
	NOTE: STL list does NOT support the at accessor. */
	virtual bool at(size_t index, std::string& value) {
		if (firstNode == NULL) {
			value = "Invalid Index";
			return false;
		}
		size_t counter = 0;
		LinkedNode<T> *lastNode = firstNode;
		while (counter < index) {
			if (lastNode->getNextValue() == NULL) {
				value = "Invalid Index";
				return false;
			}
			lastNode = lastNode->getNextValue();
			counter++;
		}
		value = lastNode->toString();
		return true;
	}

	/** Returns the number of nodes in the linked list */
	virtual size_t size() const {
		size_t counter = 0;
		LinkedNode<T> *lastNode = firstNode;
		if (firstNode == NULL) {
			return counter;
		}
		while (lastNode->getNextValue() != NULL) {
			lastNode = lastNode->getNextValue();
			counter++;
		}
		counter++;
		return counter;
	}

	/** Return a level order traversal of a Linked List as a string */
	virtual std::string toString() const {
		string result = "";
		LinkedNode<T> *lastNode = firstNode;
		if (firstNode == NULL) {
			result = result + "Empty";
			return result;
		}
		while (lastNode->getNextValue() != NULL) {
			result = result + lastNode->toString() + " ";
			lastNode = lastNode->getNextValue();
		}
		result = result + lastNode->toString();
		return result;
	}

	/** Grants friendship to the << insertion operator*/
	friend ostream& operator<< (ostream& os, const LinkedList<T>& myList) {
		os << myList.toString();
		return os;
	};
};
#endif