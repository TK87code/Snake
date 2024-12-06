#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450

int main(void)
{
    InitWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "SCREEN TITLE HERE");
    
    SetTargetFPS(60);               
    
    while (!WindowShouldClose())    
    {
        /* UPDATE */
        
        BeginDrawing();
        
        ClearBackground(RAYWHITE);
        
        EndDrawing();
    }
    
    CloseWindow();        
    
    return 0;
}