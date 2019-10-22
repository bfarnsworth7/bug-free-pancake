#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <fstream>
#include "SetInterface.h"
#include "LinkedList.h"
using namespace std;

template <typename T>
class Set : public SetInterface<T> {
private:
	LinkedList<T> setList;

	bool compare(const T newItem, string oldItem) {
		if (newItem == oldItem) {
			return false;
		}
		if (newItem > oldItem) {
			return true;
		}
		return false;
	}
public:
	Set() {}

	virtual bool insert(const T& item) {
		string compare;
		if (setList.size() == 0) {
			setList.insertHead(item);
			return true;
		}
		for (int i = setList.size() - 1; i >= 1; i--) {
			setList.at(i, compare);
			if (Set<T>::compare(item, compare)) {
				setList.insertAfter(compare, item);
				return true;
			}
		}
		setList.at(0, compare);
		if (Set<T>::compare(item, compare)) {
			setList.insertAfter(compare, item);
			return true;
		}
		else {
			setList.insertHead(item);
			return true;
		}
		return false;
	}

	/** Removes all items from the set. */
	virtual void clear() {
		setList.clear();
		return;
	}

	/** @return: the number of elements contained by the Set. */
	virtual size_t size() const {
		return setList.size();
	}

	/** @return: return 1 if contains element equivalent to item, else 0. */
	virtual size_t count(const T& item) {
		for (int i = 0; i < setList.size(); i++) {
			string tmp;
			setList.at(i, tmp);
			if (tmp == item) {
				return 1;
			}
		}
		return 0;
	}

	/** @return: string representation of items in Set. */
	virtual string toString() const {
		string result = "\t[";
		string comparer = setList.toString();
		for (size_t i = 0; i < comparer.size(); i++) {
			if (comparer[i] != ' ') {
				result += comparer[i];
			}
			else result += ',';
		}
		result += "]\n";
		return result;
	}

	LinkedList<T> getSetList() {
		return setList;
	}

	friend ostream& operator<< (ostream& os, Set<T>& mySet) {
		string result = "";
		string comparer = mySet.getSetList().toString();
		for (int i = 0; i < comparer.size(); i++) {
			if (comparer[i] != ' ') {
				result += comparer[i];
			}
			else result += ',';
		}
		os << result;
		return os;
	}

};
