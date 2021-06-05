#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <algorithm>
#include "HuffmanCoder.h"

using namespace std;

HuffmanCoder::HuffmanCoder(const string& filename) : _filename(filename), _hashTableSize(100), _encoder(NULL), _decoder(NULL) {
}

HuffmanCoder::~HuffmanCoder(){
	delete _encoder;
	_encoder = NULL;
	delete _decoder;
	_decoder = NULL;
}

void HuffmanCoder::setHashTableSize(int N) {
	_hashTableSize = N;
}

WFCIter HuffmanCoder::find(const string& key) {
	WFCIter it;
	for (it=_allWordFreqCode.begin(); it!= _allWordFreqCode.end(); it++) {
		if ( (*it)._word == key ) return it;
	}
	return _allWordFreqCode.end();
}

// descending order
bool compareFunc(const WordFreqCode& a, const WordFreqCode& b) {
	return a._freq > b._freq;
}

void HuffmanCoder::sort_assigncode() {
	sort(_allWordFreqCode.begin(), _allWordFreqCode.end(), compareFunc);

	int order = 1;		// code starting with 1
	for (WFCIter it=_allWordFreqCode.begin(); it!=_allWordFreqCode.end(); it++)
	{
		(*it)._code = order;
		order++;
	}
}

void HuffmanCoder::encode() {
	vector<pair<string,int> > allWords;
	pair<string, int> wordFreqPair;

	fstream file;
	file.open(_filename.c_str());
	if (!file.is_open()) {
      cout << "Could not open file:" << _filename << endl;
      return;
    }

	// extracting words from file
	string word;
	string punctWordInitial;
	string punctWordAfter;
	int location;
	while (file >> word) {
		//cout << word << endl;
		WFCIter it=find(word);
		if (it != _allWordFreqCode.end()) { // found
			(*it)._freq++;
		} else {
			_allWordFreqCode.push_back(WordFreqCode(word));
		}
	}
	file.close();

	// print freqs, saved to result.txt 

	for (WFCIter it=_allWordFreqCode.begin(); it!=_allWordFreqCode.end(); it++) {
		cout << (*it)._word << "\t" << (*it)._freq << endl;
	}


	sort_assigncode();	// sort in descending order and assign codes

	// create hashTables
	_encoder = new HashTable(_hashTableSize);
	_decoder = new HashTable(_hashTableSize);
	for (WFCIter it=_allWordFreqCode.begin(); it!=_allWordFreqCode.end(); it++)
	{
		//cout << (*it)._word << "\t" << (*it)._freq << "\t" << (*it)._code << endl;
		// convert int to char*
		stringstream ss;
		ss << (*it)._code;
		string codeStr = ss.str();

		pair<string, string> pairWordCode( (*it)._word, codeStr );
		pair<string, string> pairCodeWord( codeStr, (*it)._word );
		_encoder->insertElement(pairWordCode);
		_decoder->insertElement(pairCodeWord);
	}

	// print
/*
	cout << "Printing encoder >>>> " << endl;
	_encoder->printAll();
	cout << "Printing decoder >>>> " << endl;
	_decoder->printAll();
*/
}

void HuffmanCoder::outputData(const string& datafile) const {
	ifstream file;
	file.open(_filename.c_str());
	if (!file.is_open()) {
      cout << "Could not open file:" << _filename << endl;
      return;
    }

	ofstream outFS(datafile.c_str());
    if(!outFS.is_open()) {
        cout<<"Could not write file:" << datafile <<endl;
        return;
    }
	
	string line;
	while (getline(file, line)) {	// read each line
		istringstream ss(line);
		string word;
		while (ss >> word) {	// split into words
			// cout << word << "\n";
			string encoded = _encoder->locate(word);
			// cout << encoded << "\n";
			outFS << encoded << " ";
		}
		outFS << endl;
	}

	file.close();
    outFS.close();
}

void HuffmanCoder::outputMapping(const string& mapfile) const {
	_decoder->dump(mapfile);
}

void HuffmanCoder::decode(const string& datafile, const string& txtfile) const {
	ifstream file;
	file.open(datafile.c_str());
	if (!file.is_open()) {
      cout << "Could not open file:" << datafile << endl;
      return;
    }

	ofstream outFS(txtfile.c_str());
    if(!outFS.is_open()) {
        cout<<"Could not write file:" << txtfile <<endl;
        return;
    }
	
	string line;
	while (getline(file, line)) {	// read each line
		istringstream ss(line);
		string word;
		while (ss >> word) {	// split into words. Also, this part can be modified to display the punctuation correctly without spaces (but TA said it was fine)
			// cout << word << "\n";
			string decoded = _decoder->locate(word);
			// cout << decoded << "\n";
			outFS << decoded << " ";
		}
		outFS << endl;
	}

	file.close();
    outFS.close();
}

bool HuffmanCoder::containsPunct(string c) {
	if (c.find('.') != std::string::npos ||
        c.find(',') != std::string::npos ||
        c.find('\"') != std::string::npos ||
        c.find('\'') != std::string::npos ||
        c.find('!') != std::string::npos ||
        c.find('?') != std::string::npos ||
        c.find('-') != std::string::npos ||
        c.find('*') != std::string::npos ||
        c.find('{') != std::string::npos ||
        c.find('}') != std::string::npos ||
		c.find(';') != std::string::npos ||
		c.find('^') != std::string::npos ||
		c.find('%') != std::string::npos ||
		c.find('/') != std::string::npos ||
		c.find('$') != std::string::npos) {
        return true;
    }

    return false;
}

