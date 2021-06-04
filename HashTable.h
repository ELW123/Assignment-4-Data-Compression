#pragma once
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <iostream>
#include <list>
#include <string>

using namespace std;

class HashTable {
private:
	list<pair<string, string> > *table;
	int total_elements;
	int getHash(const string& key);	// Hash function to calculate hash for a value

public:
    /* Constructors */
    HashTable(int n);	// Constructor to create a hash table with 'n' indices
    ~HashTable();		// Destrctor

    /* Mutators */
	void insertElement(pair<string, string> mypair);	// Insert data in the hash table
	void removeElement(const string& key);	// Remove data from the hash table
	string locate(const string& key);		// find key and return paired data
	void dump(const string& dumpfile) const;

    /* Accessors */

    /* Printing */
	void printAll() const;
};

#endif // __HASHTABLE_H__
