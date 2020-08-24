/** Filename: Driver.cpp
 *  Name: Loc Chuong
 *  Description: The driver function for the Huffman Coding Tree compress
 *               functionality.
 *  Date: 8/23/2020
 */

#include "HCTree.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "math.h"

#define DECODE_EOF -1 /** End of file reached for decoded as non-ASCII value */
#define ACTION_ARG 1 /** Argument number to denote command */
#define INFILE_ARG 2 /** Argument number for the inFile */
#define OUTFILE_ARG 3 /** Argument number for the outFile */

#define MAX_ARGS 4 /** Number of arguments that should be passed in */

using namespace std;

/** Function Name: uncompress(string inputFile, string outputFile)
 *  Description: The function that will drive the Huffman Coding Tree program's
 *               uncompress functionality.
 *  Parameters: inputFile - Name of file that will be uncompressed
 *              outputFIle - Desired name of file to uncompress to
 *  Return Value: The success of the program as an int
 */
int uncompress(string inputFile, string outputFile) {
	/** Open the input file for reading */
	ifstream inFile(inputFile, ios::binary);
	/** Create BitInputStream to read at the bit level */
	BitInputStream inBIS = BitInputStream(inFile);
	/** Create the output stream for writing */
	ofstream outFile;
	/** Open the file */
	outFile.open(outputFile);
	/** Read from the inFile and assign values to the frequency vector */
	vector<int> freqs(ASCII_MAX);

	/** Seek to end of file */
	inFile.seekg(0, ios::end);

	/** Check for empty file */
	if (inFile.tellg() == 0) {
		return 0;
	}
	/** Seek back to beginning of file */
	inFile.clear();
	inFile.seekg(0, ios::beg);

	/** Create HCTree to hold ASCII values and their frequency */
	HCTree* HCT = new HCTree();
	/** Read in the max frequency number of bits */
	int uniqASCII = inBIS.readInt(BIT_SIZE_UNIQ_ASCII);
	/** Read in the number of bits needed to store frequency of ASCII */
	int bitFreqs = inBIS.readInt(BIT_SIZE_BIT_FREQS);
	/** Holds the total number of ASCII in the uncompressed message */
	int totalFreq = 0;
	/** Read in the correct number of uniq ASCII */
	for (int i = 0; i < uniqASCII; i++) {
		/** Read the ASCII char itself */
		byte ASCII = inBIS.readByte();
		/** Read the frequency of the ASCII char */
		int freq = inBIS.readInt(bitFreqs);
		/** Increment the total frequency of all ASCII */
		totalFreq += freq;
		/** Set the frequency of the ASCII char in the freqs vector */
		freqs[(int)ASCII] = freq;
	}
	/** Build the Huffman coding tree using freqs vector */
	HCT->build(freqs);

	/** Create string as buffer for uncompressed message */
	string lastLine = string();
	/** Decode for a total of all the ASCII characters in the message */
	for (int i = 0; i < totalFreq; i++) {
		/** Holds the next ASCII character decoded */
		int nextChar;
		/** Error checking and decodes binary to ASCII */
		if ((nextChar = HCT->decode(inBIS)) != DECODE_EOF) {
			/** Append to the output string */
			lastLine += (unsigned char)nextChar;
		}
	}
	/** Output uncompressed message to outFile */
	outFile << lastLine;

	/** Close output file */
	outFile.close();
	/** Close input file */
	inFile.close();
	/** Deallocate HCTree */
	delete HCT;
	/** Return successful program */
	return 0;
}

/** Function Name: compress(string inputFile, string outputFile)
 *  Description: The function that will drive the Huffman Coding Tree program's
 *               compress functionality. The driver will take in two arguments,
 *               an inputFile and an outputFile. inputFile will consist of either
 *               an empty file or a series of ASCII characters to compress.
 *               outputFile will be the compressed version of the inputFile
 *               using the Huffman Coding Tree.
 *  Parameters: inputFile - the filename to the file to be compressed
 *              outputFile - the filename to the compressed ver. of inputFile
 *                           using a Huffman Coding Tree
 *  Return Value: The success of the program as an int
 */
