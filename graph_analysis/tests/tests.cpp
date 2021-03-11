#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../graph.h"
#include "../readFromFile.h"
#include <vector>
#include <string>

void initialize(const string & filename,Graph &g){
    vector<string> out = file_to_vector(filename);

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
}

TEST_CASE("Graph initialized propertly given data - 10 Vertices","[weight=1]"){
    Graph g(true,true);
    initialize("sampledata/sample10.txt",g);
    //check number of vertices
    REQUIRE(g.getNumVertices() == 10);
    //check vertices
    for(int i = 0;i < 10; i++){
        Vertex v = to_string(i);
        REQUIRE(g.vertexExists(v));
    }

    REQUIRE(!g.vertexExists("10"));
    REQUIRE(!g.vertexExists("30"));

    //check edges
    REQUIRE(g.edgeExists("0","1"));
    REQUIRE(g.edgeExists("1","2"));
    REQUIRE(g.edgeExists("6","2"));
    REQUIRE(g.edgeExists("5","1"));
    REQUIRE(g.edgeExists("0","1"));
    REQUIRE(g.edgeExists("3","9"));
    REQUIRE(!g.edgeExists("4","7"));
    REQUIRE(!g.edgeExists("8","1"));
}

TEST_CASE("Graph initialized propertly given data - 20 Vertices","[weight=1]"){
    Graph g(true,true);
    initialize("sampledata/sample20.txt",g);
    //check number of vertices
    REQUIRE(g.getNumVertices() == 20);
    //check vertices
    for(int i = 0;i < 20; i++){
        Vertex v = to_string(i);
        REQUIRE(g.vertexExists(v));
    }
    
    //check edges
    REQUIRE(g.edgeExists("0","1"));
    REQUIRE(g.edgeExists("2","19"));
    REQUIRE(g.edgeExists("2","18"));
    REQUIRE(g.edgeExists("2","1"));
    REQUIRE(g.edgeExists("12","18"));
    REQUIRE(g.edgeExists("13","10"));
    REQUIRE(g.edgeExists("14","15"));
    REQUIRE(g.edgeExists("16","8"));
    REQUIRE(g.edgeExists("17","19"));
    REQUIRE(g.edgeExists("17","4"));
    REQUIRE(g.edgeExists("3","17"));
    REQUIRE(g.edgeExists("3","7"));
    REQUIRE(!g.edgeExists("6","1"));
    REQUIRE(!g.edgeExists("8","6"));
    REQUIRE(!g.edgeExists("2","3"));
}

TEST_CASE("Graph initialized propertly given data - 30 Vertices","[weight=1]"){
    Graph g(true,true);
    initialize("sampledata/sample30.txt",g);
    //check number of vertices
    REQUIRE(g.getNumVertices() == 30);
    //check vertices
    for(int i = 0;i < 30; i++){
        Vertex v = to_string(i);
        REQUIRE(g.vertexExists(v));
    }
    
    //check edges
    REQUIRE(g.edgeExists("0","1"));
    REQUIRE(g.edgeExists("0","2"));
    REQUIRE(g.edgeExists("1","20"));
    REQUIRE(g.edgeExists("2","20"));
    REQUIRE(g.edgeExists("3","9"));
    REQUIRE(g.edgeExists("3","26"));
    REQUIRE(g.edgeExists("8","20"));
    REQUIRE(g.edgeExists("10","14"));
    REQUIRE(g.edgeExists("5","12"));
    REQUIRE(g.edgeExists("16","11"));
    REQUIRE(g.edgeExists("23","5"));
    REQUIRE(g.edgeExists("27","11"));
    REQUIRE(g.edgeExists("7","22"));
    REQUIRE(g.edgeExists("29","0"));
    REQUIRE(!g.edgeExists("29","13"));
    REQUIRE(!g.edgeExists("4","22"));
    REQUIRE(!g.edgeExists("28","29"));

    //test for undirected graph implementation:
    Graph u(true,false);
    initialize("sampledata/sample30.txt",u);
    REQUIRE(u.edgeExists("1","0"));
    REQUIRE(u.edgeExists("2","0"));
    REQUIRE(u.edgeExists("1","20"));
    REQUIRE(u.edgeExists("20","2"));
    REQUIRE(u.edgeExists("9","3"));
    REQUIRE(u.edgeExists("29","0"));
    REQUIRE(!u.edgeExists("29","13"));
    REQUIRE(u.edgeExists("22","3"));
    REQUIRE(!u.edgeExists("29","28"));

}

TEST_CASE("isConnected detects connected graphs","[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/triangle.txt",g);
    REQUIRE(g.isConnected());

    g.clear();
    initialize("sampledata/octahedral.txt",g);
    REQUIRE(g.isConnected());

    g.clear();
    initialize("sampledata/pentatope.txt",g);
    REQUIRE(g.isConnected());
}

TEST_CASE("isConnected detects disconnected graphs","[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/disconnected1.txt",g);
    REQUIRE(!g.isConnected());

    g.clear();
    initialize("sampledata/disconnected2.txt",g);
    REQUIRE(!g.isConnected());

    g.clear();
    initialize("sampledata/disconnected3.txt",g);
    REQUIRE(!g.isConnected());
}

