#ifndef BST_H
#define	BST_H
#include "BSTInterface.h"

#include <string>
#include <ostream>
#include <sstream>
#include <algorithm>

using namespace std;

template<typename T>
class BST : public BSTInterface<T>
{

private:
	//the node struct
	struct Node
	{
		T data;
		Node* leftChild;
		Node* rightChild;
		Node* parent;

		//Node constructor
			Node(const T& d, Node* left = nullptr, Node* right = nullptr, Node* newParent = nullptr) : data(d), leftChild(left), rightChild(right), parent(newParent) {}
		string toString() const
		{
			std::stringstream oss;
			oss << data << " ";
			return oss.str();
		}

	};

	Node* root;

	//recursive helper function
	string print(Node* referenceNode, int desiredLine, int lineCounter = 1) const {
		if (lineCounter == desiredLine) {
			if (referenceNode == nullptr) {
				return "_ ";
			}
			return referenceNode->toString();
		}
		else {
			if (referenceNode == nullptr) {
				return "";
			}
			if (referenceNode->leftChild != nullptr || referenceNode->rightChild != nullptr) {
				return print(referenceNode->leftChild, desiredLine, lineCounter + 1) + print(referenceNode->rightChild, desiredLine, lineCounter + 1);
			}
		}
		return "";
	}

	//recursive helper function
	string lineCounter() const {
		string result = "";
		int i = 1;
		do {
			result = result + "\t" + to_string(i) + ": ";
			result += print(root, i);
			result += "\n";
			i++;
		} while (print(root, i).length() > 0);

		return result;
	}

	//recursive helper function
	int size(Node* referenceNode) {
		if (referenceNode == nullptr) {
			return 0;
		}
		return 1 + size(referenceNode->leftChild) + size(referenceNode->rightChild);
	}

	//recursive helper function
	bool clearTree(Node* referenceNode) {
		if (referenceNode == nullptr) {
			return true;
		}
		if (clearTree(referenceNode->leftChild) && clearTree(referenceNode->rightChild)) {
			delete referenceNode;
			referenceNode = nullptr;
			return true;
		}
		return false;
	}

	//swap helper function
	void swap(Node* r, Node* s)
	{
		Node* tmp = new Node(*r);
		r->data = s->data;
		s->data = tmp->data;
		delete tmp;
		return;
	}

	//recursive helper function
	Node* rightmost(Node* referenceNode) {
		if (referenceNode->rightChild == nullptr) {
			return referenceNode;
		}
		return rightmost(referenceNode->rightChild);
	}

	//recursive helper function
	bool removeNode(const T& data, Node* referenceNode) {
		if (referenceNode == nullptr) {
			return false;
		}
		if (data > referenceNode->data) {
			return removeNode(data, referenceNode->rightChild);
		}
		if (data < referenceNode->data) {
			return removeNode(data, referenceNode->leftChild);
		}
		if (data == referenceNode->data) {
			if (referenceNode->leftChild == nullptr) {
				if (referenceNode->rightChild == nullptr) {
					Node* tmp3 = new Node(*referenceNode);
					if (referenceNode->data > referenceNode->parent->data)
						referenceNode->parent->rightChild = nullptr;
					if (referenceNode->data < referenceNode->parent->data)
						referenceNode->parent->leftChild = nullptr;
					delete tmp3;
					return true;
				}
				Node* tmp = new Node(*referenceNode);
				if (referenceNode->data > referenceNode->parent->data) {
					referenceNode->rightChild->parent = referenceNode->parent;
					referenceNode->parent->rightChild = referenceNode->rightChild;
				}
				else if (referenceNode->data < referenceNode->parent->data) {
					referenceNode->rightChild->parent = referenceNode->parent;
					referenceNode->parent->leftChild = referenceNode->rightChild;
				}
				delete tmp;
				return true;
			}
			else {
				swap(referenceNode, rightmost(referenceNode->leftChild));

				if (referenceNode->leftChild->data == data) {
					Node* tmp2 = new Node(*referenceNode->leftChild);
					referenceNode->leftChild->leftChild->parent = referenceNode;
					referenceNode->leftChild = referenceNode->leftChild->leftChild;
					delete tmp2;
					return true;
				}
				return postSwapFix(referenceNode->leftChild, data);
			}
		}
		return false;
	}

	//recursive helper function
	bool postSwapFix(Node* referenceNode, const T& data) {
		if (referenceNode->rightChild->data == data) {
			Node* tmp = new Node(*referenceNode->rightChild);
			referenceNode->rightChild->leftChild->parent = referenceNode;
			referenceNode->rightChild = referenceNode->rightChild->leftChild;
			delete tmp;
			return true;
		}
		return postSwapFix(referenceNode->rightChild, data);
	}

	//recursive helper function
	bool find(const T& data, Node* referenceNode) {
		if (referenceNode == nullptr) {
			return false;
		}
		if (referenceNode->data == data) {
			return true;
		}
		if (data > referenceNode->data) {
			return find(data, referenceNode->rightChild);
		}
		if (data < referenceNode->data) {
			return find(data, referenceNode->leftChild);
		}
	}


public:
	BST()
	{
		this->root = NULL;
	}
	~BST() { clearTree(); }

	/** Return true if node added to BST, else false */
	virtual bool addNode(const T& data) {
		if (root == NULL) {
			root = new Node(data);
			return true;
		}
		Node* tmp = root;

		do {
			if (tmp->data == data) {
				return false;
			}
			if (data > tmp->data) {
				if (tmp->rightChild == nullptr) {
					tmp->rightChild = new Node(data, nullptr, nullptr, tmp);
					return true;
				}
				else {
					tmp = tmp->rightChild;
				}
			}
			else {
				if (tmp->leftChild == nullptr) {
					tmp->leftChild = new Node(data, nullptr, nullptr, tmp);
					return true;
				}
				else {
					tmp = tmp->leftChild;
				}
			}
		} while (true);

		return false;
	}

	/** Return true if node removed from BST, else false */
	virtual bool removeNode(const T& data) {
		if (root == nullptr) {
			return false;
		}
		if (root->data == data) {
			if (root->leftChild == nullptr) {
				if (root->rightChild == nullptr) {
					clearTree();
					return true;
				}
				Node* tmp = new Node(*root);
				root = root->rightChild;
				delete tmp;
				return true;
			}
			else {
				swap(root, rightmost(root->leftChild));
				
				if (root->leftChild->data == data) {
					Node* tmp2 = new Node(*root->leftChild);
					if (root->leftChild->leftChild == nullptr) {
						root->leftChild = nullptr;
					}
					else {
						root->leftChild->leftChild->parent = root;
						root->leftChild = root->leftChild->leftChild;
					}
					delete tmp2;
					return true;
				}
				return postSwapFix(root->leftChild, data);
			}
		}
		return removeNode(data, root);
	}

	/** Return true if BST cleared of all nodes, else false */
	virtual bool clearTree() {
		bool didItWork = clearTree(root);
		root = nullptr;
		return didItWork;
	}

	/** Return a level order traversal of a BST as a string */
	virtual std::string toString() const {
		if (root == NULL) {
			return " empty \n";
		}
		const string result ="\n" + lineCounter();
		return result;
	}

	//function to start the recursion find function
	string find(const T& data) {
		if (find(data, root)) {
			return "found";
		}
		return "not found";
	}

	//function to start the recursion size function
	int size() {
		return size(root);
	}
};


#endif
