/**
 * @file narytree.cpp
 * Definitions of the n-ary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

#include "narytree.h"

NaryTree::NaryTree()
    : root(NULL)
{ /* nothing */
}

NaryTree::~NaryTree()
{
    clear(root);
}

void NaryTree::clear()
{
    clear(root);
    root = NULL;
}

/**
 * @return The root of the Nary tree.
 */

typename NaryTree::Node *NaryTree::getRoot() const
{
    return root;
}

/**
 * @return Prints a text representation of the edge connections of the NaryTree
 */

void NaryTree::print()
{
    if (root == NULL)
    {
        std::cout << "x" << std::endl;
    }
    else
    {
        std::cout << "o" << std::endl;
        std::vector<bool> dflag(this->height(), false);
        printHelper(root, 0, dflag);
        std::cout << std::endl;
    }
}

void NaryTree::printHelper(Node *&node, int depth, std::vector<bool> dflag)
{

    std::map<char, Node *>::iterator it;
    std::vector<char> keyList;
    for (it = node->children.begin(); it != node->children.end(); it++)
    {
        keyList.push_back(it->first);
    }

    std::sort(keyList.begin(), keyList.end());

    for (size_t i = 0; i < keyList.size(); ++i)
    {
        std::string spacing = "";

        for (int j = 0; j < depth; ++j)
        {
            if (dflag[j])
            {
                spacing += "|     ";
            }
            else
            {
                spacing += "      ";
            }
        }

        std::cout << spacing;

        std::cout << "+--" << keyList[i] << "--o" << std::endl;

        if (i < keyList.size() - 1)
        {
            dflag[depth] = true;
        }
        else
        {
            dflag[depth] = false;
        }

        printHelper(node->children[keyList[i]], depth + 1, dflag);
    }
}

void NaryTree::insert(const std::string &s, int i)
{
    insert(root, s, i);
}

void NaryTree::insert(Node *&node, const std::string &s, int i)
{
    // If we're at a NULL pointer, we make a new Node
    if (node == NULL)
    {
        node = new Node(s, i);
    }
    else
    {
        if (s.length() > 0)
        {
            if (node->children.count(s[0]) > 0)
            {                                                 //If this character is already an edge
                insert(node->children[s[0]], s.substr(1), i); // move down the branch and try again (minus 1 char)
            }
            else
            {
                node->children[s[0]] = new Node(s.substr(1), i); // If we haven't seen this edge, fill in the rest of the branch
            }
        }
        else
        { // If end of the path, store our value
            node->index.push_back(i);
        }
    }
}

void NaryTree::clear(NaryTree::Node *subRoot)
{
    if (subRoot == NULL)
        return;

    std::map<char, Node *>::iterator it;
    for (it = subRoot->children.begin(); it != subRoot->children.end(); it++)
    {
        clear(it->second);
    }

    delete subRoot;
}

/**
 * @return The height of the Nary tree. Recall that the height of a 
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
int NaryTree::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
int NaryTree::height(const Node *subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    std::map<char, Node *>::const_iterator it;
    int max = -1;
    for (it = (subRoot->children).begin(); it != (subRoot->children).end(); it++)
    {
        int temp = height(it->second);
        if (temp > max)
        {
            max = temp;
        }
    }
    return 1 + max;
}

void NaryTree::inLexOrder(std::vector<char> &treeVector)
{
    inLexOrder(root, treeVector);
}

void NaryTree::inLexOrder(Node *subRoot, std::vector<char> &treeVector)
{
    if (subRoot == NULL)
    {
        return;
    }

    std::map<char, Node *>::iterator it;
    for (it = subRoot->children.begin(); it != subRoot->children.end(); it++)
    {
        treeVector.push_back(it->first);
        inLexOrder(it->second, treeVector);
    }
}

/*
 YOUR CODE BEGINS HERE.
*/

std::vector<int> NaryTree::patternMatch(const std::string &s){
    std::vector<int> outList;

    //YOUR CODE HERE
    if(root == NULL){
        outList.push_back(-1);
    }
    else{
        patternMatch(s, root, outList);
    }
    return outList;
}

void NaryTree::patternMatch(const std::string &s, Node *subRoot, std::vector<int> &outList){
    std::map<char, Node*>::iterator it;
    it = (subRoot->children).find(s[0]);
    //if found in map, continue checking with next character in s and correct child
    if(it != (subRoot->children).end()){
        //if index size is not = 0 AND length of string checking is 1 then perfect match!
        if(s.length() ==1){
            //index size 0, return -1
            if((it->second)->index.size()== 0){
                outList.push_back(-1);
                return;
            }
            outList = (it->second)->index; //set outList index = to index of last character of pattern
            return;
        }
        //remove first character from s string to check next character
        std::string sub = s.substr(1);
        //recursively call on children
        patternMatch(sub,it->second,outList);
    }
    else{
        outList.push_back(-1);
        return;
    }
}

/**
 * Returns a NaryTree encoding the suffix-tree for a std::string T.
 *
 * @param T A std::string object which holds the text being pre-processed.
 *
 * @return An N-ary Tree storing a full n-dimensional suffix trie
 */
NaryTree build_trie(std::string T)
{
    NaryTree myTree;
    int Tlength = T.length();
    //add all subtrings to tree separately
    for (int i = 0; i < Tlength; i++){
        std::string sub = T.substr(i);
        myTree.insert(sub, i);
        int sublength = sub.length();
        //within each substring, add index to each individual character
        for(int j = 1; j< sublength;j++){
            myTree.insert(sub.substr(0,sublength-j),i);
        }
    }
    return myTree;
}
