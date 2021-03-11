#include <stdio.h>
#include <stdlib.h>
#include "maze.h"


/*
 * createMaze -- Creates and fills a maze structure from the given file
 * INPUTS:       fileName - character array containing the name of the maze file
 * OUTPUTS:      None 
 * RETURN:       A filled maze structure that represents the contents of the input file
 * SIDE EFFECTS: None
 */

/*
 * createMaze will allocate memory for the maze structure by using malloc for the size
 * of the structure. It will then assign the width and height members of such structure
 * by reading the file and scanning for the given values. Using this width and height, it
 * uses malloc and an iterative for loop to set up the double pointer for the 2d matrix cells.
 * It then scans through the given file filling in the cells matrix and marking the start
 * and end indices in the corresponding structure members.
 * Finally, it closes the file and returns a pointer "maze" to the start of the allocated memory.
 */

maze_t * createMaze(char * fileName){
    
    //open file for reading
    FILE *mazefile;
    mazefile = fopen(fileName,"r");
    
    //allocate memory for maze_t structure
    maze_t *maze = (maze_t *) malloc(sizeof(maze_t));
    
    //store width and height in maze structure members
    int w,h;
    fscanf(mazefile,"%d %d \n",&w,&h);
    maze->width = w;
    maze->height = h;

    //allocate memory for cells first level pointer
    maze->cells = (char **) malloc(maze->height * sizeof(char *));
    int i;
    //allocate memory for cells second level pointer through iteration
    for(i = 0; i < maze->height; i++){
        maze->cells[i] = (char *) malloc(maze->width * sizeof(char));
    }
    
    //parse through file and store maze in corresponding cells
    //finds START and END, stores indices in corresponding column/row of struct
    int r=0,c=0;
    char charac;
    //finishes parsing through file once it reaches end of file
    while ((charac = fgetc(mazefile))!= EOF) {
        
        //if newline, skip column increment and don't store in cells
        if (charac == 0x0A) {
            c = 0;
            r++;
            continue;
        }
        else{
        //store character in cells
        maze->cells[r][c] = charac;
        //if character is "S" store start indices in struct
        if (charac == START) {
            maze->startColumn = c;
            maze->startRow = r;
        }
        //if character is "E" store end indices in struct
        if (charac == END) {
            maze->endColumn = c;
            maze->endRow = r;
        }
        //increment column
        c++;
        }
    }
    
    //close file since done reading
    fclose(mazefile);
    //returns pointer to start of allocated memory for struct
    return maze;
    
}

/*
 * destroyMaze -- Frees all memory associated with the maze structure, including the structure itself
 * INPUTS:        maze -- pointer to maze structure that contains all necessary information 
 * OUTPUTS:       None
 * RETURN:        None
 * SIDE EFFECTS:  All memory that has been allocated for the maze is freed
 */

/*
 * destroyMaze first frees the second level pointer within cells through iteration. It then
 * frees the cells first level pointer before finally freeing the pointer to the maze structure
 * "maze".
 */

void destroyMaze(maze_t * maze){
    //iterates through cells[i] and frees pointers
    int i;
    for(i = 0; i < maze->height; i++){
        free(maze->cells[i]);
    }
    //frees cells pointer
    free(maze->cells);
    //frees pointer to maze struct
    free(maze);
}

/*
 * printMaze --  Prints out the maze in a human readable format (should look like examples)
 * INPUTS:       maze -- pointer to maze structure that contains all necessary information 
 *               width -- width of the maze
 *               height -- height of the maze
 * OUTPUTS:      None
 * RETURN:       None
 * SIDE EFFECTS: Prints the maze to the console
 */

/*
 * printMaze uses nested for loops to print cells in "human readable format." Using i as the
 * row pointer and j as the column pointer, it prints each cell within the 2d matrix cells and
 * prints a new line at the end of each column.
 */

void printMaze(maze_t * maze){
    //iterates through rows and columns of cells and prints corresponding character
    int i,j;
    for(i=0;i<maze->height;i++){
        for (j=0; j<maze->width; j++) {
            char c = maze->cells[i][j];
            printf("%c",c);
        }
        //prints new line at end of each column
        printf("\n");
    }
}

/* finddistance compares the given indices of r and c to the indices of the end of the maze.
 * If the given cell indices is out of bounds, it returns a large distance of 1000.
 * If the given cell indices are that of a wall,start, or a cell that has already been visited
 * then it will return a very large distance of 1000. If the current cell is part of the path
 * it will add a positive offset to the distance such that the program will later prefer to
 * take a new path rather than turn around. This offset is 0 otherwise.
 * It calculates the Manhattan distance and returns this distance with an offset for the path
 * if it exists.
 */

