#pragma once
#include <vector>
#include <iostream>

using namespace std;

class DisjointSets {
    public:

    
    DisjointSets();

    DisjointSets(vector<int> uptrees);
    
    ~DisjointSets();

    void addelements(int num);

    int find(int elem);

    void setunion(int a,int b);

    int size(int elem);

    private:

    vector<int> uptrees;

};