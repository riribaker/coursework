/**
 * @file bmoore.cpp
 * Code to Boyer-Moore solution for exact pattern matching.
 * Uses strong bad character & weak good suffix rule
 */

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>

#include "bmoore.h"

/**
 * Returns the number of individual character comparisons used by the Z-algorithm
 * Also modifies the input (empty) zArray to store the Z-value at each position i<0
 *
 * @param S A std::string object which holds the pattern being pre-processed.
 * @param alpha An std::string object which holds all characters in the alphabet of P and T.
 *
 * @return An std::vector<std::vector<int>> (a 2D array of integers) storing the badCharacter array
 * BC_array rows coresspond to the alphabet and the columns correspond to positions along the pattern.
 */
std::vector<std::vector<int>> prep_bc_array(std::string P, std::string alpha)
{

  // This character lookup dictionary *might* be useful to your preprocessing.
  // You don't have to use it.
  std::map<char, int> charLookup;
  std::map<char, int>::iterator it = charLookup.begin();

  // This 2D array will be necessary to your solution.
  // Remember that <vname>.push_back(i) will add i to the end of <vname>.
  // Further tip: This is a vector of (vectors of integers).
  //create 2d vector with initialized size such that I do not have to worry about pushing vector arrays
  std::vector<std::vector<int>> bc_array(alpha.length(), std::vector<int>(P.length(), 0));

  //iterate through P and for each letter of P, iterate through alphabet vector
  int skips;
  for (unsigned int i = 0; i < P.length(); i++)
  {
    for (unsigned int j = 0; j < alpha.length(); j++)
    {
      //set iterator to map value that holds nearest index of character in alphabet
      it = charLookup.find(alpha[j]);
      //if iterator = map.end then no character in map, set skips to character index in P
      if (it == charLookup.end())
      {
        skips = i;
      }
      //number of skips = index of character - next instance of character in P - 1
      else
      {
        skips = i - it->second - 1;
      }
      //set value in 2d array equal to number of skips
      bc_array[j][i] = skips;
    }
    //initialize iterator such that can differentiate between when character exists
    //in map and when it doesn't
    std::pair<std::map<char, int>::iterator, bool> ins;
    ins = charLookup.insert(std::pair<char, int>(P[i], i));
    //if iterator->second is false then character already exists, so use [] operator to
    //change its value
    if (ins.second == false)
    {
      charLookup[P[i]] = i;
    }
  }
  //iterated through P, return 2d array of skip values
  return bc_array;
}

/**
 * Returns the index positions of first instance of c in alpha.
 * If no match is found, returns the value '-1'
 *
 * A potentually useful helper function for relating characters to integers
 * As a fun fact, this was a valid solution to naive_search (Week 1)
 *
 * @param alpha A std::string object which holds the alphabet string.
 * @param c A char object which holds the character being searched for
 *
 * @return An integer containing the index position of c in alpha
 */
int findChar(std::string alpha, char c)
{
  return alpha.find(c);
}

/**
 * Returns the number of alignments skipped by Boyer-Moore
 * In this instance, Boyer-Moore is *only* the strong bad character rule [and right-to-left scanning]
 *
 * Also modifies the outlist vector to contain the index positions of all exact matches of P in T.
 * If no match is found, modifies the vector to contain one value '[-1]'
 *
 * @param P A std::string object which holds the Pattern string.
 * @param T A std::string object which holds the Text string.
 * @param alpha A std::string object which holds the Alphabet string.
 * @param outList An std::vector<int> array (by reference) that can be modified to contain all matches
 *
 * @return An int counting the number of skipped alignments using bad character.
 */
int bmoore_search(std::string P, std::string T, std::string alpha, std::vector<int> &outList)
{
  int totalskips = 0;

  // Pre-compute BC array
  // Your solution must use this array.
  std::vector<std::vector<int>> bc_array = prep_bc_array(P, alpha);

  //store last index of each string
  int tailP = P.length() - 1;
  int tailT = T.length() - 1;

  //case in which P is greater than T
  if(tailP>tailT){
    outList.push_back(-1);
    return 0;
  }
  //iterate within bounds of T
  for (int i = tailP; i <= tailT; i++){
    int j = tailP;
    //character in P does not match T
    if (P[j] != T[i]){
      int maxskips = tailT - i - 1;
      int alphaindex = findChar(alpha, T[i]);
      int bcskips = bc_array[alphaindex][j];
      //only skip as far as possible within T
      int skips = bcskips > maxskips ? maxskips : bcskips;
    
      if(bcskips> maxskips){
        skips = tailT - i;
      }
      //if maxskips =0, but technically still have skip case:
      if(maxskips ==0 && bcskips!=0){
        skips = bcskips > (maxskips+1) ? (maxskips+1) : bcskips;
        totalskips += skips;
      }
     
      //move i to next value to check
      if(maxskips>0){
        i = i + skips;
        //add skips to total skips
        totalskips += skips;
      }
      
    }
    else{
      //character in P matches T
      int t = i; //copy i value to iterate separately
      int p = j; //copy j value to iterate separately
      while (P[p] == T[t]){
        //if reach end of P, full match!
        if (p == 0){
          outList.push_back(t);
          break;
        }
        //decrement p and t 
        --p;
        --t;
      }
      int maxskips = tailT - t - 1;
      int alphaindex = findChar(alpha, T[t]);
      int bcskips = bc_array[alphaindex][p];
      //only skip as far as possible within T
      int skips = bcskips > maxskips ? maxskips : bcskips;
      
      
      //if maxskips =0, but technically still have skip case:
      if(maxskips ==0 && bcskips!=0){
        skips = bcskips > (maxskips+1) ? (maxskips+1) : bcskips;
        totalskips += skips;
      }
     
      if(bcskips> maxskips){
        skips = tailT - t;
      }
      
      //if maxskips greater than 0, then increment i to next check 
      if(maxskips>0){
        i = i + skips;
        //add skips to total skips
        totalskips += skips;
      }
      
    }
  }

  //if no matches found, push -1 
  if(outList.empty()){
    outList.push_back(-1);
  }

  return totalskips;
}