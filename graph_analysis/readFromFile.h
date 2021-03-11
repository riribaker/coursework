//readFromFile.h
#pragma once 

#include <string>
#include <vector>

using namespace std;

/**
 * Function that takes input file and outputs data as string.
 * @param filename - input file
 * @return string of file's data
 */
string file_to_string(const string & filename);

/**
 * Function that takes input file and outputs data as vector of strings.
 * @param filename - input file
 * @return vector of strings of file's data
 */
vector<string> file_to_vector(const string & filename);
