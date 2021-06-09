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

    nScreenWidth = 120;
    nScreenHeight = 40;
    // getmaxyx(stdscr, yMax, xMax);

    screenBuffer = (wchar_t *)malloc((nScreenHeight*nScreenWidth + 1) * sizeof(wchar_t));
    map = (wchar_t *)malloc((nMapHeight*nMapWidth + 1) * sizeof(wchar_t));
    wcscpy(map, L"################");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
    wcscat(map, L"#..............#");
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
void draw_map(void)
{
    for (int ny = 0; ny < nMapHeight; ny++)
    {
        for (int nx = 0; nx < nMapWidth; nx++)
        {
            screenBuffer[ny*nScreenWidth + nx] = map[ny*nMapWidth + nx];
        }
    }
}

void draw_stat_overlay(void)
{

}

int main(int argc, char **argv)
{
    setup();

    // for (int i = 0; i < nScreenHeight*nScreenWidth; i++) {
    //     static int cnt = 0;
    //     switch (cnt)
    //     {
    //     case 0:
    //         screenBuffer[i] = LIGHT_BLOCK;
    //         break;
    //     case 1:
    //         screenBuffer[i] = MEDIUM_BLOCK;
    //         break;
    //     case 2:
    //         screenBuffer[i] = DARK_BLOCK;
    //         break;
    //     case 3:
    //         screenBuffer[i] = FULL_BLOCK;
    //         break;
    //     } // switch

    //     if (++cnt > 3) cnt = 0;
    // }
    // screenBuffer[nScreenHeight*nScreenWidth] = L'\0';

    int c;
    while((c = getch()) != 'q')
    {
        // Calculate ray angle
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)nScreenWidth) * fFOV;
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

            int nCeiling = ((float)nScreenHeight / 2.0f) - (float)nScreenHeight / fDistanceToWall;
            int nFloor = nScreenHeight - nCeiling;

            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                {
                    screenBuffer[y*nScreenWidth + x] = L' ';
                }
                else if (y <= nFloor)
                {
                    screenBuffer[y*nScreenWidth + x] = L'#';
                }
                else
                {
                    screenBuffer[y*nScreenWidth + x] = L' ';
                }
            } // for
        } // for

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