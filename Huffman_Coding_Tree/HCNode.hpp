/** Filename: HCNode.hpp
 *  Name: Loc Chuong
 *  Description: A class, instances of which are nodes in an HCTree. HCNodes
 *               have instance variables count, symbol, c0, c1, and p. Count
 *               is the frequency of symbol, a byte that we are keeping track
 *               of. c0 points to the '0' child, c1 to the '1' child, and p to
 *               the parent.
 *  Date: 5/11/2019
 */

#ifndef HCNODE_HPP
#define HCNODE_HPP

#include <iostream>

typedef unsigned char byte;

using namespace std;

/** Class Name: HCNode
 *  Description: A class, instances of which are nodes in an HCTree. HCNodes
 *               have instance variables count, symbol, c0, c1, and p. Count
 *               is the frequency of symbol, a byte that we are keeping track
 *               of. c0 points to the '0' child, c1 to the '1' child, and p to
 *               the parent.
 */
class HCNode {
	friend bool comp(HCNode* one, HCNode* other);

public:
	int count; /** Denotes the frequency of the symbol in the HCTree */
	byte symbol; /** byte in the file we're keeping track of */
	HCNode* c0;  /** pointer to '0' child */
	HCNode* c1;  /** pointer to '1' child */
	HCNode* p;   /** pointer to parent */

	/** Constructor
	 *  Description: Creates a HCNode using the parameters to set instance
	 *               variables. c0, c1, and p will be set to 0 if not
	 *               specified.
	 *  Parameters: count - Frequency of the symbol in the HCTree
	 *              symbol - Byte in the file we're keeping track of
	 *              c0 - pointer to '0' child
	 *              c1 - pointer to '1' child
	 *              p - pointer to parent
	 */
	HCNode(int count,
		byte symbol,
		HCNode* c0 = 0,
		HCNode* c1 = 0,
		HCNode* p = 0)
		: count(count), symbol(symbol), c0(c0), c1(c1), p(p) { }

	/** Less-than operator
	 *  Description: Less-than comparison, so HCNodes will work in
	 *               std::priority_queue. We want small counts to have high
	 *               priority. And we want to break ties deterministically.
	 *  Parameters: other - The other HCNode to compare to this HCNode
	 *  Return Value: True if other has higher priority than this, false
	 *                otherwise
	 */
	bool operator<(const HCNode& other) {
		/** if counts are different, just compare counts */
		if (this->count != other.count) {
			return this->count > other.count;
		}
		/** counts are equal. use symbol value to break tie. */
		return this->symbol < other.symbol;
	}
};

/** Operator <<
 *  Description: For printing an HCNode to an ostream.
 *               Possibly useful for debugging.
 *               Shut the linker up.
 */
 /*
 ostream& operator<<(ostream& stm, const HCNode& n) {
	 stm << "[" << n.count << "," << (int) (n.symbol) << "]";
	 return stm;
 }
 */

 /** Function Name: comp(HCNode* one, HCNode* other)
  *  Description: An implementation of a comparison function used to sort items
  *               in a std::priority_queue. We want small counts to have high
  *               priority. And we want to break ties deterministically.
  *  Parameters: one - A pointer to a HCNode to compare priority
  *              other - A pointer to a HCNode to compare priority
  *  Return Value: True if other has higher priority than this, false otherwise
  *  NOTE: This function has been implemented, but the operator < and comparator
  *        class will be used instead.
  */
bool comp(HCNode* one, HCNode* other);

#endif // HCNODE_HPP
