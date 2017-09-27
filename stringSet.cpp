#include <iostream>
#include <string.h>
#include <assert.h>
#include "stringset.h"

//there are two arrays, one that holds webpages
//one that hold words

using namespace std;

/* Return a hash for the string s in the range 0..table_size-1 */
int hashs(string s, int table_size)
{
	unsigned int i, h = 0;
	for (i = 0; i<s.length(); i++)
		h = (h * 2917 + (unsigned int)s[i]) % table_size;
	return h;
}

void allocateIDStruct(idStruct*& table){
	table = new idStruct;
}

/* Allocate a table of pointers to nodes, all initialized to NULL */
Node **allocate_table(int size)
{
	Node **table = new Node *[size];
	for (int i = 0; i<size; i++)
		table[i] = NULL;
	return table;
}

int BAD_RESULT = 0;

int& Stringset::operator[](string key) {

	int h = hashs(key, size);
	Node* n = table[h];
	while (n) {
		if (n->key == key) { return n->count; }
		n = n->next;
	}
	return BAD_RESULT;
}

Stringset::Stringset()
{   
	table = allocate_table(size);
	num_elems = 0;
}

Stringset::~Stringset()
{
	for (int i = 0; i<size; i++) {
		while (table[i] != NULL) {
			Node *temp = table[i];
			table[i] = table[i]->next;
			delete temp;
		}
	}
	delete[] table;
}

/* Return true if key is in the set */
bool Stringset::find(string key)
{
	int h = hashs(key, size);
	Node *n = table[h];
	while (n != NULL) {
		if (n->key == key) return true;
		n = n->next;
	}
	return false;
}

void Stringset::insert(string key, int position, Node**& table) {
	//if (find(key)) return;
	//add at tablePosition
	Node* temp = table[position];
	while (temp) {
		if (temp->next == NULL){
			temp->next = new Node(key, temp->next); //add at the end
			break;
		}
		temp = temp->next;
	}
}

/* Inserts a new ID.  It is an error if key is already in the set. */
void Stringset::insertID(int id, idStruct* table) {

	if (!table) { //meaning it has not been initialized...
		table = new idStruct(id, table); //make head
		return;
	}

	else {
		idStruct *temp = table;
		while (temp) {
			if (temp->next == NULL) {
				temp->next = new idStruct(id, temp->next); //add at the end
				break;
			}
			temp = temp->next;
		}
	}
}

/* Inserts a new key.  It is an error if key is already in the set. */
int Stringset::insertHead(string key, Node**& table)
{
	table[hashs(key, size)] = new Node(key, table[hashs(key, size)]);
	return hashs(key, size);
}

/* Inserts a new word.  It is an error if key is already in the set. */
int Stringset::insertWord(string key, Node**& table, int size)
{
	table[hashs(key, size)] = new Node(key, table[hashs(key, size)]);
	return hashs(key, size);
}

void Stringset::print(void)
{
	//print webpages
	for (int i = 0; i < size; i++) {
		Node* temp = webPages[i];
		if (temp) {
			std::cout << temp->key << (webPages[i]->weight) << std::endl;
			while (temp->next) {
				temp = temp->next;
				std::cout << temp->key << std::endl;
			}
		}
	} //end for
	/*
	//print words
	for (int i = 0; i < size; i++) {
		Node* temp = table[i];
		if (temp) {
			//skip head, it's the newpage url
			while (temp->next) {
				temp = temp->next;
				std::cout << temp->key << std::endl;
			}
		}
	} //end for
	*/

}//end function

