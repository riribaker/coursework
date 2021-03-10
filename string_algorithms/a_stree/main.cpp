/**
 * @file main.cpp
 * A simple C++ program for running NaryTree search.
 */

#include <string>
#include <iostream>
#include <vector>
#include <map>

//#include "strie.h"
#include "stree.h"

int main() {

  std::cout << "example 1" << std::endl;

  std::string P = "ABB";
  std::string T = "BABAAB";

  NaryTree myTree = build_strie(T);

  myTree.print();
  myTree.trie_to_tree();
  myTree.print();

  std::cout << "Height: " << myTree.height() << std::endl;

  std::vector<int> lV = myTree.patternMatch("BAB");


  for(size_t i = 0; i < lV.size(); ++i){
    std::cout  << "Match at: " << lV[i] << std::endl;
  }

  std::vector<std::string> treeV; 
  

  myTree.inLexOrder(treeV);

  std::cout << "{ ";
  for(size_t i = 0; i < treeV.size(); ++i){
    std::cout  << "\"" << treeV[i] << "\" , ";
  }
  std::cout << "}" << std::endl;

  std::cout << "example 2" << std::endl;

  NaryTree mt2;

  mt2.insert("AB$",0);
  //mt2.print();
  mt2.insert("ABAB$",1);
  //mt2.print();
  mt2.insert("ABBBA$",3);
  //mt2.print();
  mt2.insert("BAB$",0);
  //mt2.print();
  mt2.insert("BBB$",0);
  //mt2.print();

//mt2.trie_to_tree();

//mt2.print();

lV = mt2.patternMatch("AB");

  std::cout << "{ ";
  for(size_t i = 0; i < lV.size(); ++i){
    std::cout  << lV[i] << ", ";
  }
  std::cout << "}" << std::endl;


  return 0;
}
