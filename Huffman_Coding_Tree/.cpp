/** Filename: uncompress.cpp
 *  Name: Loc Chuong
 *  Userid: cs100sp19aj
 *  Description: The driver function for the Huffman Coding Tree uncompress
 *               functionality. The driver will take in two arguments, an
 *               inFile and an outFile. InFile should be the first argument
 *               and should consist of a compressed version of the Huffman
 *               Coding Tree. OutFile should be the uncompressed version of the
 *               inFile.
 *  Date: 5/11/2019
 */

#include "HCTree.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#define DECODE_EOF -1 /** End of file reached for decoded as non-ASCII value */
#define INFILE_ARG 1 /** Argument number for the inFile */
#define OUTFILE_ARG 2 /** Argument number for the outFile */
#define MAX_ARGS 3 /** Number of arguments that should be passed in */

using namespace std;

/** Function Name: main(int argc, char** argv)
 *  Description: The function that will drive the Huffman Coding Tree program's
 *               uncompress functionality.
 *
 *
 */
int main(int argc, char** argv) {
    /** Check for correct number of arguments */
    if(argc != MAX_ARGS) {
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./uncompress <infile filename> <outfile filename>." 
            << endl;
        return 0;
    }

    /** Open the input file for reading */
    ifstream inFile(argv[INFILE_ARG], ios::in);
    /** Create BitInputStream to read at the bit level */
    BitInputStream inBIS = BitInputStream(inFile);
    /** Create the output stream for writing */
    ofstream outFile;
    /** Open the file */
    outFile.open(argv[OUTFILE_ARG]);
    /** Read from the inFile and assign values to the frequency vector */
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
    
    /** Create HCTree to hold ASCII values and their frequency */
    HCTree * HCT = new HCTree();

    /** Read in the max frequency number of bits */
    int uniqASCII = inBIS.readInt(BIT_SIZE_UNIQ_ASCII);
    /** Read in the number of bits needed to store frequency of ASCII */
    int bitFreqs = inBIS.readInt(BIT_SIZE_BIT_FREQS);
    /** Holds the total number of ASCII in the uncompressed message */ 
    int totalFreq = 0;
    /** Read in the correct number of uniq ASCII */
    for(int i = 0; i < uniqASCII; i++) {
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
    for(int i = 0; i < totalFreq; i++) {
        /** Holds the next ASCII character decoded */
        int nextChar;
        /** Error checking and decodes binary to ASCII */
        if( (nextChar = HCT->decode(inBIS)) != DECODE_EOF) {
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