void Stringset::readFile() {

	wordCount = 0;
	string http = "four"; //allocate a temp string size 4
	ifstream in;
	string s;

	in.open("C:/Users/Ty/Desktop/proj1/fakewebpages.txt"); //PC
	//in.open("C:/Users/ty/Desktop/labs/proj1/fakewebpages.txt"); //laptop

	if (!in) {
		std::cout << "file not opened\n";
	}

	cout << "\ngetting size...\n";
	//get the size of the array
	while (in) {
		in >> s;
		if (s == "NEWPAGE")
			size++;
	}
	in.close();

	table = allocate_table(size);
	webPages = allocate_table(size);

	cout << "\nwebPages and Table...";
	in.open("C:/Users/Ty/Desktop/proj1/fakewebpages.txt"); //PC
	//in.open("C:/Users/ty/Desktop/labs/proj1/fakewebpages.txt"); //laptop

	while (in) {
		in >> s;
		///READ ALL OF THE WEBPAGES IN
		if (s == "NEWPAGE") { //new webpage encountered
			//read in the link as the head.. hash to get position
			in >> s;
			index = insertHead(s, webPages); //webpages
			index = insertHead(s, table); //words
			//webPages[index]->weight = 1 / size; //all weights set to default
		}
	}
	in.close();

	in.open("C:/Users/Ty/Desktop/proj1/fakewebpages.txt"); //PC
	//in.open("C:/Users/ty/Desktop/labs/proj1/fakewebpages.txt"); //laptop

	cout << "\nreading words...\n";

	while (in) {
		in >> s;

		if (s == "NEWPAGE") { //new webpage encountered
			in >> s;				  //read in the link as the head.. hash to get position
			index = hashs(s, size); //these are already read in
		}
		else {
			if (s != "NEWPAGE") {
				//in >> s;

				//if https is read in w.o a new page then ignore it
				if (s[0] == 'h') {
					for (int i = 0; i < 4; i++) {
						if (s[i])
							http[i] = s[i];
						else break;
					}
					if (http == "http") {
						//if it hashes to a key in the index
						if (webPages[hashs(s, size)]) {
							if (webPages[hashs(s, size)]->key == s) insert(s, index, webPages);
							else in.ignore();
						}
					}
					else {
						//insert an h word that's not http
						insert(s, index, table);
						++wordCount;
					}
				}
				else {
					//insert word, if not already in word array
					//if (!find(s))
						insert(s, index, table);
						++wordCount;
				}
			}




		}


	}
	in.close();
	iIndex = allocate_table(wordCount);
}

void Stringset::pageRank() {

	for (int k = 0; k < size; k++) { //set defaults
		webPages[k]->weight = (1.0 / size);
	}

	//do this 50 times
	for (int u = 0; u < 50; u++) {
		int t = 0;//keep count of linked web pages
		//loop thru each webpage
		for (int i = 0; i < size; i++) {
			webPages[i]->new_weight = (0.1 / size);
			//for each link on the [i] webpage
			for (Node* temp = webPages[i]; temp != NULL; temp = temp->next) {
				t++; //get total links on page
			}
			for (Node* temptwo = webPages[i]->next; temptwo != NULL; temptwo = temptwo->next) {
				int index = hashs(temptwo->key, size);
				webPages[index]->new_weight += (0.9 * webPages[index]->weight / t); //add the new weight for each link on the list
			}
		webPages[i]->weight = webPages[i]->new_weight;
		}
	}
	


}

void Stringset::invertedIndex() {

	//read into word array
	for (int i = 0; i < size; i++) {
		Node* temp = table[i];
		if (temp) {
			//get the id of the url the words belong to
			int idStorage = hashs(temp->key, size);
			while (temp->next) {
				temp = temp->next;
				int index = insertWord(temp->key, iIndex, wordCount);
				if (!iIndex[index]->id)
					allocateIDStruct(iIndex[index]->id);
				insertID(idStorage, iIndex[index]->id);
			
			}
		}
	} //end for
	
	//now add the urls if the word appears on their page, back to table
	//the word's id the hashed index of the webpage it belongs to
	
		int x = 0;
		string s;

		while (s != "quit") {
			cout << "\nEnter a word to be found\n";
			cin >> s;
			int b = hashs(s, wordCount); //hash to find the word in word table
			if (iIndex[b]) {
				idStruct* traverse = iIndex[b]->id->next;
				while (traverse) { //find all the urls associated with that word
					//check id
					//int lookup = iIndex[b]->id[x];
					s = table[traverse->index]->key; //you now have the webpage
					index = hashs(s, size);
					cout << webPages[index]->weight << " " << s << "\n";
					traverse = traverse->next;
					++x;
				}
			}
			else cout << "\n not in structure\n";
		}

}
