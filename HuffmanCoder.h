#pragma once
#ifndef __HUFFMANCODER_H__
#define __HUFFMANCODER_H__

#include <vector>
#include "HashTable.h"

using namespace std;

struct WordFreqCode {
	string _word;
	int	_freq;
	int _code;

	WordFreqCode(string w) : _word(w), _freq(1), _code(0) {}
};

typedef vector<WordFreqCode>::iterator WFCIter;

class HuffmanCoder {
private:
	string _filename;		// file needed to be coded
	int _hashTableSize;
	HashTable* _encoder;	// encoder HashTable
	HashTable* _decoder;	// encoder HashTable
	vector<WordFreqCode> _allWordFreqCode;

	// helper functions
	WFCIter find(const string& key);  // find in vector
	void sort_assigncode();				// sort and assign code accordingly

public:
    /* Constructors */
    HuffmanCoder(const string& filename);
    ~HuffmanCoder();

    /* Mutators */
	void setHashTableSize(int N);// set hash table size accordingly based on how big the testfile is, default is 100

	void encode();	// create hashtables for encode and decode

	// decode the data file & dump to txt file to compare
	void decode(const string& datafile, const string& txtfile) const;

	// reload file and convert to data file
	void outputData(const string& datafile) const;	

	// dump decoding hashtable
	void outputMapping(const string& mapfile) const;

    /* Accessors */

    /* Printing */
};

#endif // __HUFFMANCODER_H__
