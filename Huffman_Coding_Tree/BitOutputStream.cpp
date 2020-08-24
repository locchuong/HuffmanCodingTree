/** Filename: BitOutputStream.cpp
 *  Name: Loc Chuong
 *  Description: An output stream class that permits writing one bit at a time.
 *               This stream utilizes the ostream from std to write and
 *               supports bit-level writing.
 *  Date: 5/14/2019
 */

#include "BitOutputStream.hpp"

 /** Constructor
  *  Description: Initializes a BitOutStream using arguement os as the ostream
  *               and sets buf and nbits to 0.
  */
BitOutputStream::BitOutputStream(std::ostream& os) :out(os), buf(0), nbits(0) {
}

/** Function Name: flush()
 *  Description: Send the buffer to the output, and clear it
 *  Return Value: None
 */
void BitOutputStream::flush() {
	/** Send buffer to the output */
	out.put(buf);
	/** Flush the ostream */
	out.flush();
	/** Clear the buffer */
	buf = nbits = 0;
}

/** Function Name: writeBit(int i)
 *  Description: Write the least significant bit of the argument to the bit
 *               buffer, and increment the bit buffer index. But flush the
 *               buffer first, if it is full.
 *  Parameters: i - The least significant bit of this argument is writen to
 *                  the bit buffer
 *  Return Value: None
 */
void BitOutputStream::writeBit(int i) {
	/** Checks if bit buffer is full */
	if (nbits == BUFFER_SIZE) {
		flush();
	}
	/** Write least significant bit of i into buffer at current index */
	if (i & 1) {
		/** Set buf to 1 at nbits index */
		buf |= (1 << (7 - nbits));
	}
	else {
		/** Set buf to 0 at nbits index */
		buf &= ~(1 << (7 - nbits));
	}
	/** Increment the index */
	nbits++;
	/** Checks if bit buffer is full */
	if (nbits == BUFFER_SIZE) {
		flush();
	}
}

/** Function Name: writeByte(byte b)
 *  Description: Write the parameter byte b to the buf and flushes to
 *               ostream out when necessary.
 *  Parameters: b - The byte to write to the char buf (All of b will
 *                  be written to buf, but buf will not always equal
 *                  buf if buf already has elements)
 *  Return Value: None
 */
void BitOutputStream::writeByte(byte b) {
	/** Checks if bit buffer is full */
	if (nbits == BUFFER_SIZE) {
		flush();
	}
	for (int i = 0; i < BUFFER_SIZE; i++) {
		/** Set ith bit of buf to ith bit of b (which is 1)*/
		if (b & (1 << i)) {
			/** Set ith bit of buf as 1 */
			buf |= (1 << (7 - nbits));
			/** Increment the index */
			nbits++;
		}
		/** Set ith bit of buf to ith bit of b (which is 0)*/
		else {
			/** Set ith bit of buf as 0 */
			buf &= ~(1 << (7 - nbits));
			/** Increment the index */
			nbits++;
		}
		/** Flush if the buf is full */
		if (nbits == BUFFER_SIZE) {
			flush();
		}
	}
}

/** Function Name: writeInt(int freq, int maxBitFreq = BIT_INT)
 *  Description: Write the parameter int freq to buf up to maxBitFreq
 *               bits and flushes when buf is full.
 *  Parameters: freq - The int to write to the bit buffer buf
 *              maxBitFreq - The number of bits of freq to write to
 *                           the bit buffer buf (Default is size of
 *                           int in bits)
 *  Return Value: None
 */
void BitOutputStream::writeInt(int freq, int maxBitFreq) {
	/** Flush if the buf is full */
	if (nbits == BUFFER_SIZE) {
		flush();
	}
	/** Writes freq to bit buffer buf up to maxBitFreq bits */
	for (int i = 0; i < maxBitFreq; i++) {
		writeBit(freq >> i);
	}
	/** Flush if the buf is full */
	if (nbits == BUFFER_SIZE) {
		flush();
	}
}

/** Function Name: getnbits()
 *  Description: Getter method for nbits
 *  Return Value: The value of nbits
 */
int BitOutputStream::getnbits() {
	return nbits;
}

