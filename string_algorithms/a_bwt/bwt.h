/**
 * @file bwt.h
 * Declarations of BWT functions.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

// This function might help
std::vector<std::string> rotate(std::string T);
std::string getMinLex(std::vector<std::string> &rotations);

// Your assignment is to build these two functions
std::string encode_bwt(std::string T);
std::string decode_bwt(std::string T);