int compress(string inputFile, string outputFile) {

	/** Open the input file for reading */
	ifstream inFile(inputFile, ios::in);
	/** Open the output file for writing */
	ofstream outFile(outputFile, ios::binary);
	/** Create BitOutputStream to write at the bit level */
	BitOutputStream outBOS = BitOutputStream(outFile);
	/** Read bytes from the file and count occurences of each byte value */
	unsigned char nextChar;
	/** Create int vector to hold ASCII frequency */
	vector<int> freqs(ASCII_MAX);

	/** Seek to end of file */
	inFile.seekg(0, ios::end);
	/** Check for empty file */
	if (inFile.tellg() == 0) {
		return 0;
	}
	/** Seek back to beginning of file */
	inFile.clear();
	inFile.seekg(0, ios::beg);

	/** Max frequency in vector of ASCII vals */
	double maxFreq = 0;
	/** Number of unique ASCII characters in our HCTree */
	unsigned int uniqASCII = 0;
	/** Populate freqs vector with characters from file */
	while (1) {
		nextChar = (unsigned char)inFile.get();
		if (inFile.eof()) break;
		/** Increment frequency of ASCII value */
		freqs[nextChar]++;
		/** Find the largest frequency of all the ASCII values */
		if (freqs[nextChar] > maxFreq) {
			/** Reassign max value of frequency to larger value */
			maxFreq = freqs[nextChar];
		}
	}
	/** Construct the Huffman coding tree using freqs vector */
	HCTree* HCT = new HCTree();
	/** Build Huffman coding tree */
	HCT->build(freqs);
	/** Find the number of unique ASCII values */
	for (int i = 0; i < freqs.size(); i++) {
		if (freqs[i] != 0) {
			uniqASCII++;
		}
	}
	/** Write the number of unique ASCII values */
	outBOS.writeInt(uniqASCII, BIT_SIZE_UNIQ_ASCII);

	/** Convert number of bits needed to store max frequency to byte */
	byte bitFreqs = (byte)(floor(log2(maxFreq)) + 1);

	/** Write the number of bits needed to represent the max frequency */
	outBOS.writeInt(bitFreqs, BIT_SIZE_BIT_FREQS);
	/** Open the output file for writing */
	for (int i = 0; i < freqs.size(); i++) {
		/** Only write ASCII values that have frequency over 0 */
		if (freqs[i] != 0) {
			/** Write ASCII char as byte to output file */
			outBOS.writeByte((byte)i);
			/** Write frequency of ASCII char as byte to output file */
			outBOS.writeInt(freqs[i], (int)bitFreqs);
		}
	}
	/** Print out header size */
	cout << "Header size is: " << (int)outFile.tellp() << " bytes" << endl;

	/** Move file iterator to beginning of file */
	inFile.clear();
	inFile.seekg(0, ios::beg);
	/** Encode the ASCII characters from inFile */
	while (1) {
		/** Holds the next ASCII char from the input stream */
		nextChar = (unsigned char)inFile.get();
		/** Error checking */
		if (inFile.eof()) break;
		HCT->encode(nextChar, outBOS);
	}
	/** Add padding bits to end if a full byte was not written */
	while (outBOS.getnbits() != 0) {
		/** Append padding bits to end if the full byte was not written */
		outBOS.writeBit(0);
	}

	/** Close the output file */
	outFile.close();
	/** Close the input file */
	inFile.close();
	/** Deallocate HCTree */
	delete HCT;

	/** Uncompress the message */
	return 0;
}

/** Function Name: main(int argc, char** argv)
 *  Description: The main driver function for the Huffman Coding Program.
 *               Note: Command Line arguments not used.
 *  Parameters: argc - Number of command line arguments
 *              argv - Array consisting of the command line arguments.
 *  Return Value: The success of the program as an int
 */
int main(int argc, char** argv) {
	/** Initialize user input strings, char, and flag */
	string inputFile{};
	string outputFile{};
	string command{};
	char cont;
	int flag = -1;
	cout << "Huffman Coding Tree" << endl;
	/** Loop until user exits */
	while (1) {
		/** Loop until user inputs a valid command */
		while (1) {
			cout << "Type in 'compress' or 'uncompress' to compress/uncompress a file." << endl;
			cin >> command;
			if (command.compare("compress") == 0) { flag = 0; break; }
			if (command.compare("uncompress") == 0) { flag = 1;  break; }
		}
		cout << "Type in the name of the input file" << endl;
		cin >> inputFile;
		cout << "Type in the desire name of the output file" << endl;
		cin >> outputFile;
		/** Perform compression or decompression */
		if (flag == 0) compress(inputFile, outputFile);
		if (flag == 1) uncompress(inputFile, outputFile);
		/** Ask user if they want to loop through program */
		cout << "Do you want to continue? (y/n)" << endl;
		cin >> cont;
		if (cont == 'y') {
			continue;
		}
		else {
			break;
		}
	}
	return 0;
}