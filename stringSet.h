#ifndef STRINGSET_H /* Prevent multiple inclusion... */
#define STRINGSET_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

struct Node {
	string key;
	Node *next;
	Node(string k, Node *n) { key = k; next = n; count = 1; }
	Node() { key = ""; next = NULL; count = 0; }
	int count;
	int weight;
	int new_weight;
};


class Stringset {

private:
	
	int index;

	Node **table;  // array of words
	Node **webPages; //array of webpages
	int size;      // size of table, as currently allocated
	int num_elems; // number of elements stored in the table

public:

	inline int getSize() { return size; }
	Stringset();
	~Stringset();
	void readFile();
	bool find(string key);
	void insert(string key, int, Node**&);
	int insertHead(string key, Node**&);
	void print(void);
	int& operator[](string key);
	void pageRank();
};


#endif