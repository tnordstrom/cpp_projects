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
#include <random>
#include "/home/tony/raylib/raylib-master/src/raylib.h"

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

    double result { 0.0 };
    double xx { 1.0 };
    double yy { 2.0 };

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

            //ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
            #if 0
            for (int i { 0 }; i < 1000; i++)
            {
                int x { 0 };
                int y { 0 };
                
                x = rand() % screenWidth;
                y = rand() % screenHeight;
                
                DrawPixel(x, y, RED);
            }
            #endif
            
            for (xx = 0; xx < screenWidth; xx++)
            {
                for (yy = 0; yy < screenHeight; yy++)
                {
             
                    for (int i { 0 }; i < 10; i++)
                    {
                        double first { xx };
                        result = calc_iteration(first, yy);
                        first = result;
                        std::cout << "result = " << result << '\n';
                        if (result > 1000)
                        {
                            std::cout << "iterations = " << i << '\n';
                            
                            if (result < 10000)
                            {
                                std::cout << "x = " << xx << " y = " << yy << '\n';
                                DrawPixel(xx, yy, RED);
                            }
                            result = 0.0;
                            break;
                        }
                    }       
                }
            }
            
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

 
    return(0);
}
