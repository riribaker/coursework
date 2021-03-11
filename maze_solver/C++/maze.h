#include "dsets.h"
#include <vector>
#include <iostream>
#include "cs225/PNG.h"
#include <algorithm>
using namespace std;
using namespace cs225;


class SquareMaze{
    public:

    SquareMaze();
    ~SquareMaze();
    void makeMaze(int w,int h);
    bool canTravel(int x,int y,int dir) const;
    void setWall(int x,int y,int dir,bool exists);
    vector<int> solveMaze();
    PNG* drawMaze() const;
    PNG* drawMazeWithSolution();

    //my additions:
    private:
    void solh(vector<vector<int>>* dst, vector<vector<int>>* way, int tmpx, int tmpy);
    class mnode {
      public:
        bool down;
        bool right;
        mnode() {
            down = false;
            right = false;
        }
        
        mnode(bool dQ, bool rQ) {
            down = dQ;
            right = rQ;
        }
        
    };
	vector<mnode*> mpts;
    DisjointSets* dsjset;
    int width_;
	int height_;
};