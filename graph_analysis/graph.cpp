//graph.cpp
#include "graph.h"
#include "readFromFile.h"
#include <stdlib.h> 
#include <iterator>
#include <fstream>

//for detecting errors:
const Vertex Graph::InvalidVertex = "INVALIDVERTEX";
const int Graph::InvalidWeight = INT_MIN;
const string Graph:: InvalidLabel = "INVALIDLABEL";
const Edge Graph::InvalidEdge = Edge(Graph::InvalidVertex, Graph::InvalidVertex, Graph::InvalidWeight, Graph::InvalidLabel);

Graph::Graph(bool weighted) : weighted(weighted),directed(false),p(false)
{
}

Graph::Graph(bool weighted, bool directed) : weighted(weighted),directed(directed),p(false)
{
}

Graph::Graph(bool weighted,bool directed,bool print) : weighted(weighted),directed(directed),p(print)
{

}

vector<Vertex> Graph::getAdjacent(Vertex source) const 
{
    auto lookup = adjacency_list.find(source);

    if(lookup == adjacency_list.end())
        return vector<Vertex>();

    else
    {
        vector<Vertex> vertex_list;
        unordered_map <Vertex, Edge> & map = adjacency_list[source];
        for (auto it = map.begin(); it != map.end(); it++)
        {
            vertex_list.push_back(it->first);
        }
        return vertex_list;
    }
}


Vertex Graph::getStartingVertex() const
{
    return adjacency_list.begin()->first;
}

vector<Vertex> Graph::getVertices() const
{
    vector<Vertex> ret;

    for(auto it = adjacency_list.begin(); it != adjacency_list.end(); it++)
    {
        ret.push_back(it->first);
    }

    return ret;
}

int Graph::getNumVertices() const
{
    vector<Vertex> vertices = getVertices();
    return (int)vertices.size();
}

Edge Graph::getEdge(Vertex source , Vertex destination) const
{
    if(assertEdgeExists(source, destination, __func__) == false)
        return Edge();
    Edge ret = adjacency_list[source][destination];
    return ret;
}

vector<Edge> Graph::getEdges() const
{
    if (adjacency_list.empty())
        return vector<Edge>();

    vector<Edge> ret;
    set<pair<Vertex, Vertex>> seen;

    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); it++)
    {
        Vertex source = it->first;
        for (auto its = adjacency_list[source].begin(); its != adjacency_list[source].end(); its++)
        {
            Vertex destination = its->first;
            if(seen.find(make_pair(source, destination)) == seen.end())
            {
                //this pair is never added to seen
                ret.push_back(its->second);
                seen.insert(make_pair(source,destination));
                if(!directed)
                {
                    seen.insert(make_pair(destination, source));
                }
            }
        }
    }

    return ret;
}

bool Graph::vertexExists(Vertex v) const
{
    return assertVertexExists(v, "");
}

bool Graph::edgeExists(Vertex source, Vertex destination) const
{
    return assertEdgeExists(source, destination, "");
}

Edge Graph::setEdgeLabel(Vertex source, Vertex destination, string label)
{
    if (assertEdgeExists(source, destination, __func__) == false)
        return InvalidEdge;
    Edge e = adjacency_list[source][destination];
    Edge new_edge(source, destination, e.getWeight(), label);
    adjacency_list[source][destination] = new_edge;

    if(!directed)
    {
        Edge new_edge_reverse(destination,source, e.getWeight(), label);
        adjacency_list[destination][source] = new_edge_reverse;
    }
    return new_edge;
}


string Graph::getEdgeLabel(Vertex source, Vertex destination) const
{
    if(assertEdgeExists(source, destination, __func__) == false)
        return InvalidLabel;
    return adjacency_list[source][destination].getLabel();
}

int Graph::getEdgeWeight(Vertex source, Vertex destination) const
{
    if (!weighted)
        error("can't get edge weights on non-weighted graphs!");

    if(assertEdgeExists(source, destination, __func__) == false)
        return InvalidWeight;
    return adjacency_list[source][destination].getWeight();
}

void Graph::insertVertex(Vertex v)
{
    // will overwrite if old stuff was there
    removeVertex(v);
    // make it empty again
    adjacency_list[v] = unordered_map<Vertex, Edge>();
}


Vertex Graph::removeVertex(Vertex v)
{

    if (adjacency_list.find(v) != adjacency_list.end())
    {
        if(!directed){
            for (auto it = adjacency_list[v].begin(); it != adjacency_list[v].end(); it++)
            {
                Vertex u = it->first;
                adjacency_list[u].erase(v); 
            }
            adjacency_list.erase(v);
            return v;
        }
        
        adjacency_list.erase(v);
        for(auto it2 = adjacency_list.begin(); it2 != adjacency_list.end(); it2++)
        {
            Vertex u = it2->first;
            if (it2->second.find(v)!=it2->second.end())
            {
                it2->second.erase(v);
            }
        }
        return v;
    }

    return InvalidVertex;
}

