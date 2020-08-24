/** Filename: HCTree.hpp
 *  Name: Loc Chuong
 *  Description: A Huffman Code Tree class. This implentation is not generic
 *               since it only uses the alphabet consisting of unsigned chars.
 *  Date: 5/12/2019
 */

#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <stack>
#include <fstream>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

#define ASCII_MAX 256 /** Max number of ASCII values */
#define BIT_SIZE_UNIQ_ASCII 9 /** Num of bits to store all unique ASCII val */
#define BIT_SIZE_BIT_FREQS 5 /** Num of bits to store bits storing freq */

using namespace std;

/** Class Name: HCNodePtrComp
 *  Description: A 'function class' for use as the Compare class in a
 *               priority_queue<HCNode*>. For this to work, operator< must be
 *               defined to do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
	/** operator()
	 *  Description: Compares lhs and rhs such that the HCNode with higher
	 *               priority will be first in the priority_queue
	 *  Return Value: True if rhs has higher priority than lhs, false
	 *                otherwise
	 */
	bool operator()(HCNode*& lhs, HCNode*& rhs) const {
		return *lhs < *rhs;
	}
};

/** Class Name: HCTree
 *  Description: A Huffman Code Tree class. Not very generic:  Use only if
 *               alphabet consists of unsigned chars.
 */
class HCTree {
private:
	HCNode* root; /** Pointer to the root HCNode in the HCTree */

public:
	vector<HCNode*> leaves; /** Vector of pointers to leaves of HCTree */
	/** Constructor
	 *  Description: Creates a HCTree with no root and a set leaves vector
	 *               of size 256(MAX ASCII Chars)
	 */
	explicit HCTree() : root(0) {
		leaves = vector<HCNode*>(256, (HCNode*)0);
	}

	/** Destructor
	 *  Description: Deletes the HCTree.
	 */
	~HCTree();

	/** Function Name: deleteNodes(HCNode * currNode)
	 *  Description: Deletes the HCTree from the root node by recursion
	 *  Parameters: currNode - Pointer to the root node
	 *  Return Value: None
	 */
	void deleteNodes(HCNode* currNode);

	/** Function Name: build(const vector<int>& freqs)
	 *  Description: Uses the Huffman algorithmn to build a Huffman coding
	 *               trie
	 *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is the
	 *                frequency of occurence of byte i in the message.
	 *  POSTCONDITION: root points to the root of the trie, and leaves[i]
	 *                 points to the leaf node containing byte i.
	 *  Parameters: freqs - A vector of ints s.t. freqs[i] is the frequency
	 *                      of occurence of byte i in the message
	 *  Return Value: None
	 */
	void build(const vector<int>& freqs);

	/** Function Name: encode(byte symbol, BitOutputStream& out) const
	 *  Description: Write to the given BitOutputStream the sequence of
	 *               bits coding the given symbol.
	 *  PRECONDITION: build() has been called, to create the coding
	 *                tree, and initialize root pointer and leaves vector.
	 *  Parameters: symbol - Used to write to BitOutputStream
	 *              out - Stream to write to
	 *  Return Value: None
	 */
	void encode(byte symbol, BitOutputStream& out) const;

	/** Function Name: encode(byte symbol, ofstream& out) const
	 *  Description: Write to the given ofstream the sequence of bits
	 *               (as ASCII) coding the given symbol.
	 *  PRECONDITION: build() has been called, to create the coding tree,
	 *                and initialize root pointer and leaves vector.
	 *  Parameters: symbol - Represents a sequence of bits representing a
	 *                       symbol (as ASCII)
	 *              out - The ofstream the sequence of bits will be writen
	 *                    to
	 *  Return Value: None
	 */
	void encode(byte symbol, ofstream& out) const;

	/** Function Name: decode(BitInputStream& in) const
	 *  Description: Return symbol coded in the next sequence of bits from
	 *               the stream.
	 *  PRECONDITION: build() has been called, to create the coding
	 *                tree, and initialize root pointer and leaves vector.
	 *  Parameters: in - the BitInputStream object to read from
	 *  Return Value: Symbol coded in the next sequence of bits from the
	 *                stream
	 */
	int decode(BitInputStream& in) const;

	/** Function Name: decode(ifstream& in)
	 *  Description: Return the symbol coded in the next sequence of bits
	 *               (represented as ASCII text) from the ifstream.
	 *  PRECONDITION: build() has been called, to create the coding
	 *                tree, and initialize root pointer and leaves vector.
	 *  Return Value: the symbol coded in the next sequence of bits from
	 *                the ifstream
	 */
	int decode(ifstream& in) const;
};

#endif // HCTREE_H
