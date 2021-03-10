/**
 * @file fmi.cpp
 * Code to constructiong and searching a FM Index
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <algorithm>

#include "fmi.h"

/**
 * Returns a vector of strings containing all rotations of a text
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An std::vector<std::string> storing all rotations
 */
std::vector<std::string> rotate(std::string T){
  //from a_bwt
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
* FM Index constructor; Given T, creates:
* a Burrows Wheeler Transform (BWT)
* a suffix array (SA)
* an occurence table (OT)
* @param T The string being preprocessed into an FM Index
* @param alpha The alphabet we are building our index around
* @param sr The sample rate for the SA and OT (Optional)
*/
FMI::FMI(std::string T, std::string a, int sr){
  sampleRate = sr; //A purely optional protected variable
  alpha = a;

  // Alphabet is sorted to minimize debug difficulties
  // Also makes code easier
  std::sort(alpha.begin(), alpha.end()); 

  // Your code must implement all three of these
  // Autograder will check each individually
  BWT = buildBWT(T);
  SA = buildSA(T);
  OT = buildOT(BWT);
}


/*
* Destructor for FM Index
*/
FMI::~FMI()
{
    /* nothing unless you added more variables */
}

/**
 * Returns the SA of T as a vector of ints
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An std::vector<int> storing the suffix array
 */
std::vector<int> FMI::buildSA(std::string T){
  std::vector<int> sarray;

  std::vector<std::pair<std::string, int>> sufList;
  
  T = T + "$";
  size_t tlen = T.size();

  for (size_t i = 0; i < tlen; ++i){
    std::string t_suf = T.substr(i);
    sufList.push_back(make_pair(t_suf, i));
  }

  std::sort(sufList.begin(),sufList.end()); 

  for (size_t i = 0; i < tlen; ++i){
      sarray.push_back(sufList[i].second);
  }

  return sarray;
}


/**
 * Returns the BWT of T as a string
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An std::string storing the BWT
 */
std::string FMI::buildBWT(std::string T){

  //from a_bwt
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
 * Returns the Occurrence Table of a BWT (A 2D table of tsize x alphabet)
 *
 * @param T A std::string object which holds the text being pre-processed.
 *        T in this instance is *usually* the BWT. 
 *        Other strings may be tested by the autograder
 *
 * @return An std::vector<std::vector<int>> storing the cumulative count
 *         of each character 
 *
 *         rows are alphabet in length (one row per individual index positions)
 */

std::vector<std::vector<int>> FMI::buildOT(std::string T){
  std::vector<std::vector<int>> OT;
  
  std::string::iterator it;
  std::string::iterator it2;

  int asize = alpha.size();
  std::vector<int> count;
    for(int i=0;i<asize;i++){
      count.push_back(0);
  }

  for(it = T.begin();it!=T.end();it++){
    std::vector<int> letter;
    for(int j =0; j<asize;j++){
      if(*it == alpha[j]){
        count[j]++;
      }
      letter.push_back(count[j]);
    }
    OT.push_back(letter);
  }

  return OT;
}

/**
* FM Index First Range Search
* Gives the index start and end position for the character 
* (and rank range) being searched
* 
* @param c The character being looked up
* @param s_rank The starting rank being searched 
* @param e_rank The ending rank being searched 
*
* HINT: You will have to reconstruct F from the BWT or the OT
*/
std::pair<int, int> FMI::get_frange(std::string c, int srank, int erank){
  std::pair<int, int> out;
  //b0b1b2b3b4 for (s_rank 1, end_rank 4) would look at b1b2b3b4
  //returns startindex, endindex in F for given range, does not check next character
  
  //reconstruct F from BWT:
  std::string F = BWT;
  std::sort(F.begin(), F.end());
  
  int range =0;
  if(srank != erank){
    range = erank - srank;
  }
  
  int rank0index = F.find(c);
  if(rank0index != -1){
    out.first = rank0index + srank;
    out.second = out.first + range;
    return out;
  }

  //if not found:
  out.first = -1;
  out.second = -1;
  return out;
}

/**
* FM Index Last Range Search
* Gives the number of occurrences of character *before* s_index (s_rank)
* and the number of occurrences of character at the e_index 
* 
* @param c The character being looked up
* @param s_index The starting index being searched 
* @param e_index The ending index being searched 
*
* HINT: L is the BWT, s_index *cannot* be 0. Why?
*/
std::pair<int, int> FMI::get_lrange(std::string c, int s_index, int e_index){
  std::pair<int, int> out;
  //first find character in alphabet:
  int col = alpha.find(c);
  
  //now search for occurences in that column
  out.first = OT[s_index -1][col];
  out.second = OT[e_index][col];

  return out;
}


/**
 * Performs exact pattern matching on the FM Index for input P
 *
 * @param P A std::string object which holds the pattern being searched.
 *
 * @return An std::vector<int> storing the index positions of matches
 *         or returns {-1} if there is no match
 */
std::vector<int> FMI::search(std::string P){
  std::vector<int> outList;

  std::string c = P.substr(P.size()-1,1);

  int letter = alpha.find(c);
  
  // If letter not in alphabet, stop searching
  if (letter == -1){
    outList.push_back(-1);
    return outList;
  }
  
  // F column can be represented by the occurrence # of each character
  std::vector<int> F = OT.back();

  // Our first occurrence of char c has rank 0
  // Our last occurrence of char c has rank # occurrences - 1
  // @TODO: REPLACE THESE VALUES WITH THE APPROPRIATE STARTING VALUES
  int s_rank = 0;
  int numoccur = OT[BWT.size() -1][letter];
  int e_rank = numoccur -1; //HINT: How do I know the # occurrences?

  // If my last occurrence has rank -1, it means I never saw the character at all.
  if (e_rank == -1){
    outList.push_back(-1);
    return outList;
  }

  // Get the initial F range to search (Index start - Index end)
  // You will have to code get_frange to match this code
  std::pair<int,int> frange = get_frange(c, s_rank, e_rank);

  // FMI logic finds range of occurrences (using L), then finds FM index range to search (in F)
  // In other words:
  // lrange outputs # of occurrences before start index and at end index
  // These occurrence numbers determine the range start and end values, used by frange.
  //
  // frange outputs index start and end
  // These index positions are used by lrange directly
  // 
  // The final frange (index start and end) tells us which suffix array values to look up
  for(int i = P.size()-2; i >= 0; i--){
    c = P.substr(i,1);

    letter = alpha.find(c);

    //Gets the start and end rank values for c_{i-1} based on index start and end for c_{i}
    // (Recall that we are starting from i = size()-1)
    // You will have to code get_lrange to match this code
    std::pair<int,int> lrange = get_lrange(c, frange.first, frange.second);

    // @TODO: REPLACE THIS VALUE WITH THE APPROPRIATE VALUES
    int numMatches = lrange.second - lrange.first;

    // If lrange returns the same value start to end, we have no characters in range.
    if(numMatches <= 0){
      outList.push_back(-1);
      return outList;
    }

    // Given our range values we want the index values
    // Recall that lrange outputs occurrences not ranges
    // The occurrence # before our first index is our first rank -- why?
    // The occurrence # at our last position MINUS 1 is our last rank -- why?
    frange = get_frange(c, lrange.first, lrange.second - 1);

    if( frange.first == -1 || frange.second == -1){
      outList.push_back(-1);
      return outList;
    }
  }
  
  //find index in T using SA, push to outList
  for(int i = frange.first; i<=frange.second;i++){
    outList.push_back(SA[i]);
  }

  return outList;
}