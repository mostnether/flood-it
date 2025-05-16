#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>

#define GRID_SIZE 8
#define CELL_SIZE 80

typedef struct Cell
{
    int x;
    int y;
    char color;

}Cell;

char ColorCode(int intC);
void floodfill(int x, int y, Cell (*grid)[8], char startColor,  char replaceColor);
Color getC(Cell (*grid)[8], int x, int y);

//creates the grid with random values
int main(void)
{
  InitWindow(1920,1080, "Flood-it!");  

  Cell grid[8][8];
  //GRID INITIALIZE
  for (int i = 0; i < 8; i++)
    {
    for (int j = 0; j < 8; j++)
      {
        grid[i][j].x = i;
        grid[i][j].y = j;
        int intC = rand()%4;

        grid[i][j].color  = ColorCode(intC);
      }
    }

    int x;
    int y;
    char startC;
    char replaceC;

    
  
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //DRAW THE BOXES
        for (int i = 0; i < 8; i++)
            {
            for (int j = 0; j < 8; j++)
                {
                    DrawRectangle(j * 80, i * 80, 80, 80, getC(grid, i, j));
                }
    }
        //FLOODFILL
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
            int y = GetMouseX()/80;
            int x = GetMouseY()/80;
            printf("X: %i Y: %i\n ", x,y);
            startC = grid[0][0].color;
            replaceC = grid[x][y].color;
            printf("start color: %c ", replaceC);
            printf("replace color: %c ", replaceC);

            floodfill(0, 0, grid, startC, replaceC);
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;

  }
void floodfill(int x, int y, Cell (*grid)[8], char startColor,  char replaceColor)
{

  if (x<0 || y<0 || x>7 || y>7){
    return;
  }
  if (grid[x][y].color == replaceColor){
    return;
  }
  if (grid[x][y].color != startColor){
    return;
  }

  grid[x][y].color = replaceColor;

  floodfill(x+1,y ,grid, startColor, replaceColor);
  floodfill(x-1,y ,grid, startColor, replaceColor);
  floodfill(x,y+1 ,grid, startColor, replaceColor);
  floodfill(x,y-1 ,grid, startColor, replaceColor);
}

char ColorCode(int intC)


{
  char C;
  if (intC==0){
    C = 'R';
  }
  else if (intC==1)
  {
    C = 'G';
  }
  else if (intC==2){
    C = 'B';
  }
  else{
    C = 'Y';
  }
  return C;
}

Color getC(Cell (*grid)[8], int x, int y)
{   
    Color color;

    switch (grid[x][y].color)
    {
    case 'R':
        color =RED;
        break;
    case 'G':
        color = GREEN;
        break;
    case 'B':
        color =BLUE;
        break;
    case 'Y':
        color =YELLOW;
        break;
    }
    return color;
}

