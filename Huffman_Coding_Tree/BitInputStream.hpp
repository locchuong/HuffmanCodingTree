/** Filename: BitInputStream.hpp
 *  Name: Loc Chuong
 *  Description: An input stream class that permits reading one bit at a time.
 *               This stream utilizes the istream from std to read and
 *               supports bit-level reading.
 *  Date: 5/14/2019
 */

#ifndef BITINPUTSTREAM_HPP
#define BITINPUTSTREAM_HPP

#define BUF_SIZE 8 /** Size of bit buffer in terms of bits */

#include <iostream>
#include "HCNode.hpp"

class BitInputStream {
private:
	char buf; /** One byte buffer of bits */
	int nbits; /** How many bits have been read from buf */
	std::istream& in; /** The input stream to use */
public:
	/** Constructor
	 *  Description: Initializes a bit input stream using istream is
	 */
	BitInputStream(std::istream& is);

	/** Function Name: fill()
	 *  Description: Fill the buffer from the input
	 *  Return Value: None
	 */
	void fill();

	/** Function Name: readBits()
	 *  Description: Read the next bit from the bit buffer
	 *               Fills the buffer from the input stream first if
	 *               needed
	 *  Return Value: Returns 0 if the bit read is 1, returns 0 if the bit
	 *                read is 0
	 */
	int readBits();

	/** Function Name: readByte()
	 *  Description: Reads a byte from the bit buffer and fills the
	 *               buffer from the input stream when necessary
	 *  Return Value: A byte read from the bit buffer
	 */
	byte readByte();

	/** Function Name: readByte()
	 *  Description: Reads bitFreqs num of bits from the bit buffer and
	 *               fills the buffer from the input stream when necessary
	 *  Parameters: bitFreqs - The number of bits to read from the buffer
	 *  Return Value: An int set up to bitFreqs num of bits
	 */
	int readInt(int bitFreqs);

	int getnbits();
};
#endif // BITINPUTSTREAM_HPP
