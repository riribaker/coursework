using namespace std;
 using namespace cs225;

 //constructor
 SquareMaze::SquareMaze() {
     //initialize set,height,width
     dsjset = new DisjointSets();
     width_ = 0;
     height_ = 0;
 }

 //destructor
 SquareMaze::~SquareMaze() {
     //delete set & mpts within maze
     delete dsjset;
 @@ -30,6 +32,12 @@ SquareMaze::~SquareMaze() {
     mpts.clear();
 }

 /* Makes a new SquareMaze of the given height and width.
  * If this object already represents a maze it will clear all the existing data before doing so. 
  * You will start with a square grid (like graph paper) with the specified height and width. 
  * You will select random walls to delete without creating a cycle, until there are no more walls 
  * that could be deleted without creating a cycle. Do not delete walls on the perimeter of the grid.
  */
 void SquareMaze::makeMaze(int w, int h) {
     //initialize private members
     width_ = w;
 @@ -104,6 +112,9 @@ void SquareMaze::makeMaze(int w, int h) {
     return;
 }

 /* this uses your representation of the maze to determine whether it is 
  * possible to travel in the given direction from the square at coordinates (x,y).
  */
 bool SquareMaze::canTravel(int x, int y, int dir) const {
     //right
     if (dir == 0) {
 @@ -148,6 +159,11 @@ bool SquareMaze::canTravel(int x, int y, int dir) const {
     return false;
 }

 /*Sets whether or not the specified wall exists. This function should be fast (constant time). 
  * You can assume that in grading we will not make your maze a non-tree and then call one of the other member functions. 
  * setWall should not prevent cycles from occurring, but should simply set a wall to be present or not present. 
  * Our tests will call setWall to copy a specific maze into your implementation.
  */
 void SquareMaze::setWall(int x, int y, int dir, bool exists) {
     if (dir == 0) {
         mpts.at(x + width_ * y)->right = exists;
 @@ -172,8 +188,8 @@ vector<int> SquareMaze::solveMaze() {
             dist_tmp.push_back(-1);
             dir_tmp.push_back(-1);
         }
         dst.push_back(dist_tmp);
         way.push_back(dir_tmp);
         dst.push_back(dist_tmp);
     }

     solh(&dst, &way, 0, 0);
 @@ -216,56 +232,72 @@ vector<int> SquareMaze::solveMaze() {
         }
         final.push_back(cdxy);
     }

     for (unsigned i = 0; i < final.size() / 2; i++) {
         std::swap(final.at(i), final.at(final.size() - i - 1));
     }

     return final;
 }

 /* helper function for the solve maze function. this function will take the distance vector
  * the direction (way) vector and temporary x and y to make the decision on which direction
  * to take.
  */
 void SquareMaze::solh(vector<vector<int>>* dst, vector<vector<int>>* way, int tmpx, int tmpy) {
     int x = tmpx;
     int y = tmpy;

     
     int dc = dst->at(x).at(y);
     int dir_c = way->at(x).at(y);

     //if can travel right, travel there
     if (canTravel(x, y, 0) == true) {
         if (dst->at(1+x).at(y) == -1) {
             dst->at(1+x).at(y) = dc + 1;
             way->at(1+x).at(y) = 2;
             solh(dst, way, 1+x, y);
         int right = 1+x;
         if (dst->at(right).at(y) == -1) {
             dst->at(right).at(y) = dc + 1;
             way->at(right).at(y) = 2;
             solh(dst, way, right, y);
         }
     }

     //if can travel down travel there
     if (canTravel(x, y, 1) == true) {
         if (dst->at(x).at(1+y) == -1) {
             dst->at(x).at(1+y) = dc + 1;
             way->at(x).at(1+y) = 3;
             solh(dst, way, x, y + 1);
         int down = 1+y;
         if (dst->at(x).at(down) == -1) {
             dst->at(x).at(down) = dc + 1;
             way->at(x).at(down) = 3;
             solh(dst, way, x, down);
         }
     }

     //if can travel left, travel there
     if (canTravel(x, y, 2) == true) {
         if (dst->at(x - 1).at(y) == -1) {
             dst->at(x - 1).at(y) = dc + 1;
             way->at(x - 1).at(y) = 0;
             solh(dst, way, x - 1, y);
         int left = x-1;
         if (dst->at(left).at(y) == -1) {
             dst->at(left).at(y) = dc + 1;
             way->at(left).at(y) = 0;
             solh(dst, way, left, y);
         }
     }

     //if can travel up, travel there
     if (canTravel(x, y, 3) == true) {
         if (dst->at(x).at(y - 1) == -1) {
             dst->at(x).at(y - 1) = dc + 1;
             way->at(x).at(y - 1) = 1;
             solh(dst, way, x, y - 1);
         int up = y-1;
         if (dst->at(x).at(up) == -1) {
             dst->at(x).at(up) = dc + 1;
             way->at(x).at(up) = 1;
             solh(dst, way, x, up);
         }
     }
     return;
 }
 /* Draws the maze without the solution.First, create a new PNG. Set the dimensions of the PNG to (width*10+1,height*10+1). 
  * where height and width were the arguments to makeMaze. Blacken the entire topmost row and leftmost column of pixels, 
  * except the entrance (1,0) through (9,0). For each square in the maze, call its maze coordinates (x,y). If the right wall exists, 
  * then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from 0 to 10. If the bottom wall exists, then blacken the pixels 
  * with coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
  */

 PNG* SquareMaze::drawMaze() const {
     /* First, create a new PNG. Set the dimensions of the PNG 
 @@ -323,6 +355,16 @@ PNG* SquareMaze::drawMaze() const {
     return ret;
 }

 /* This function calls drawMaze, then solveMaze; 
  * it modifies the PNG from drawMaze to show the solution vector and the exit.
  * Start at pixel (5,5). Each direction in the solution vector corresponds to a 
  * trail of 11 red pixels in the given direction. If the first step is downward, 
  * color pixels (5,5) through (5,15) red. (Red is 0,1,0.5,1 in HSLA). Then if the 
  * second step is right, color pixels (5,15) through (15,15) red. Then if the third 
  * step is up, color pixels (15,15) through (15,5) red. Continue in this manner 
  * until you get to the end of the solution vector, so that your output looks analogous 
  * the above picture.
  */
 PNG* SquareMaze::drawMazeWithSolution() {
     //start at coordinate (5,5)
     int x = 5;
    int y = 5;
    // Each direction in the solution vector corresponds 
    // to a trail of 11 red pixels in the given direction
    vector<int> path = solveMaze();
    PNG* ret = drawMaze();
    for (unsigned i = 0; i < path.size(); i++) {
        //up direction:
        if (path.at(i) == 3) {
            for (int j = 0; j < 10; j++) {
                HSLAPixel& c_pixel = ret->getPixel(x, y);
                //decrement y since up direction
                y--;
                c_pixel.s = 1.0;
                c_pixel.l = 0.5;
                c_pixel.a = 1.0;
            }
        }
        //left direction:
        if (path.at(i) == 2) {
            for (int j = 0; j < 10; j++) {
                HSLAPixel& c_pixel = ret->getPixel(x, y);
                //decrement x since left direction
                x--;
                c_pixel.s = 1.0;
                c_pixel.l = 0.5;
                c_pixel.a = 1.0;
            }
        }
        //down direction:
        if (path.at(i) == 1) {
            for (int j = 0; j < 10; j++) {
                HSLAPixel& c_pixel = ret->getPixel(x, y);
                //increment y since down direction
                y++;
                c_pixel.s = 1.0;
                c_pixel.l = 0.5;
                c_pixel.a = 1.0;
            }
        }
        //right direction
        if (path.at(i) == 0) {
            for (int j = 0; j < 10; j++) {
                HSLAPixel& c_pixel = ret->getPixel(x, y);
                //increment x since right direction
                x++;
                c_pixel.s = 1.0;
                c_pixel.l = 0.5;
                c_pixel.a = 1.0;
            }
        }
    }
    /*Make the exit by undoing the bottom wall of 
     * the destination square: call the destination maze coordinates (x,y) 
     * and whiten the pixels with coordinates (x*10+k, (y+1)*10) for k from 1 to 9.
     */
    //now that at last spot in maze, set endwall 
    HSLAPixel& c_pixel = ret->getPixel(x, y);
    c_pixel.s = 1.0;
    c_pixel.l = 0.5;
    //set x and y 
    y = y + 5;
    x = x - 4;
    //set ending wall to open
    for (int i = 0; i < 9; i++) {
        HSLAPixel& c_pxl2 = ret->getPixel(i + x, 10 * height_);
        c_pxl2.l = 1.0;
    }
    return ret;
}