#define WALL    '%'
#define EMPTY   ' '
#define START   'S'
#define END     'E'
#define PATH    '.'
#define VISITED '~'


typedef struct {
    int width;        // the width of the maze
    int height;       // the height of the maze
    int startColumn;  // the column the start of the maze is in
    int startRow;     // the row the start of the maze is in
    int endColumn;    // the column the end of the maze is in
    int endRow;       // the row the tend of the maze is in
    char ** cells;    // the 2D array containing the maze layout
} maze_t;

int checkMaze(maze_t * maze);

// These functions must be implemented correctly for full functionality points 
maze_t * createMaze(char * fileName);
void destroyMaze(maze_t * maze);
int solveMazeManhattanDFS(maze_t * maze, int col, int row);
void printMaze(maze_t * maze);