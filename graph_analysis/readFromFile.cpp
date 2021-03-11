//readFromFile.cpp
#include "readFromFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <iterator>

//based off AMA slides
string file_to_string(const string & filename){
    ifstream text(filename); //file stream
    stringstream strStream;  //string stream

	if (!text){
		std::cout<< "Could not open file " << filename << endl;
    }

    if(text.is_open()) strStream << text.rdbuf();
    //return string copy of buffer
    return strStream.str();
}

//based off AMA slides
vector<string> file_to_vector(const string & filename){
    ifstream text(filename); //file stream
	vector<string> out;      //output vector

	if (!text){
		std::cout<< "Could not open file " << filename << endl;
    }

	if (text.is_open()) {
		istream_iterator<string> it(text);
        //push 'chunks' of strings to vector
		while (!text.eof()) {
			out.push_back(*it);
			++it;
		}
	}

	return out;
}