#include <iostream>
#include "HuffmanCoder.h"

int main() {
	//HuffmanCoder hfcoder("mytestfile.txt");
	HuffmanCoder hfcoder("Assignment_4_Test_File.txt");

	hfcoder.setHashTableSize(500);	// set hash table size accordingly based on how big the testfile is, default is 100

	hfcoder.tokenizePunct("TestFilePuncted.txt");

	hfcoder.encode();	// create hashtables for encode and decode

	hfcoder.outputData("encoded.data");
	hfcoder.outputMapping("encoded.map");

	hfcoder.decode("encoded.data", "recover.txt");	// decode the data file & dump to txt file to compare

  return 0;
}
