/**
 * @file bwt.cpp
 * Code to encoding and decoding a text using the BWT
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <algorithm>
#include "bwt.h"

/**
 * Returns a vector of strings containing all rotations of a text
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An std::vector<std::string> storing all rotations
 */
std::vector<std::string> rotate(std::string T){
  std::string str = T + "$";
  std::vector<std::string> rotations;
  rotations.push_back(str);
  int length = str.length();
  //start at 1 since already pushed the string itself 
  for(int i = 1;i<length;i++){
    std::string rot = str.substr(1) + str[0]; //left rotation
    rotations.push_back(rot);
    str = rot;
  }
  return rotations;
}

/**
 * Returns the suffix array of T as an int vector.
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An std::vector<int> storing the suffix array
 */
std::string encode_bwt(std::string T){
  std::vector<std::string> rotations = rotate(T);
  //sort lexicographically:
  std::sort(rotations.begin(),rotations.end());
  std::vector<std::string>::iterator it;
  //iterate through vector, appending last character to bwt string
  std::string bwt;
  int lastchar = T.length();
  for(it= rotations.begin();it!=rotations.end();it++){
    bwt = bwt + (*it)[lastchar];
  }
  return bwt;
}

/**
 * Returns the suffix array of T as an int vector.
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An std::vector<int> storing the suffix array
 */

std::string decode_bwt(std::string bwt){
  //first sort given bwt
  std::string bwts = bwt;
  std::sort(bwts.begin(),bwts.end());
  std::vector<std::string> bwtmatrix;
  //initialize vector matrix with sorted bwt
  for(int i =0;i<(int)bwts.length();i++){
    std::string s;
    s = s + bwts[i];
    bwtmatrix.push_back(s);
  }
  //prepend bwt & sort until length of vector[0] matches length of bwt
  while((bwtmatrix.back()).length() != bwt.length()){
    //iterate through vector matrix, appending original bwt to the front
    std::vector<std::string>::iterator it;
    int index = 0;
    for(it = bwtmatrix.begin();it!=bwtmatrix.end();it++){
      (*it) = bwt[index] + (*it);
      index++;
    }
    //now sort matrix
    std::sort(bwtmatrix.begin(),bwtmatrix.end());
  }

  //finally, find string with last char = '$' this is original string
  std::vector<std::string>::iterator it;
  for(it=bwtmatrix.begin();it!=bwtmatrix.end();it++){
    if((*it)[bwt.length()-1] == '$'){
      std::string final = (*it).substr(0,bwt.length()-1);
      return final;
    }
  }
  std::string invalid = "Invalid";
  return invalid;
}
