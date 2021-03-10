/**
 * @file pigeon.cpp
 * Approximate Pattern Matching using pigeonhole
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <unordered_map>

#include "pigeon.h"

/**
 * Takes a filename and reads in all the text from the file
 * Newline characters are also just characters in ASCII
 * 
 * @param filename The name of the file that will fill the string
 */
std::string file_to_string(const std::string& filename){
  std::ifstream text(filename);

  std::stringstream strStream;
  if (text.is_open()) {
    strStream << text.rdbuf();
  }
  return strStream.str();
}

/**
 * Takes an input text and transforms it to a vector of all the kmers in that text.
 * A kmer is a fixed length substring (watch your edge cases!)
 * @param T A reference variable storing the input text
 * @param k The size of the kmer
 */
kmerMap text_to_kmer_map(std::string & T, int k){
  kmerMap outMap;
  if(k == 0) return outMap;
  for(int i=0; i< (int)T.size();i++){
    if(i+k -1 >= (int)T.size()) break;
    std::string sub = T.substr(i,k);
    auto lookup = outMap.find(sub);
    if(lookup != outMap.end()){
      outMap[sub].push_back(i);
    }
    else{
      std::vector<int> newvect;
      newvect.push_back(i);
      outMap[sub] = newvect;
    }
  }

  return outMap;
}

/**
 * Takes an input pattern and number of partitions and returns a vector of type Seed
 * 
 * Seed is a pair<string, int> containing the string partition and its starting index in P
 * The partitions are non-overlapping strings
 *
 * If there are excess characters (division is not a whole number)
 * distribute the characters round-robin starting with the first partition.
 * Ex: ABCD partitioned into three partitions is AB, C, D (The extra char goes to first partition)
 *
 * @param P The input pattern
 * @param numPart The number of partitions
 */
std::vector<Seed> partitionPattern(std::string P, int numPart){
  std::vector<Seed> outVec;
  int subsize  = P.size()/numPart;
  int rem = P.size()%numPart;
  int index = 0;
  for(int i=0;i<rem;i++){
    Seed pair;
    pair = std::make_pair(P.substr(index,subsize+1),index);
    outVec.push_back(pair);
    index = index +subsize+1;
  }
  while(index< (int)P.size()){
    Seed pair;
    pair = std::make_pair(P.substr(index,subsize),index);
    outVec.push_back(pair);
    index = index +subsize;
  }
  return outVec;
}

/**
 * Performs inexact pattern matching between input text and pattern
 * Returns a vector<int> of match index positions and offsets
 * You can implement this using any method you want.
 * You are strongly encouraged to use the Seed values and kmerMap
 * @param fname The input text file being indexed and then searched over
 * @param P The input pattern being searched for
 * @param mm The number of mismatches (defaults to one but others will be tested)
 */
std::vector<int> approximate_search(const std::string & fname, std::string P, int mm){
  std::vector<int> outList;
  std::string S = file_to_string(fname);
  //kmersize should match #of partitions!!! O(1) lookup
  std::vector<Seed> pts = partitionPattern(P,mm+1);
  int kmersize = pts[0].first.size();
  kmerMap map = text_to_kmer_map(S,kmersize);

  std::map<int,bool> pushed;
  //search for each seed in the kmer map
  for(Seed seed : pts){
    auto lookup = map.find(seed.first);
    //if found in map, have a match, now validate:
    if(lookup != map.end()){
      std::vector<int> Tmatches = lookup->second;
      //for EACH match of seed in T, validate
      for(int i : Tmatches){
        int mismatches = 0;
        int pindex = seed.second;
        int startidx = i-pindex;
        int endidx = startidx + P.size()-1;
        //case that cuts off front of P:
        if(startidx < 0){
          mismatches += abs(startidx)-1;
          startidx = 0;
        }
        //case that cuts off end of P:
        if(endidx >= (int)S.size()){
          continue;
          /*
          mismatches += S.size()- endidx +1;
          endidx = S.size()-1;
          */
        }
        int pdx = 0;
        for(int j = startidx;j<=endidx;j++){
          if(P[pdx] != S[j]){
            mismatches++;
          }
          pdx++;
        }
        if(mismatches <= mm){
          auto lookup = pushed.find(startidx);
          if(lookup == pushed.end()){
            pushed[startidx] = true;
            outList.push_back(startidx);
          }
        }
      }
    }
  }
 
  if(outList.empty()){
    outList.push_back(-1);
  }
  std::sort(outList.begin(),outList.end());
  return outList;
}