bool Graph::insertEdge(Vertex source, Vertex destination)
{
    if(adjacency_list.find(source)!= adjacency_list.end() 
    && adjacency_list[source].find(destination)!= adjacency_list[source].end())
    {
        //edge already exist
        return false;
    }

    if(adjacency_list.find(source)==adjacency_list.end())
    {
        adjacency_list[source] = unordered_map<Vertex, Edge>();
    }
        //source vertex exists
    adjacency_list[source][destination] = Edge(source, destination);
    if(!directed)
    {
        if(adjacency_list.find(destination)== adjacency_list.end())
        {
            adjacency_list[destination] = unordered_map<Vertex, Edge>();
        }
        adjacency_list[destination][source] = Edge(source, destination);
    }
    
    return true;
}

Edge Graph::removeEdge(Vertex source, Vertex destination)
{
    if(assertEdgeExists(source, destination, __func__) == false)
        return InvalidEdge;
    Edge e = adjacency_list[source][destination];
    adjacency_list[source].erase(destination);
    // if undirected, remove the corresponding edge
    if(!directed)
    {
        adjacency_list[destination].erase(source);
    }
    return e;
}


Edge Graph::setEdgeWeight(Vertex source, Vertex destination, int weight)
{
    if (assertEdgeExists(source, destination, __func__) == false)
        return InvalidEdge;
    Edge e = adjacency_list[source][destination];
    //std::cout << "setting weight: " << weight << std::endl;
    Edge new_edge(source, destination, weight, e.getLabel());
    adjacency_list[source][destination] = new_edge;

    if(!directed)
        {
            Edge new_edge_reverse(destination,source, weight, e.getLabel());
            adjacency_list[destination][source] = new_edge_reverse;
        }

    return new_edge;
}

bool Graph::assertVertexExists(Vertex v, string functionName) const
{
    if (adjacency_list.find(v) == adjacency_list.end())
    {
        if (functionName != "")
            error(functionName + " called on nonexistent vertices");
        return false;
    }
    return true;
}

bool Graph::assertEdgeExists(Vertex source, Vertex destination, string functionName) const
{
    if(assertVertexExists(source,functionName) == false)
        return false;
    if(adjacency_list[source].find(destination)== adjacency_list[source].end())
    {
        if (functionName != "")
            error(functionName + " called on nonexistent edge " + source + " -> " + destination);
        return false;
    }

    if(!directed)
    {
        if (assertVertexExists(destination,functionName) == false)
            return false;
        if(adjacency_list[destination].find(source)== adjacency_list[destination].end())
        {
            if (functionName != "")
                error(functionName + " called on nonexistent edge " + destination + " -> " + source);
            return false;
        }
    }
    return true;
}

bool Graph::isDirected() const
{
    return directed;
}

void Graph::clear()
{
    adjacency_list.clear();
}


/**
 * Prints a graph error and quits the program.
 * The program is exited with a segfault to provide a stack trace.
 * @param message - the error message that is printed
 */
void Graph::error(string message) const
{
    cerr << "\033[1;31m[Graph Error]\033[0m " + message << endl;
}

/**
 * Prints the graph to stdout.
 */
void Graph::print() const
{
    for (auto it = adjacency_list.begin(); it != adjacency_list.end(); ++it) 
    {
        cout << it->first << endl;
        for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2) 
        {
            std::stringstream ss;
            ss << it2->first; 
            string vertexColumn = "    => " + ss.str();
            vertexColumn += " " ;
            cout << std::left << std::setw(26) << vertexColumn;
            string edgeColumn = "edge label = \"" + it2->second.getLabel()+ "\"";
            cout << std::left << std::setw(26) << edgeColumn;
            if (weighted)
                cout << "weight = " << it2->second.getWeight();
            cout << endl;
        }
        cout << endl;
    }
}

void Graph::DFS()
{
    vertexmap visited;
    vector<Vertex> vertices = getVertices();

    //initialize all vertices as unvisited
    for(Vertex v : vertices){
        visited[v] = false;
    }

    Vertex start = getStartingVertex();
    DFS(start,visited); //start traversal at startingVertex

    //if printable graph, output to RESULTS.txt for organization
    if(p){
        ofstream outfile;
        outfile.open("RESULTS.txt",fstream::app);
        outfile << "-----------------------------"<< endl;
    }
}

void Graph::DFS(Vertex v, vertexmap &visited)
{
    /**
     * If printable graph, output each vertex and respective color to RESULTS.txt.
     * Will output each vertex in order of traversal.
     */
    if(p){
        ofstream outfile;
        outfile.open("RESULTS.txt",fstream::app);
        outfile << "("<<v<<")"<<"--"<<"#"<< getVertexColor(v)<<endl;
    }

    visited[v] = true;
    vector<Vertex> adj = getAdjacent(v);
    for(Vertex v : adj){
        //if vertex in adjacency vector has not been visited, call traversal
        if(!visited[v]) DFS(v,visited); 
    }
}

