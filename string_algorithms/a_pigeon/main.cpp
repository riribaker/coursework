/**
 * @file main.cpp
 * A simple C++ program for encoding and decoding a text using the BWT.
 */

#include <string>
#include <iostream>
#include <vector>
#include <map>

#include "pigeon.h"

int main() {

  std::string fname = "tests/pub1.txt";
  std::string P = "ABBA";
  int mm = 1;

  std::string T = "ABCDEFG";
  kmerMap outMap = text_to_kmer_map(T,4);
  for(kmerMap::iterator it = outMap.begin(); it != outMap.end(); ++it){
    std::cout << it->first << " : { ";
    std::vector<int> indList = it->second;
    for(size_t i = 0; i < indList.size(); ++i){
      std::cout << indList[i] << " ";
    }
    std::cout << "}" << std::endl;
  }

  std::vector<Seed> outPart = partitionPattern(P, 2);
  std::cout << "Partition List:" << std::endl;
  for(size_t i = 0; i < outPart.size(); ++i){
    std::cout << "{ " << outPart[i].first << ", " << outPart[i].second << " }" << std::endl;
  }

  std::vector<int> output = approximate_search(fname, P, mm);

  std::cout << "Search results: " << std::endl;
  std::cout << "{ ";
  for(size_t i = 0; i < output.size(); ++i){
    std::cout  << output[i] << ", ";
  }
  std::cout << "}" << std::endl;

  


  return 0;
}
