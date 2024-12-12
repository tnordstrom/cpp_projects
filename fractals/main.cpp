/* Tony Nordstrom */
/* December 11 02024 */
/* Project name: Fractals */
/* Project path: /cygdrive/c/dev/c++/fractals */
/* To build: g++ -Wall -o "main" "main.cpp" */
/* To run: ./main.exe */
/* 
 Version 0.1
 - 
 */

#include <iostream>
#include "/cygdrive/c/raylib/raylib/src/raylib.h"

double calc_iteration(double z, double c)
{
    return((z * z) + c);
}

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

 
    return(0);
}
