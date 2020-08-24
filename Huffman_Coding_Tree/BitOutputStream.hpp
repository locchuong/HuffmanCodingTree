/** Filename: BitOutputStream.hpp
 *  Name: Loc Chuong
 *  Description: An output stream class that permits writing one bit at a time.
 *               This stream utilizes the ostream from std to write and
 *               supports bit-level writing.
 *  Date: 5/14/2019
 */

#ifndef BITOUTPUTSTREAM_HPP
#define BITOUTPUTSTREAM_HPP

#include <iostream>
#include "HCNode.hpp"

#define BUFFER_SIZE 8 /** The size of the bit buffer buf in bits */
#define BIT_INT 32 /** The size of an int in terms of bits */

using namespace std;

/** Class Name: BitOutputStream
 *  Description: An output stream class that permits writing one bit at a time.
 *               This stream utilizes the ostream from std to write and
 *               supports bit-level writing.
 */
class BitOutputStream {
private:
	int nbits; /** How many bits have been written to buf */
	std::ostream& out; /** Reference to the output stream to use */
public:
	char buf; /** One byte buffer of bits */
	/** Constructor
	 *  Description: Initializes a BitOutStream using arguement os as the
	 *               ostream and sets buf and nbits to 0.
	 */
	BitOutputStream(std::ostream& os);

	/** Function Name: flush()
	 *  Description: Send the buffer to the output, and clear it
	 *  Return Value: None
	 */
	void flush();

	/** Function Name: writeBit(int i)
	 *  Description: Write the least significant bit of the argument to
	 *               the bit buffer, and increment the bit buffer index.
	 *               But flush the buffer first, if it is full.
	 *  Parameters: i - The least significant bit of this argument is
	 *                  writen to the bit buffer
	 *  Return Value: None
	 */
	void writeBit(int i);

	/** Function Name: writeByte(byte b)
	 *  Description: Write the parameter byte b to the buf and flushes to
	 *               ostream out when necessary.
	 *  Parameters: b - The byte to write to the char buf (All of b will
	 *                  be written to buf, but buf will not always equal
	 *                  buf if buf already has elements)
	 *  Return Value: None
	 */
	void writeByte(byte b);

	/** Function Name: writeInt(int freq, int maxBitFreq = BIT_INT)
	 *  Description: Write the parameter int freq to buf up to maxBitFreq
	 *               bits and flushes when buf is full.
	 *  Parameters: freq - The int to write to the bit buffer buf
	 *              maxBitFreq - The number of bits of freq to write to
	 *                           the bit buffer buf (Default is size of
	 *                           int in bits)
	 *  Return Value: None
	 */
	void writeInt(int freq, int maxBitFreq = BIT_INT);

	/** Function Name: getnbits()
	 *  Description: Getter method for nbits
	 *  Return Value: The value of nbits
	 */
	int getnbits();
};
#endif // BITOUTPUTSTREAM_HPP
