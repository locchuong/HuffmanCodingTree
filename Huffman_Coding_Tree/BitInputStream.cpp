/** Filename: BitInputStream.cpp
 *  Name: Loc Chuong
 *  Description: An input stream class that permits reading one bit at a time.
 *               This stream utilizes the istream from std to read and
 *               supports bit-level reading.
 *  Date: 5/14/2019
 */

#include "BitInputStream.hpp"

 /** Constructor
  *  Description: Initializes a bit input stream using istream is
  */
BitInputStream::BitInputStream(std::istream& is) :in(is) {
	/** Initialize empty buffer */
	buf = 0;
	/** Denote that the buffer needs to be filled */
	nbits = 8;
}

/** Function Name: fill()
 *  Description: Fill the buffer from the input
 *  Return Value: None
 */
void BitInputStream::fill() {
	/** Fill buffer from input */
	buf = in.get();
	/** Set number of bits already read to 0 */
	nbits = 0;
}

/** Function Name: readBits()
 *  Description: Read the next bit from the bit buffer
 *               Fills the buffer from the input stream first if needed
 *  Return Value: Returns 1 if the bit read is 1, returns 0 if the bit read is
 *                0
 */
int BitInputStream::readBits() {
	/* Fill the buffer if all bits in the buffer are read */
	if (nbits == BUF_SIZE) {
		fill();
	}
	if (buf & (1 << (7 - nbits))) {
		/** Increment the index */
		nbits++;
		return 1;
	}
	else {
		/** Increment the index */
		nbits++;
		return 0;
	}
}

/** Function Name: readByte()
 *  Description: Reads a byte from the bit buffer and fills the
 *               buffer from the input stream when necessary
 *  Return Value: A byte read from the bit buffer
 */
byte BitInputStream::readByte() {
	/** Holds a byte from buf */
	byte ret = 0;
	/** Loop through an entire byte (8 bits) */
	for (int i = 0; i < BUF_SIZE; i++) {
		/** Read in the next bit from the buf */
		int bit = readBits();
		/** Bit grabbed from input stream is 1 */
		if (bit == 1) {
			/** Set 1 in the returning value in the appropriate index */
			ret |= (1 << i);
		}
		/** Bit grabbed from input stream is 0 */
		else {
			/** Set 0 in the returning value in the appropriate index */
			ret &= ~(1 << i);
		}
	}
	return ret;
}

/** Function Name: readByte()
 *  Description: Reads bitFreqs num of bits from the bit buffer and
 *               fills the buffer from the input stream when necessary
 *  Parameters: bitFreqs - The number of bits to read from the buffer
 *  Return Value: An int set up to bitFreqs num of bits
 */
int BitInputStream::readInt(int bitFreqs) {
	/** Holds bitFreqs num of bits from buf */
	int ret = 0;
	/** Loop through the num of bitFreqs parameter */
	for (int i = 0; i < bitFreqs; i++) {
		/** Read in the next bit from the buf */
		int bit = readBits();
		/** Bit grabbed from input stream is 1 */
		if (bit == 1) {
			/** Set 1 in the returning value in the appropriate index */
			ret |= (1 << i);
		}
		/** Bit grabbed from input stream is 0 */
		else {
			/** Set 0 in the returning value in the appropriate index */
			ret &= ~(1 << i);
		}
	}
	return ret;
}
int BitInputStream::getnbits() {
	return nbits;
}