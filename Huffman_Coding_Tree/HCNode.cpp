/** Filename: HCNode.cpp
 *  Name: Loc Chuong
 *  Description: A class, instances of which are nodes in an HCTree. HCNodes
 *               have instance variables count, symbol, c0, c1, and p. Count
 *               is the frequency of symbol, a byte that we are keeping track
 *               of. c0 points to the '0' child, c1 to the '1' child, and p to
 *               the parent.
 *  Date: 5/11/2019
 */

#include "HCNode.hpp"

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
bool comp(HCNode* one, HCNode* other) {
	return *one < *other;
}
