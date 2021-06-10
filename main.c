#ifdef _WIN32
    #include <Windows.h>
#else
    #include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#include <ncurses.h>

#include "unicode_defines.h"

/* Defines */
#define MAX_DEPTH 16.0f

/* Global variables */
wchar_t *screenBuffer;
wchar_t *map;
int nScreenHeight, nScreenWidth;

float fPlayerX = 8.0;
float fPlayerY = 8.0;
float fPlayerA = 0.0;

float fFOV = M_PI / 4.0;

int nMapHeight = 16;
int nMapWidth = 16;

void setup(void)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, true);

    // resize_term(240, 320);
    nScreenWidth = 120;
    nScreenHeight = 40;
    getmaxyx(stdscr, nScreenHeight, nScreenWidth);

    screenBuffer = (wchar_t *)malloc((nScreenHeight*nScreenWidth + 1) * sizeof(wchar_t));
    map = (wchar_t *)malloc((nMapHeight*nMapWidth + 1) * sizeof(wchar_t));
    wcscpy(map, L"################");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..........#...#");
    wcscat(map, L"#..........#...#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#.......########");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"################");
}

void output_screen_buffer(void)
{
    for (int y = 0; y < nScreenHeight; y++)
    {
        for (int x = 0; x < nScreenWidth; x++)
        {
            // screenBuffer[ny*xMax + nx] = map[ny*nMapWidth + nx];
            mvprintw(y, x, "%lc", screenBuffer[y*nScreenWidth + x]);
        }
    }
    refresh();
}

// Output map
void draw_map(int y, int x)
{
    for (int ny = 0; ny < nMapHeight; ny++)
    {
        for (int nx = 0; nx < nMapWidth; nx++)
        {
            screenBuffer[(ny + y)*nScreenWidth + (nx + x)] = map[ny*nMapWidth + nx];
        }
    }

    screenBuffer[((int)fPlayerY + y)*nScreenWidth + (int)fPlayerX + x] = L'*';
}

void draw_stat_overlay(void)
{

}

uint64_t get_timestamp_micro(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    uint64_t timestamp = ts.tv_sec * 1000000;
    timestamp += ts.tv_nsec / 1000;
    return timestamp;
}

int main(int argc, char **argv)
{
    setup();

    uint64_t tsp1 = get_timestamp_micro();
    uint64_t tsp2 = get_timestamp_micro();
    uint64_t elapsedTimeMs = 0;
    wchar_t buf[60];

    int c;
    while((c = getch()) != 'q')
    {
        // Player movement
        if (c == 'w')
        {
            fPlayerX += 0.00005f * sinf(fPlayerA) * (float)elapsedTimeMs;
            fPlayerY += 0.00005f * cosf(fPlayerA) * (float)elapsedTimeMs;
        }
        if (c == 's')
        {
            fPlayerX -= 0.00005f * sinf(fPlayerA) * (float)elapsedTimeMs;
            fPlayerY -= 0.00005f * cosf(fPlayerA) * (float)elapsedTimeMs;
        }
        if (c == 'd')
        {
            fPlayerA -= 0.00001f * (float)elapsedTimeMs;
        }
        if (c == 'a')
        {
            fPlayerA += 0.00001f * (float)elapsedTimeMs;
        }

        // if (fPlayerX > 16.0f) fPlayerX = 16.0f;
        // if (fPlayerY > 16.0f) fPlayerY = 16.0f;
        // if (fPlayerX < 0.0f) fPlayerX = 0.0f;
        // if (fPlayerY < 0.0f) fPlayerY = 0.0f;

        // Calculate ray angle
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA + fFOV / 2.0f) - ((float)x / (float)nScreenWidth) * fFOV;
            float fDistanceToWall = 0.0f;
            bool bHitWall = false;

            // Unit vector for ray in player space
            float fEyeX = sinf(fRayAngle);
            float fEyeY = cosf(fRayAngle);

            while(!bHitWall && fDistanceToWall < MAX_DEPTH)
            {
                fDistanceToWall += 0.1f;

                int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

                // Bounds checking on ray
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    fDistanceToWall = MAX_DEPTH;
                }
                else
                {
                    if (map[nTestY * nMapWidth + nTestX] == L'#')
                    {
                        bHitWall = true;
                    }
                } //if
            } // while

            // Calculate distance to ceiling and floor
            int nCeiling = ((float)nScreenHeight / 2.0f) - (float)nScreenHeight / fDistanceToWall;
            int nFloor = nScreenHeight - nCeiling;

            // Calculate wall shading
            wchar_t nShade;
            if (fDistanceToWall < MAX_DEPTH / 4.0f)        // Very close
                nShade = FULL_BLOCK;
            else if (fDistanceToWall < MAX_DEPTH / 3.0f)
                nShade = DARK_BLOCK;
            else if (fDistanceToWall < MAX_DEPTH / 2.0f)
                nShade = MEDIUM_BLOCK;
            else if (fDistanceToWall < MAX_DEPTH)
                nShade = LIGHT_BLOCK;
            else
                nShade = EMPTY_BLOCK;                       // Very far away

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                {
                    screenBuffer[y*nScreenWidth + x] = L' ';
                }
                else if (y <= nFloor)
                {
                    screenBuffer[y*nScreenWidth + x] = nShade;
                }
                else
                {
                    screenBuffer[y*nScreenWidth + x] = L' ';
                }
            } // for
        } // for

        // Framerate check
        tsp2 = get_timestamp_micro();
        elapsedTimeMs = tsp2 - tsp1;
        tsp1 = tsp2;
        swprintf(buf, 59, L"Frame time: %ld, Frame rate: %ldFPS\n", elapsedTimeMs, 1000000L/elapsedTimeMs);
        wcscpy(screenBuffer, buf);
        swprintf(buf, 59, L" X%.2f Y%.2f A%.2f, Res: %dx%d\n", fPlayerX, fPlayerY, fPlayerA, nScreenWidth, nScreenHeight);
        wcscpy(&screenBuffer[1*nScreenWidth], buf);

        draw_map(2, 0);
        output_screen_buffer();
        usleep(1000);
    }

    clear();
    mvprintw(0, 0, "Press any key to exit...");
    nodelay(stdscr, false);
    getch();
    free(screenBuffer);
    endwin();

    return EXIT_SUCCESS;
}










// Saving this for improving the keyboard input
// int main ( int argc, char *argv[], char *env[] )
// {
//     gtk_init(&argc, &argv);

//     GdkModifierType button_state;
//     gdk_window_get_pointer(NULL, NULL, NULL, &button_state);
//     if(button_state & GDK_CONTROL_MASK) {
//         printf("ctrl key is pressed");
//     }
// }















    // int c;
    // char buf[20];
    // while ((c = getch()) != 'q')
    // {
    //     static int lc = 0;
    //     static int cnt = 0;
    //     if (c == -1)
    //     {
    //         usleep(10000);
    //         continue;
    //     }

    //     if (c == lc)
    //     {
    //         cnt++;
    //     }
    //     else
    //     {
    //         cnt = 0;
    //     }
    //     lc = c;

    //     memset(buf, 0, sizeof(buf));
    //     sprintf(buf, "You pressed: %c! %d", c, cnt);
    //     mvwprintw(stdscr, yMax/2, xMax/2, buf);
    //     refresh();
    //     // usleep(1000);
    // }