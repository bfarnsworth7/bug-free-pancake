//**** YOU MAY NOT MODIFY THIS DOCUMENT ****/
#ifndef LINKED_LIST_INTERFACE_H
#define LINKED_LIST_INTERFACE_H
#include <string>

template<typename T>
class LinkedListInterface
{
public:
	LinkedListInterface(void) {};
	virtual ~LinkedListInterface(void) {};

	/** Insert Node at beginning of linked list (no duplicates) */
	virtual bool insertHead(T value) = 0;

	/** Insert Node at end of linked list (no duplicates) */
	virtual bool insertTail(T value) = 0;

	/** Insert node after matchNode (no duplicates) */
	virtual bool insertAfter(T matchNode, T node) = 0;

	/** Remove Node from linked list */
	virtual bool remove(T value) = 0;

	/** Remove all Nodes from linked list */
	virtual bool clear() = 0;

	/** Return Node at index (0 based) via value parameter
	NOTE: STL list does NOT support the at accessor. */
	virtual bool at(size_t index, std::string& value) = 0;

	/** Returns the number of nodes in the linked list */
	virtual size_t size() const = 0;

	/** Return a level order traversal of a Linked List as a string */
	virtual std::string toString() const = 0;
};
#endif	// LINKED_LIST_INTERFACE_H