bool Graph::isConnected() 
{ 
    vertexmap visited;
    vector<Vertex> vertices = getVertices();
    //initialize all vertices as unvisited
    for(Vertex v : vertices){
        visited[v] = false;
    }

    for (Vertex v : vertices){
        //call DFS on vertices with adjacent nodes only
        if((getAdjacent(v)).size() != 0){
            DFS(v,visited);
            for(Vertex v : vertices){
                //if a vertex has not been visited, it is not connected to graph
                if(!visited[v] && (getAdjacent(v)).size()> 0){
                    //if printable graph print results of function to RESULTS.txt
                    if(p){
                        ofstream outfile;
                        outfile.open("RESULTS.txt",fstream::app);
                        outfile << endl<<"-----------------------------"<< endl;
                        outfile <<"The graph is disconnected." << endl;
                    }
                    return false;
                }
            }
        }
    }
    
    //if printable graph print results of function to RESULTS.txt
    if(p){
        ofstream outfile;
        outfile.open("RESULTS.txt",fstream::app);
        outfile <<endl<< "-----------------------------"<< endl;
        outfile <<"The graph is connected." << endl;
    }
    return true;
}

int Graph::isEulerian()
{
    //if graph is not connected, cannot be Eulerian
    if(isConnected() == false) return 0;

    int odd = 0;
    for(Vertex v : getVertices()){
        //if size is odd, increment number of odd degree vertices
        if((getAdjacent(v)).size() & 1){
            ++odd;
        }
    }

    //if greater than 2 odd degree vertices, not Eulerian 
    if(odd > 2){
        //if printable graph print results of function to RESULTS.txt
        if(p){
            ofstream outfile;
            outfile.open("RESULTS.txt",fstream::app);
            outfile << "The graph is not Eulerian." << endl;
        }
        return 0;
    }
    //if 2 odd degree vertices, Semi-Eulerian (has a Euler path) 
    if(odd == 2){
        //if printable graph print results of function to RESULTS.txt
        if(p){
            ofstream outfile;
            outfile.open("RESULTS.txt",fstream::app);
            outfile << "The graph is Semi-Eulerian (has a Euler path)." << endl;
        }
        return 1;
    }
    //if 0 odd degree vertices, Eulerian
    //if printable graph print results of function to RESULTS.txt
    if(p){
        ofstream outfile;
        outfile.open("RESULTS.txt",fstream::app);
        outfile << "The graph is Eulerian." << endl;
    }
    return 2;
}


string Graph::getVertexColor(Vertex v){
    return vertexColor[v];
}

void Graph::color(){
    vector<Vertex> vertices = getVertices();
    vertexColor.clear();

    int maxnumColors = vertices.size();

    //boolean map to see if color has been used yet
    //unordered_map<int, bool> available;
    unordered_map<string, bool> available;
    //add distinct colors to available:
    for(int i = 0; i<maxnumColors; i++){
        //randomly choose rgb number:
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        //convert rgb to Hex:
        string color = intToHex(r) + intToHex(g)+ intToHex(b);
        
        unordered_map<string, bool>::iterator lookup = available.find(color);
        int count = 0;
        //if color already existant in available colors, randomly pick again
        if(lookup != available.end()){
            while(lookup != available.end()){
                //randomly choose rgb number:
                int r = rand() % 255;
                int g = rand() % 255;
                int b = rand() % 255;
                //convert rgb to Hex:
                string color = intToHex(r) + intToHex(g)+ intToHex(b);
                
                lookup = available.find(color);
                count++;
                //prevent infinite loop (large data cases):
                if(count>5) break;
            }
        }
        //initialize all color's availability to false:
        available[color] = false;
    }

    //first set color of starting vertex to first color:
    auto it = available.begin();
    vertexColor[getStartingVertex()]= it->first;

    //initialize rest of colors to white:
    string initcolor = "FFFFFF";
    for(Vertex v : vertices){
        vertexColor[v] = initcolor;
    }

    //greedy coloring algorithm
    for(Vertex v : vertices){
        vector<Vertex> adj = getAdjacent(v);
        for(Vertex v : adj){
            //if vertexcolor has been changed, initialize to available 
            if(vertexColor[v] != initcolor){
                available[vertexColor[v]] = true;
            }
        }

        for(auto it : available){
            //if color is not available set to vertexColor 
            if(it.second == false){
                vertexColor[v] = it.first;
            }
        }

        for(Vertex v2 : adj){
            //set available back to false for adjacent
            if(vertexColor[v2] != initcolor){
                available[vertexColor[v2]] = false;
            }
        }
    }
}

void Graph::printAdjacent(Vertex v){
    ofstream outfile;
    outfile.open("RESULTS.txt",fstream::app);
    outfile << "-----------------------------"<< endl<<endl;
    outfile << "Adjacent vertices of "<<"("<<v<<")"<<"--"<<"#"<< getVertexColor(v)<<":"<<endl;
    outfile << "-----------------------------"<< endl;
    vector<Vertex> adj = getAdjacent(v);
    for(Vertex v : adj){
        outfile << "("<<v<<")"<<"--"<<"#"<< getVertexColor(v)<<endl;
    }
}

string Graph::intToHex(int c){
    char hex[c/2 + c%2];
    sprintf(hex, "%x", c);
    return hex;
}
    
  

