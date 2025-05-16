#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <time.h>

#define GRID_SIZE 24
#define CELL_SIZE 30
#define COLORS 7 // don't make this more that 10

/*
Potential features / Next steps:
- Undo functionality.
- Animated flood fill (rather than instant).
- Cellular automata rules for a nicer looking starting grid.
 */

void floodfill(int x, int y, Color grid[GRID_SIZE][GRID_SIZE], Color startColor, Color replaceColor);

static Color colors[] = {
    RED, GREEN, BLUE, YELLOW, PURPLE, SKYBLUE, ORANGE, BROWN, GRAY, GOLD
};

bool ColorEqual(Color a, Color b) {
    return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

int main(void)
{
    srand((unsigned int)time(NULL));
    InitWindow(1920,1080, "Flood-it!");  

    Color grid[GRID_SIZE][GRID_SIZE] = {0};
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int random = rand() % COLORS;
            grid[i][j] = colors[random];
        }
    }

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //DRAW THE BOXES
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                DrawRectangle(j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE, CELL_SIZE, grid[i][j]);
            }
        }

        //FLOODFILL
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int y = GetMouseX()/CELL_SIZE;
            int x = GetMouseY()/CELL_SIZE;
            printf("X: %i Y: %i\n ", x,y);
            Color startC = grid[0][0];
            Color replaceC = grid[x][y];
            printf("start color: %c\n", replaceC);
            printf("replace color: %c\n", replaceC);

            floodfill(0, 0, grid, startC, replaceC);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void floodfill(int x, int y, Color grid[GRID_SIZE][GRID_SIZE], Color startColor, Color replaceColor)
{
  if (x < 0 || y < 0 || x >= GRID_SIZE || y >= GRID_SIZE) return;
  if (ColorEqual(grid[x][y], replaceColor)) return;
  if (!ColorEqual(grid[x][y], startColor)) return;

  grid[x][y] = replaceColor;

  floodfill(x+1, y, grid, startColor, replaceColor);
  floodfill(x-1, y, grid, startColor, replaceColor);
  floodfill(x, y+1, grid, startColor, replaceColor);
  floodfill(x, y-1, grid, startColor, replaceColor);
}
