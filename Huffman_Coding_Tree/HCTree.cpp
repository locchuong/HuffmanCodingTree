/** Filename: HCTree.cpp
 *  Name: Loc Chuong
 *  Description: A Huffman Code Tree class. This implentation is not generic
 *               since it only uses the alphabet consisting of unsigned chars.
 *  Date: 5/11/2019
 */

#include "HCTree.hpp"

 /** Function Name: build(const vector<int>& freqs)
  *  Description: Uses the Huffman algorithmn to build a Huffman coding trie
  *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is the
  *                frequency of occurence of byte i in the message.
  *  POSTCONDITION: root points to the root of the trie, and leaves[i] points
  *                 to the leaf node containing byte i.
  *  Parameters: freqs - A vector of ints s.t. freqs[i] is the frequency of
  *                      occurence of byte i in the message
  *  Return Value: None
  */
void HCTree::build(const vector<int>& freqs) {
	/** Create priority queue to sort HCNode and use Huffman's algorithm */
	priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
	/** Set vector of leaves in calling HCTree */
	for (int i = 0; i < freqs.size(); i++) {
		if (freqs[i] != 0) {
			this->leaves[i] = new HCNode(freqs[i], i);
		}
		else {
			this->leaves[i] = nullptr;
		}
	}
	/** Transfer contents of vector of leaves to priority queue */
	for (int i = 0; i < freqs.size(); i++) {
		if (freqs[i] != 0) {
			pq.push(this->leaves[i]);
		}
	}
	if (pq.size() == 1) {
		this->root = pq.top();
	}
	/** Perform Huffman's algorithm */
	while (pq.size() != 1) {
		/** Grab count of lowest element */
		int count = pq.top()->count;
		/** Create pointer to lowest element */
		HCNode* p1 = pq.top();
		/** Pop lowest element from priority_queue */
		pq.pop();
		/** Grab count of 2nd lowest element and add to lowest element */
		count += pq.top()->count;
		/** Create pointer to 2nd lowest element */
		HCNode* p2 = pq.top();
		/** Pop 2nd lowest element from priority_queue */
		pq.pop();
		/** Create pair from lowest and 2nd lowest element */
		HCNode* parPtr = new HCNode(count, 0, p1, p2);
		/** Connect children to parent */
		p1->p = parPtr;
		p2->p = parPtr;
		/** Push parent of lowest and 2nd lowest element to */
		pq.push(parPtr);
	}
	/** Set root of calling HCTree */
	this->root = pq.top();
	/** Pop last element from priority queue */
	pq.pop();
}

/** Function Name: encode(byte symbol, BitOutputStream& out) const
 *  Description: Write to the given BitOutputStream the sequence of
 *               bits coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding
 *                tree, and initialize root pointer and leaves vector.
 *  Parameters: symbol - Used to write to BitOutputStream
 *              out - Stream to write to
 *  Return Value: None
 */
void HCTree::encode(byte symbol, BitOutputStream& out) const {
	/** Create pointer to leaf node */
	HCNode* currNode = this->leaves[symbol];
	/** Write 0s if HCTree only has one node */
	if (currNode != nullptr && currNode->p == nullptr
		&& currNode->c0 == nullptr && currNode->c1 == nullptr) {
		out.writeBit(0);
	}
	/** Holds num of times node traverses */
	int traverseNum = 0;
	/** Holds the encoded symbol */
	int encoded = 0;
	/** Traverse to parent if possible */
	if (currNode) {
		while (currNode->p != nullptr) {
			/** Checks if the currNode's parent's c0 child is this node */
			if (currNode->p->c0 != nullptr && currNode->p->c0 == currNode) {
				/** Set 0 at the appropriate index in encode */
				encoded &= ~(1 << traverseNum);
			}
			/** Checks if the currNode's parent's c1 child is this node */
			if (currNode->p->c1 != nullptr && currNode->p->c1 == currNode) {
				/** Set 0 at the appropriate index in encode */
				encoded |= (1 << traverseNum);
			}
			/** Increment number of traversals */
			traverseNum++;
			/** Traverse to parent node */
			currNode = currNode->p;
		}
	}
	/** Write the appropriate bit to the stream */
	for (int i = 0; i < traverseNum; i++) {
		out.writeBit(encoded >> (traverseNum - i - 1));
	}
}

/** Function Name: encode(byte symbol, ofstream& out) const
 *  Description: Write to the given ofstream the sequence of bits (as ASCII)
 *               coding the given symbol.
 *  PRECONDITION: build() has been called, to create the coding tree, and
 *                initialize root pointer and leaves vector.
 *  Parameters: symbol - Represents a sequence of bits representing a symbol
 *                       (as ASCII)
 *              out - The ofstream the sequence of bits will be writen to
 *  Return Value: None
 */
