// prog2.cpp
// Chase Worsley
// COSC 2030, Prog 2
// Last Edited: 2-27-2024
// Main file for prog2

#include "timer.h"
#include "myTree.h"

#include <iostream> // Normal input/output shenanigans
using std::cout;
using std::cerr;
using std::endl;

#include <fstream> // For reading and writing to files
using std::ifstream;
using std::ofstream;

#include <string> // For words
using std::string;

#include <vector> // Only used for storing misspelled words
using std::vector;

string cleanWord(const string& word); // Function that cleans any string passed to it so the output only contains: lowercase letters, numbers, and apostrophes.
void readDic(myTree<string>& dict, string dictName); // Reads dictionary into data structure (data), using dictName as source file name
void writeMisspelled(const string& misspelledName, const vector<string>& misspelledWords); // Writes misspelled words to file name at top of main

void readBook(myTree<string>& dictionary, string bookName, int& numCorrectWords, long int& numCorrectCompares, int& numWrongWords, long int& numWrongCompares, int& numWordsSkipped, vector<string>& misspelledWords);
/* 
* Parameter breakdown for readBook function 
*  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
* dictionary: Name of variable for dictionary data structure | bookName: Name of the file the book is stored as
* numCorrectWords: The variable that will be output for number of correct words in the book, correct meaning it exists in the supplied dictionary
* numCorrectCompares: The variable that will be output for optimization's sake and the average, sum of amount of compares the find method does for correct words
* numWrongWords: Same as numCorrectWords, but for incorrect words
* numWrongCompares: Same as numCorrectCompares, but for incorrect words
* numWordsSkipped: The variable to be output that keeps track of amount of words skipped by the readBook function
* misspelledwords: vector/list of the actual strings that were misspelled, is later output to a file
* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
*/
int main() {
    myTree<string> dict; // tree intialization
    vector<string> misspelledWords; // Stores words that were skipped by readBook() for later file output

	string dictName = "dict.txt"; // Names for input/output files
	string bookName = "book.txt"; 
	string misspelledName = "misspelled.txt"; 

	int numCorrectWords = 0; // Intializes in0teger variables for later use
	long int numCorrectCompares = 0;
	int numWrongWords = 0;
	long int numWrongCompares = 0;
	int numWordsSkipped = 0;

    readDic(dict, dictName); // Read the dictionary.txt into the tree
	Timer timer;
	timer.Start(); // Start timer
	readBook(dict, bookName, numCorrectWords, numCorrectCompares, numWrongWords, numWrongCompares, numWordsSkipped, misspelledWords); // Primary function, reads the book and sends it to find method
	timer.Stop(); // End timer

	int numCorrectAvg = (numCorrectCompares == 0 && numCorrectWords == 0) ? (0) : (numCorrectCompares / numCorrectWords); // Just for calculating averages, also takes into account 0's for either value
	int numWrongAvg = (numWrongCompares == 0 && numWrongWords == 0) ? (0) : (numWrongCompares / numWrongWords);

	cout << "Dictionary Size: " << dict.getSize() << endl; // Lots of output shenanigans
	cout << "Done checking and these are the results" << endl;
	cout << "Finished in time: " << timer.Time() << endl;
	cout << "There are " << numCorrectWords << " words found in the dictionary" << endl;
	cout << "          " << numCorrectCompares << " compares. Average: " << numCorrectAvg << endl;
	cout << "There are " << numWrongWords << " words NOT found in the dictionary" << endl;
	cout << "          " << numWrongCompares << " compares. Average: " << numWrongAvg << endl;
	cout << "There are " << numWordsSkipped << " words not checked" << endl;

	writeMisspelled(misspelledName, misspelledWords); // Writes all misspelled words to output file "misspelledName" provided above

    return 0;
}

string cleanWord(const string& input) { // Cleans the word provided, DOES NOT CHANGE PROVIDED WORD
	string output;
	for (char c : input) { // Checks each character, adds lowercase letter to output and apostrophes
		if (isalnum(c) || c == '\'') {
			output += tolower(c);
		}
	}
	return output;
}

void readDic(myTree<string>& dict, string dictName) { // Reads dictionary into our data structure
	ifstream file(dictName);
	if (!file.is_open()) { // In case of file missing or file errors
		cerr << "File not open, fatal error" << endl; return; // Error in case of file opening malfunction
	}

	string temp;
	while (file >> temp) { // Reads word by word into temp
		string cleanedWord = cleanWord(temp); // Ensures word is clean
		if (!cleanedWord.empty()) {
			dict.insert(cleanedWord); // Add temp/cleaned word to dictionary linked list
		}
	}
	return;
}

void readBook(myTree<string>& dictionary, string bookName, int& numCorrectWords, long int& numCorrectCompares, int& numWrongWords, long int& numWrongCompares, int& numWordsSkipped, vector<string>& misspelledWords) {
	ifstream file(bookName);
	if (!file.is_open()) { // In case of file missing or file errors
		cerr << "File not open, fatal error" << endl; return; // Error in case of file opening malfunction
	}

	string temp;
	while (file >> temp) { // Reads word by word
		string cleanedWord = cleanWord(temp); // Cleans word
		long int numComparesForWord = 0; // Resets to 0 to avoid artifically large compare results
		if (!cleanedWord.empty()) { // Only search if word isn't empty (shouldn't be empty anyways, good to be safe)
			if (isdigit(cleanedWord[0]) || cleanedWord[0] == '\'') { // Skips word if first character is an apostrophe or number
				numWordsSkipped += 1; // Adds 1 to count of words skipped

			}
			else {
				if (dictionary.find(cleanedWord, numComparesForWord)) {
					numCorrectWords++;
					numCorrectCompares += numComparesForWord; // Adds numComparesForWord to numCorrectCompares (total amount of compares for correct words)
				}
				else {
					misspelledWords.push_back(cleanedWord); // Adds misspelled word to vector for output later
					numWrongWords++;
					numWrongCompares += numComparesForWord; // Adds numComparesForWord to numWrongCompares (total amount of compares for incorrect words)
				}
			}
		}
	}
	return;
}
void writeMisspelled(const string& misspelledName, const vector<string>& misspelledWords) { // Creates and opens a file named via the misspelledName parameter and writes all words in misspelledWords vector to the file
	ofstream misspelledFile (misspelledName, ofstream::out);
	if (misspelledFile.is_open()) {
		for (string s : misspelledWords) {
			misspelledFile << s << endl;
		}
		misspelledFile.close(); // Ensures file closes properly
	}
	else {
		cerr << "Could not open " << misspelledName << endl;
	}
}