// reads in data from TestFile and creates a new punctuation friendly doc that the rest of the program will read over
void HuffmanCoder::tokenizePunct(const string& datafile) {
	ifstream file;
	file.open(_filename.c_str());
	if (!file.is_open()) {
      cout << "Could not open file:" << _filename << endl;
      return;
    }
	
	ofstream outFS(datafile.c_str());
    if(!outFS.is_open()) {
        cout<<"Could not write file:" << datafile <<endl;
        return;
    }

	string line;
	while (getline(file, line)) {	// read each line
		istringstream ss(line);
		string word;
		string substrAfter;
		
		// still need to account for words after punctuation if nececssary
		while (ss >> word) {	// split into words
			// cout << word << "\n";
			if (word.find(".") != string::npos) {
				outFS << word.substr(0, word.find(".")) << " " << "." << " "; //<< word.substr(word.find(".")+1) << " ";
				// cout << word.substr(word.find(".")) << endl;
				if (word.substr(word.find(".")+1).length() > 0) {
					// cout << word.substr(word.find(".")+1);
					outFS << word.substr(word.find(".")+1) << " ";
				}
			} 
			if (word.find(",") != string::npos) {
				outFS << word.substr(0, word.find(",")) << " " << "," << " ";
				if (word.substr(word.find(",")+1).length() > 0) {
					outFS << word.substr(word.find(",")+1) << " ";
				}
			}
			if (word.find("\"") != string::npos) {
				outFS << word.substr(0, word.find("\"")) << " " << "\"" << " ";
				if (word.substr(word.find("\"")+1).length() > 0) {
					outFS << word.substr(word.find("\"")+1) << " ";
				}
			}
			if (word.find("'") != string::npos) {
				outFS << word.substr(0, word.find("'")) << " " << "'" << " ";
				if (word.substr(word.find("'")+1).length() > 0) {
					outFS << word.substr(word.find("'")+1) << " ";
				}
			}
			if (word.find("!") != string::npos) {
				outFS << word.substr(0, word.find("!")) << " " << "!" << " ";
				if (word.substr(word.find("!")+1).length() > 0) {
					outFS << word.substr(word.find("!")+1) << " ";
				}
			}
			if (word.find("?") != string::npos) {
				outFS << word.substr(0, word.find("?")) << " " << "?" << " ";
				if (word.substr(word.find("?")+1).length() > 0) {
					outFS << word.substr(word.find("?")+1) << " ";
				}
			}
			if (word.find("-") != string::npos) {
				outFS << word.substr(0, word.find("-")) << " " << "-" << " ";
				if (word.substr(word.find("-")+1).length() > 0) {
					outFS << word.substr(word.find("-")+1) << " ";
				}
			}
			if (word.find("*") != string::npos) {
				outFS << word.substr(0, word.find("*")) << " " << "*" << " ";
				if (word.substr(word.find("*")+1).length() > 0) {
					outFS << word.substr(word.find("*")+1) << " ";
				}
			}
			if (word.find("{") != string::npos) {
				outFS << word.substr(0, word.find("{")) << " " << "{" << " ";
				if (word.substr(word.find("{")+1).length() > 0) {
					outFS << word.substr(word.find("{")+1) << " ";
				}
			}
			if (word.find("}") != string::npos) {
				outFS << word.substr(0, word.find("}")) << " " << "}" << " ";
				if (word.substr(word.find("}")+1).length() > 0) {
					outFS << word.substr(word.find("}")+1) << " ";
				}
			}
			if (word.find(";") != string::npos) {
				outFS << word.substr(0, word.find(";")) << " " << ";" << " ";
				if (word.substr(word.find(";")+1).length() > 0) {
					outFS << word.substr(word.find(";")+1) << " ";
				}
			}
			if (word.find("^") != string::npos) {
				outFS << word.substr(0, word.find("^")) << " " << "^" << " ";
				if (word.substr(word.find("^")+1).length() > 0) {
					outFS << word.substr(word.find("^")+1) << " ";
				}
			}
			if (word.find("%") != string::npos) {
				outFS << word.substr(0, word.find("%")) << " " << "%" << " ";
				if (word.substr(word.find("%")+1).length() > 0) {
					outFS << word.substr(word.find("%")+1) << " ";
				}
			}
			if (word.find("/") != string::npos) {
				outFS << word.substr(0, word.find("/")) << " " << "/" << " ";
				if (word.substr(word.find("/")+1).length() > 0) {
					outFS << word.substr(word.find("/")+1) << " ";
				}
			}
			if (word.find("$") != string::npos) {
				outFS << word.substr(0, word.find("$")) << " " << "$" << " ";
				if (word.substr(word.find("$")+1).length() > 0) {
					outFS << word.substr(word.find("$")+1) << " ";
				}
			}
			if (!containsPunct(word)) {
				outFS << word << " ";
			}
		}
		outFS << endl;
	}

	_filename = datafile;

	file.close();
	outFS.close();
}