void HCTree::encode(byte symbol, ofstream& out) const {
	/** Create pointer to leaf node */
	HCNode* currNode = this->leaves[symbol];
	/** Create buffer to hold encoded message */
	string output = string();
	/** Handles case where the currNode is the only node in the Trie */
	if (currNode != nullptr && currNode->p == nullptr) {
		/** Append 0 to beginning of output string */
		output = "0" + output;
	}
	if (currNode) {
		while (currNode->p != nullptr) {
			/** Check if currNode is the left child of its parent*/
			if (currNode->p->c0 != nullptr && currNode->p->c0 == currNode) {
				/** Append 0 to beginning of output string */
				output = "0" + output;
			}
			/** Check if currNode is the right child of its parent*/
			if (currNode->p->c1 != nullptr && currNode->p->c1 == currNode) {
				/** Append 1 to beginning of output string */
				output = "1" + output;
			}
			/** Traverse up the Trie */
			currNode = currNode->p;
		}
	}
	/** Output encoded message to ofstream */
	out << output;
}

/** Function Name: decode(BitInputStream& in) const
 *  Description: Return symbol coded in the next sequence of bits from
 *               the stream.
 *  PRECONDITION: build() has been called, to create the coding
 *                tree, and initialize root pointer and leaves vector.
 *  Parameters: in - the BitInputStream object to read from
 *  Return Value: Symbol coded in the next sequence of bits from the
 *                stream
 */

int HCTree::decode(BitInputStream& in) const {
	/** Holds next bit read from BitInputStream */
	int nextBit = 0;
	/** Create pointer to root node */
	HCNode* currNode = root;
	/** Handles HCTree with only one node */
	if (currNode->p == nullptr && currNode->c0 == nullptr
		&& currNode->c1 == nullptr && currNode != nullptr) {
		return currNode->symbol;
	}
	/** Checks if currNode still has children to traverse */
	if (currNode) {
		while (currNode->c0 != nullptr && currNode->c1 != nullptr) {
			/** Read the next bit from the stream */
			nextBit = in.readBits();
			/** Check if the c0 child exists and the next bit read is 0 */
			if (currNode->c0 != nullptr && nextBit == 0) {
				/** Traverse to c0 child */
				currNode = currNode->c0;
			}
			/** Check if the c1 child exists and the next bit read is 1 */
			else if (currNode->c1 != nullptr && nextBit == 1) {
				/** Traverse to c1 child */
				currNode = currNode->c1;
			}
			/** Current node has no children */
			if (currNode->c0 == nullptr && currNode->c1 == nullptr) {
				return currNode->symbol;
			}
		}
	}
	return -1;
}

/** Function Name: decode(ifstream& in) const
 *  Description: Return the symbol coded in the next sequence of bits
 *               (represented as ASCII text) from the ifstream
 *  PRECONDITION: build() has been called, to create the coding tree, and
 *                initialize root pointer and leaves vector.
 *  Return Value: An int representing the ASCII value of a symbol
 */
int HCTree::decode(ifstream& in) const {
	/** Create unsigned char to hold next bit in last line of ifstream */
	unsigned char nextChar;
	/** Pointer to root node */
	HCNode* currNode = root;
	/** Keep grabbing bits from last line of ifstream until exit condition */
	while (1) {
		/** Grab next character from last line of ifstream */
		nextChar = (unsigned char)in.get();
		/** Break out of loop if end-of-file reached */
		if (in.eof()) break;
		/** Traverse to c0 child */
		if (currNode->c0 != nullptr && nextChar == '0') {
			currNode = currNode->c0;
		}
		/** Traverse to c1 child */
		else if (currNode->c1 != nullptr && nextChar == '1') {
			currNode = currNode->c1;
		}
		/** On a node that has no children, return the current node's symbol */
		if (currNode->c0 == nullptr && currNode->c1 == nullptr) {
			return currNode->symbol;
		}
	}
	/** EOF reached */
	return -1;
}

/** Function Name: deleteNodes(HCNode * currNode)
 *  Description: Deletes the HCTree from the root node by recursion
 *  Parameters: currNode - Pointer to the root node
 *  Return Value: None
 */
void HCTree::deleteNodes(HCNode* currNode) {
	/** Traverse down the tree if the current node is not nullptr */
	if (currNode != nullptr) {
		/** Traverse to c0 child */
		deleteNodes(currNode->c0);
		/** Traverse to c1 child */
		deleteNodes(currNode->c1);
		/** Delete the current node */
		delete currNode;
	}
}

/** Destructor
 *  Description: Deallocates HCTree and all its instance variables
 */
HCTree::~HCTree() {
	deleteNodes(root);
}
