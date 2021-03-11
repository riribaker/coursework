//main.cpp
#include "readFromFile.h"
#include "graph.h"
#include <vector>
#include <string>

int main(){
    Graph g = Graph(true,false,true); //weighted, non-directed, printable graph
    vector<string> out = file_to_vector("facebookdata.txt");
    
    /**
     * Initialize graph using facebook data.
     * Each line in facebookdata.txt represents an edge between two vertices.
     */ 
    for(int i=0;i< (int)out.size()-1;i++){
        Vertex one = out[i];
        Vertex two = out[i+1];
        i++;
        if(!g.vertexExists(one)) g.insertVertex(one);
        if(!g.vertexExists(two)) g.insertVertex(two);
        if(g.insertEdge(one,two)){
            g.setEdgeLabel(one,two,one+","+two);
        }
    }
    
    //Initialize RESULTS.txt header
    ofstream ofs;
    ofs.open("RESULTS.txt", ofstream::out | ofstream::trunc); //clear RESULTS.txt file
    ofs<<"-------- Facebook Data Analysis -------- "<<endl<<endl;
    ofs<<"DFS Traversal: (vertex -- Hex Node Color)"<<endl;
    ofs<<"-----------------------------"<<endl;

    g.color(); //color the graph nodes (Greedy Coloring Algorithm)

    /**
     * Eulerian function uses both DFS && isConnected functions.
     * While running the Eulerian function, since print is set to true, it will output
     * all necessary data to RESULTS.txt.
     */
    g.isEulerian();

    /**
     * To analyze graph coloring worked properly, use printAdjacent(v)
     * Prints adjacent vertices && respective colors given starting vertex v to RESULTS.txt.
     */
    g.printAdjacent("3");
    
    //g.print(); //view graph node by node
}