TEST_CASE("isEulerian detects Eulerian graphs","[weight=1]"){

    Graph g(true,false);
    initialize("sampledata/eulerian.txt",g);
    REQUIRE(g.isEulerian()==2);

    g.clear();
    initialize("sampledata/pentatope.txt",g);
    REQUIRE(g.isEulerian()==2);

    g.clear();
    initialize("sampledata/triangle.txt",g);
    REQUIRE(g.isEulerian()==2);

    g.clear();
    initialize("sampledata/octahedral.txt",g);
    REQUIRE(g.isEulerian()==2);

    g.clear();
    g.insertVertex("0");
    REQUIRE(g.isEulerian()==2);

}

TEST_CASE("isEulerian detects Semi-Eulerian graphs","[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/semieulerian.txt",g);
    REQUIRE(g.isEulerian()==1);
}

TEST_CASE("isEulerian detects nonEulerian graphs","[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/noneulerian.txt",g);
    REQUIRE(!g.isEulerian());

    g.clear();
    initialize("sampledata/noneulerian2.txt",g);
    REQUIRE(!g.isEulerian());

    g.clear();
    initialize("sampledata/noneulerian3.txt",g);
    REQUIRE(!g.isEulerian());
}

typedef unordered_map<Vertex, bool> vertexmap;

TEST_CASE("DFS correctly traverses connected nodes in disconnected graph", "[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/disconnected1.txt",g);
    
    vertexmap visited;
    vector<Vertex> vertices = g.getVertices();
    
    //initialize all vertices as unvisited
    for(Vertex v : vertices){
        visited[v] = false;
    }

    Vertex start = "0";
    g.DFS(start,visited);

    REQUIRE(visited["0"]);
    REQUIRE(visited["2"]);
    REQUIRE(visited["1"]);
    REQUIRE(!visited["3"]);
    REQUIRE(!visited["4"]);

    start = "3";
    g.DFS(start,visited);
    REQUIRE(visited["3"]);
    REQUIRE(visited["4"]);
    REQUIRE(visited["5"]);
    REQUIRE(visited["8"]);
    REQUIRE(!visited["10"]);
    REQUIRE(!visited["11"]);

    start = "10";
    g.DFS(start,visited);
    REQUIRE(visited["10"]);
    REQUIRE(visited["11"]);
    REQUIRE(visited["12"]);
    REQUIRE(visited["13"]);
}

TEST_CASE("DFS processes all nodes once", "[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/octahedral.txt",g);
    
    vertexmap visited;
    vector<Vertex> vertices = g.getVertices();
    
    //initialize all vertices as unvisited
    for(Vertex v : vertices){
        visited[v] = false;
    }
    Vertex start = "3";
    g.DFS(start,visited);
    REQUIRE(visited["3"]);
    REQUIRE(visited["2"]);
    REQUIRE(visited["0"]);
    REQUIRE(visited["1"]);
    REQUIRE(visited["5"]);

    start ="5";
    visited[start] = false;
    g.DFS(start,visited);
    REQUIRE(visited["5"]);
    REQUIRE(visited["1"]);
    REQUIRE(visited["4"]);
    REQUIRE(visited["0"]);
}


TEST_CASE("Color assigns a color to all vertices", "[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/sample10.txt",g);
    g.color();

    vector<Vertex> vertices = g.getVertices();
    for(Vertex v: vertices){
        REQUIRE(g.getVertexColor(v)!="FFFFFF");
    }

    g.clear();
    vertices.clear();
    initialize("sampledata/pentatope.txt",g);
    g.color();

    vertices = g.getVertices();
    for(Vertex v: vertices){
        REQUIRE(g.getVertexColor(v)!="FFFFFF");
    }

    g.clear();
    vertices.clear();
    initialize("sampledata/octahedral.txt",g);
    g.color();

    vertices = g.getVertices();
    for(Vertex v: vertices){
        REQUIRE(g.getVertexColor(v)!="FFFFFF");
    }
}

TEST_CASE("Color uses a correct number of colors", "[weight=1]"){
    Graph g(true,false);
    initialize("sampledata/pentatope.txt",g);
    g.color();
    int chromaticnumber = 5;
    int colorCount = 0;

    vector<Vertex> vertices = g.getVertices();
    unordered_map<string,Vertex> colors;

    for(Vertex v : vertices){
        string color = g.getVertexColor(v);
        unordered_map<string,Vertex>::iterator lookup = colors.find(color);
        if(lookup == colors.end()){
            colorCount++;
            colors[color] = v;
        }
    }

    REQUIRE(colorCount == chromaticnumber);

    g.clear();
    colors.clear();
    initialize("sampledata/octahedral.txt",g);
    g.color();
    chromaticnumber = 3;
    colorCount = 0;

    vertices = g.getVertices();

    for(Vertex v : vertices){
        string color = g.getVertexColor(v);
        unordered_map<string,Vertex>::iterator lookup = colors.find(color);
        if(lookup == colors.end()){
            colorCount++;
            colors[color] = v;
        }
    }
    REQUIRE(colorCount == chromaticnumber);

    g.clear();
    colors.clear();
    initialize("sampledata/cuboctahedral.txt",g);
    g.color();
    int optimalnumber = 4;
    colorCount = 0;

    vertices = g.getVertices();

    for(Vertex v : vertices){
        string color = g.getVertexColor(v);
        unordered_map<string,Vertex>::iterator lookup = colors.find(color);
        if(lookup == colors.end()){
            colorCount++;
            colors[color] = v;
        }
    }

    REQUIRE(colorCount == optimalnumber);
}


