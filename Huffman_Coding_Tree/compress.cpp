/** Filename: compress.cpp
 *  Name: Loc Chuong
 *  Userid: cs100sp19aj
 *  Description: The driver function for the Huffman Coding Tree compress
 *               functionality. The driver will take in two arguments, an
 *               inFile and an outFile. InFile should be the first argument
 *               and should consist of either an empty file or a series of
 *               ASCII characters to compress. OutFile should be the compressed
 *               version of the inFile done in the Huffman Coding Tree.
 *  Date: 5/11/2019
 */

#include "HCTree.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "math.h"

#define INFILE_ARG 1 /** Argument number for the inFile */
#define OUTFILE_ARG 2 /** Argument number for the outFile */
#define MAX_ARGS 3 /** Number of arguments that should be passed in */

using namespace std;

/** Function Name: main(int argc, char** argv)
 *  Description: The function that will drive the Huffman Coding Tree program's
 *               compress functionality. The driver will take in two arguments,
 *               an inFile and an outFile. InFile should be the first argument
 *               and should consist of either an empty file or a series of
 *               ASCII characters to compress. OutFile should be the compressed
 *               version of the inFile done in the Huffman Coding Tree.
 *  Parameters: argc - The number of arguments passed in
 *              argv - A pointer to the arguments passed in
 *  Return Value: The success of the program as an int
 */
int compress(int argc, char** argv) {
    /** Check for correct number of arguments */
    if(argc != MAX_ARGS) {
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./compress <infile filename> <outfile filename>." 
            << endl;
        return 0;
    }

    /** Open the input file for reading */
    ifstream inFile(argv[INFILE_ARG], ios::in);
    /** Create the output file for writing */
    ofstream outFile;
    /** Open output file */
    outFile.open(argv[OUTFILE_ARG]);
    /** Create BitOutputStream to write at the bit level */
    BitOutputStream outBOS = BitOutputStream(outFile);
    /** Read bytes from the file and count occurences of each byte value */
    unsigned char nextChar;
    /** Create int vector to hold ASCII frequency */
    vector<int> freqs(ASCII_MAX);

    /** Seek to end of file */
    inFile.seekg(0, ios::end);
    /** Check for empty file */
    if(inFile.tellg() == 0) {
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
    while(1) {
        nextChar = (unsigned char)inFile.get();
        if( inFile.eof()) break;
        /** Increment frequency of ASCII value */
        freqs[nextChar]++;
        /** Find the largest frequency of all the ASCII values */
        if( freqs[nextChar] > maxFreq) {
            /** Reassign max value of frequency to larger value */
            maxFreq = freqs[nextChar];
        }
    }
    /** Construct the Huffman coding tree using freqs vector */
    HCTree * HCT = new HCTree();
    /** Build Huffman coding tree */
    HCT->build(freqs);
    /** Find the number of unique ASCII values */
    for(int i = 0; i < freqs.size(); i++) {
        if(freqs[i] != 0) {
            uniqASCII++;
        }
    }
    /** Write the number of unique ASCII values */
    outBOS.writeInt(uniqASCII, BIT_SIZE_UNIQ_ASCII);
    /** Convert number of bits needed to store max frequency to byte */
    byte bitFreqs = ceil(log2(maxFreq));
    /** Handles case where maxFreq is 1 such that log2 gives unwanted num */
    if(maxFreq == 1) {
         bitFreqs = 1;
    }
    /** Write the number of bits needed to represent the max frequency */
    outBOS.writeInt(bitFreqs, BIT_SIZE_BIT_FREQS);

    /** Open the output file for writing */
    for(int i = 0; i < freqs.size(); i++) {
        /** Only write ASCII values that have frequency over 0 */
        if(freqs[i] != 0) {
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
    while(1) {
        /** Holds the next ASCII char from the input stream */
        nextChar = (unsigned char)inFile.get();
        /** Error checking */
        if( inFile.eof()) break;
        HCT->encode(nextChar, outBOS);
    }
    /** Add padding bits to end if a full byte was not written */
    while(outBOS.getnbits() != 0) {
        /** Append padding bits to end if the full byte was not written */
        outBOS.writeBit(0);
    }
    /** Close the output file */
    outFile.close();
    /** Close the input file */
    inFile.close();
    /** Deallocate HCTree */
    delete HCT;
    /** Return successful program */
    return 0;
}
