#include <iostream>
#include <vector>
#include <iterator>
#include "dsets.h"

using namespace std;


DisjointSets::DisjointSets(){
    uptrees.clear();
}

DisjointSets::DisjointSets(vector<int> intrees){
    std::vector<int>::iterator it = intrees.begin();
    while(it!=intrees.end()){
        uptrees.push_back(*it);
    }
}

DisjointSets::~DisjointSets(){

}


void DisjointSets::addelements(int num){
    for(int i =0;i<num;i++){
        uptrees.push_back(-1);
    }
}

//find with compression:
int DisjointSets::find(int elem){
    if(uptrees[elem] <0){
        return elem;
    }
    else{
        int root = find(uptrees[elem]);
        uptrees[elem] = root;
        return root;
    }
}

void DisjointSets::setunion(int a,int b){

    int r1 = find(a);
    int r2 = find(b);

    if(r1 ==r2) return;

    int newSize = uptrees[r1] + uptrees[r2];

    if (uptrees[r1] < uptrees[r2]){
        uptrees[r2] = r1;
        uptrees[r1] = newSize;
    } 
    else {
        uptrees[r1] = r2;
        uptrees[r2] = newSize;
    }
}

int DisjointSets::size(int elem){
    int s = uptrees[find(elem)];
    return -s;
}