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
#include "compress.cpp"
#include "uncompress.cpp"

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
int main(int argc, char** argv) {
    compress(argc, argv);
    uncompress(argc, argv);
    return 0;
}