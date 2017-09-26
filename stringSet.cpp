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

/* Inserts a new key.  It is an error if key is already in the set. */
int Stringset::insertHead(string key, Node**& table)
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
			std::cout << temp->key << std::endl;
			while (temp->next) {
				temp = temp->next;
				std::cout << temp->key << temp->weight << std::endl;
			}
		}
	} //end for

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
}//end function

void Stringset::readFile() {

	string http = "four"; //allocate a temp string size 4
	ifstream in;
	string s;

	in.open("C:/Users/Ty/Desktop/proj1/fakewebpages.txt");

	if (!in) {
		std::cout << "file not opened\n";
	}

	//get the size of the array
	while (in) {
		in >> s;
		if (s == "NEWPAGE")
			size++;
	}
	in.close();
	table = allocate_table(size);
	webPages = allocate_table(size);

	in.open("C:/Users/Ty/Desktop/proj1/fakewebpages.txt");
	while (in) {
		in >> s;
		///READ ALL OF THE WEBPAGES IN
		if (s == "NEWPAGE") { //new webpage encountered
			//read in the link as the head.. hash to get position
			in >> s;
			index = insertHead(s, webPages); //webpages
			index = insertHead(s, table); //words
			webPages[index]->weight = 1 / size; //all weights set to default
		}
	}
	in.close();
	in.open("C:/Users/Ty/Desktop/proj1/fakewebpages.txt");

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
					for (int i = 0; i < 4; i++)
						http[i] = s[i];

					if (http == "http") {
						//if it hashes to a key in the index
						if (webPages[hashs(s, size)]->key == s) insert(s, index, webPages);
						else in.ignore();
					}
					else
						//insert an h word that's not http
						insert(s, index, table);
				}
				else
					//insert word, if not already in word array
					if (!find(s))
						insert(s, index, table);
			}




		}


	}
	in.close();
}

void Stringset::pageRank() {

	//do this 50 times
	for (int u = 0; u < 50; u++) {
		//loop thru each webpage
		for (int i = 0; i < size; i++) {
			webPages[i]->new_weight = 0.1 / size;
			//for each link on the [i] webpage
			int t = 0; //keep count of linked web pages
			for (Node* temp = webPages[i]; temp != NULL; temp = temp->next) {
				++t;
				temp->new_weight *= (0.9 * temp->weight / t);
			}
		webPages[i]->weight = webPages[i]->new_weight;
		}
	}
	


}