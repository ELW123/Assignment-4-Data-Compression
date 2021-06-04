#include <fstream>
#include "HashTable.h"

using namespace std;

HashTable::HashTable(int n){
    total_elements = n;
    table = new list<pair<string, string> >[total_elements];
}

HashTable::~HashTable(){
	delete[] table;
	table = NULL;
}

// Hash function to calculate hash for a value:
int HashTable::getHash(const string& key){
	int sum=0;
	for (size_t i=0; i<key.size(); i++) {
		char c = key[i];
		sum += (int)c;
	}
    return sum % total_elements;
}

// Insert data in the hash table:
void HashTable::insertElement(pair<string, string> mypair){
	string key = mypair.first;
    table[getHash(key)].push_back(mypair);
}

// Remove data from the hash table:
void HashTable::removeElement(const string& key){
    int x = getHash(key);

    list<pair<string,string> >::iterator i; 
    for (i = table[x].begin(); i != table[x].end(); i++) { 
      // Check if the iterator points to the required item:
      if ((*i).first == key) 
        break;
    }

    // If the item was found in the list, then delete it:
    if (i != table[x].end()) 
      table[x].erase(i);
}

void HashTable::printAll() const {
    // Traverse each index:
    for(int i = 0; i < total_elements; i++){
      cout << "Index " << i << ": ";
      // Traverse the list at current index:
      list<pair<string, string> >::iterator j; 
      for (j = table[i].begin(); j != table[i].end(); j++) { 
        cout << " => " << (*j).first << " " << (*j).second;
      }

      cout << endl;
    }
}

void HashTable::dump(const string& dumpfile) const {
	ofstream outFS(dumpfile.c_str());
    if(!outFS.is_open()) {
        cout<<"Could not write file:" << dumpfile <<endl;
        return;
    }

    // Traverse each index:
    for(int i = 0; i < total_elements; i++){
	  if (table[i].size() == 0) continue;	// skip empty entries
      outFS << "Index " << i << ": ";
      // Traverse the list at current index:
      list<pair<string, string> >::iterator j; 
      for (j = table[i].begin(); j != table[i].end(); j++) { 
        outFS << " => " << (*j).first << " " << (*j).second;
      }

      outFS << endl;
    }

    outFS.close();
}

// find key and return paired data
string HashTable::locate(const string& key){
    int x = getHash(key);

    list<pair<string,string> >::iterator i; 
    for (i = table[x].begin(); i != table[x].end(); i++) { 
      // Check if the iterator points to the required item:
      if ((*i).first == key) 
        return (*i).second;
    }
	return string("");
}

