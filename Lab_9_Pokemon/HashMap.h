#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <fstream>
#include "HashMapInterface.h"
using namespace std;

template <typename KEY, typename VALUE>
class hash_map : public HashMapInterface<KEY, VALUE>{
private:
	size_t capacity, size2;
	pair<KEY, VALUE>* myPairs;
public:
	hash_map<KEY, VALUE>() : myPairs(new pair<KEY, VALUE>[31]), capacity(31), size2(0) {}

	virtual VALUE& operator[](const KEY& key) {
		size_t hash = key[0];
		for (size_t i = 1; i < key.length(); ++i) {
			hash = hash * 3 + key[i];
		}
		hash %= capacity;
		cout << endl << "Key=" << key << ", hash=" << hash;
		int k = 1;
		while (1) {
			if (myPairs[hash].first.length()) {
				if (myPairs[hash].first == key) break;
				hash = (hash + k) % capacity;
				k += 2;
				continue;
			}
			myPairs[hash].first = key;
			size2++;
			break;
		}
		return myPairs[hash].second;
	}

	virtual string toString() const {
		ostringstream os;
		for (size_t i = 0; i < capacity; ++i) {
			if (myPairs[i].first.length()) {
				os << endl << "\t[" << i << ":";
				os << myPairs[i].first << "->" << myPairs[i].second << "]";
			}
		}
		return os.str();
	}

	/** Read/write index access operator.
If the key is not found, an entry is made for it.
@return: Read and write access to the value mapped to the provided key. */

	/** @return: the number of elements that match the key in the Map. */
	virtual size_t count(const KEY& key) {
		return 0;
	}

	/** Removes all items from the Map. */
	virtual void clear(void) {
		return;
	}

	/** @return: number of Key-Value pairs stored in the Map. */
	virtual size_t size(void) const {
		return size2;
	}

	/** @return: maximum number of Key-Value pairs that the Map can hold. */
	virtual size_t max_size(void) const {
		return capacity;
	}
};