int finddistance(maze_t * maze,int r,int c){
    //dist initialized to 0
    int dist = 0;
    //if out of bounds return very large distance of 1000
    if (c<0 || c>= maze->width || r<0 || r>=maze->height) {
        dist = 1000;
        return dist;
    }
    //if WALL or START return very large distance of 1000
    if (maze->cells[r][c] == WALL){
        dist = 1000;
        return dist;
    }
    //if VISITED before return very large distance of 1000
    if (maze->cells[r][c] == VISITED){
        dist = 1000;
        return dist;
    }
    //if PATH, add disadvantaged distance (prefer not to follow path)
    if (maze->cells[r][c] == PATH){
        dist = 5;
    }
    
    if (maze->cells[r][c] == START){
        dist = 10;
    }
    
    //find dist in x dir to END
    int dist_x = abs(maze->endColumn -c);
    //find dist in y dir to END
    int dist_y = abs(maze->endRow -r);
    //return the manhattan distance + offset for path or start
    return (dist_x + dist_y + dist);
}

/*
 * solveMazeDFS -- recursively solves the maze using depth first search,
 * INPUTS:               maze -- pointer to maze structure with all necessary maze information
 *                       col -- the column of the cell currently beinging visited within the maze
 *                       row -- the row of the cell currently being visited within the maze
 * OUTPUTS:              None
 * RETURNS:              0 if the maze is unsolvable, 1 if it is solved
 * SIDE EFFECTS:         Marks maze cells as visited or part of the solution path
 */

/*
 * solveMazeDFS will first check the bounds of row and col and return 0 if they are out of bounds.
 * It then checks if the indices match the end of the maze and returns 1 if it has. If the current
 * cell is empty, it will mark it as part of our path through the maze.
 * It then compares the distance to the end of the maze (high distance of 1000 if can't move there)
 * between the directions of left, right, up or down from the current cell. It then chooses either
 * the direction with the smallest distance to end of the maze or the only option it has (path/start).
 * If it chooses a direction in which it will go back to the path, it marks the current cell as
 * VISITED. Finally, it recursively calls itself to check if it can continue through the maze with
 * this choice selected. If it leads to a dead end, it will backtrack and remake its decision if possible.
 * If it reaches the end of the maze it will finally return 1, if it gets stuck, it will return 0.
 */

int solveMazeDFS(maze_t * maze, int col, int row){
    //if out of bounds, return 0
    if (col>=maze->width || row>= maze->height)
        return 0;
    if (col<0 || row<0)
        return 0;
    
    //if end of maze, return 1
    if (maze->cells[row][col]== END)
        return 1;
    
    //if cell is empty, mark it as part of path
    if (maze->cells[row][col]==EMPTY) {
        maze->cells[row][col]=PATH;
    }
    //compare distance to end between choices of left,right,up,down
    int bestchoice;
    int left = finddistance(maze,row,col-1);
    int right = finddistance(maze,row,col+1);
    int up = finddistance(maze,row-1,col);
    int down = finddistance(maze,row+1,col);
    
    //choose direction with smallest distance to END
    //if greater than 1000, then can't move there since it is wall,start, or
    //has already been visited so bestchoice == 4 (will return 0 later)
    
    if (left<up && left<down && left<right && left<1000)
        bestchoice =0;
    else if (right<=up && right<=down && right<=left && right<1000)
        bestchoice =1;
    else if (up<=right && up<down && up<=left && up<1000)
        bestchoice =2;
    else if (down<=right && down<=up && down<=left && down<1000)
        bestchoice =3;
    else
        bestchoice =4;
    
    //backtrack marking
    //moving left towards previous path
    if ((bestchoice ==0) && (maze->cells[row][col-1]== PATH))
        maze->cells[row][col] = VISITED;
    //moving right towards previous path
    if ((bestchoice ==1) && (maze->cells[row][col+1]== PATH))
        maze->cells[row][col] = VISITED;
    //moving up towards previous path
    if ((bestchoice ==2) && (maze->cells[row-1][col]== PATH))
        maze->cells[row][col] = VISITED;
    //moving down towards previous path
    if ((bestchoice ==3) && (maze->cells[row+1][col]== PATH))
        maze->cells[row][col] = VISITED;
    
    //move left
    if ((bestchoice ==0) && (solveMazeDFS(maze,col-1, row) == 1))
        return 1;
    //move right
    if ((bestchoice ==1) && (solveMazeDFS(maze,col+1, row) == 1))
        return 1;
    //move up
    if ((bestchoice ==2) && (solveMazeDFS(maze,col, row-1) == 1))
        return 1;
    //move down
    if ((bestchoice ==3) && (solveMazeDFS(maze,col, row+1) == 1))
        return 1;
    
    //else, mark cell as visited and return 0
    maze->cells[row][col] = VISITED;
    return 0;
    
}
