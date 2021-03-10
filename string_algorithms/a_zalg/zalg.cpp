/**
 * @file zalg.cpp
 * Code to create Z-array and use Z-algorithm for exact pattern matching.
 */

#include <iostream>
#include <cmath>
#include <cstdlib>

#include "zalg.h"

/**
 * Returns the number of individual character comparisons used by the Z-algorithm
 * Also modifies the input (empty) zArray to store the Z-value at each position i<0
 *
 * @param S A std::string object which holds the String being analyzed.
 * @param Z An int* that points to a length |S| int[] which holds the zArray.
 *
 * @return An integer counting the number of character comparisons needed to make the Z-array.
 */
int create_zarray(std::string S, int* Z) {

  int charComps = 0; //the number of character comparisons needed to make the Z-array
  
  //Z[0] always zero
  Z[0] = 0;
  //check S is not empty
  if(S.length() == 0){
    return 0;
  }

  //initialize l and r of Z-box
  unsigned long l = 0, r =0;

  for(unsigned long i =1;i<S.length();i++){
    //case 1:
    if(i>r){
      int count =0;
      unsigned long curri = i;
      for(int j=0; j<S.length(); j++){
        charComps++;
        if(S[curri]!= S[j]){
          break;
        }
        count++;
        r = curri;
        l = i;
        curri++;
      }
      Z[i] = count;
    }
    //case 2:
    else{
      int k = i-l;
      int b = r-i+1;
      //case 2a:
      if(Z[k]<b){
        Z[i]=Z[k];
      }
      else{
        //case 2b:
        if(Z[k]== b){
          int count = 0;
          unsigned long curri = r+1;
          for(int j =r-l+1;j<S.length();j++){
            charComps++;
            if(S[curri]!=S[j]){
              break;
            }
            count++;
            r = curri;
            l = i;
            curri++;
          }
          Z[i] = Z[k]+ count;
        }
        else{
          //case 2c:
          Z[i] = b;
        }
      }
    }
  }
  return charComps;
}

/**
 * Returns the index positions of all exact matches of P in T.
 * If no match is found, returns a vector with one value '[-1]'
 * NOTE: We want the exact matches *in T* not string S.
 *
 * Search the Text string (T) for the Pattern string (P)
 * For example, if P = 'AAA' and T = 'BAAAT', will return 1
 *
 * @param P A std::string object which holds the Pattern string.
 * @param T A std::string object which holds the Text string.
 *
 * @return An std::vector<int> array containing ALL index matches.
 */
std::vector<int> zalg_search(std::string P, std::string T) {
  std::vector<int> outList;
  
  // NOTE: You should change this function as necessary to complete the assignment.
  std::string S = P+ "$" +T;
  
  int* zarr = new int[S.length()];
  int charComps = create_zarray(S, zarr);
  int plen = P.length();
  for(int i= 0;i<S.length();i++){
    //if Z value is length of P, exact match!
    if(zarr[i] == plen){
      outList.push_back(i - (plen+1));
    }
  }
  //std::cout << "charComps (search): " << charComps << std::endl;


  // NOTE: You may not have seen std::vector<int> before. It is a dynamic array, useful for 
  //       situations where you don't know how large to make your array allocation. 

  // No need to edit this line but you will need to add code to deal with the non-empty case
  if (outList.empty()) { // bool empty() const; returns if my dynamic array is empty.
    outList.push_back(-1); //push_back appends (adds) a value to the end of a dynamic array
  }

  delete[] zarr; // No memory leaks here!
  return outList;
}