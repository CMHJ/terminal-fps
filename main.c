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

#include <ncurses.h>

#include "unicode_defines.h"

/* Global variables */
wchar_t *screenBuffer;
wchar_t *map;
int yMax, xMax;

float fPlayerX = 0.0;
float fPlayerY = 0.0;
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

    xMax = 120;
    yMax = 40;
    // getmaxyx(stdscr, yMax, xMax);

    screenBuffer = (wchar_t *)malloc((yMax*xMax + 1) * sizeof(wchar_t));
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
    for (int y = 0; y < yMax; y++)
    {
        for (int x = 0; x < xMax; x++)
        {
            // screenBuffer[ny*xMax + nx] = map[ny*nMapWidth + nx];
            mvprintw(y, x, "%lc", screenBuffer[y*xMax + x]);
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
            screenBuffer[ny*xMax + nx] = map[ny*nMapWidth + nx];
        }
    }
}

void draw_stat_overlay(void)
{

}

int main(int argc, char **argv)
{
    setup();

    for (int i = 0; i < yMax*xMax; i++) {
        static int cnt = 0;
        switch (cnt)
        {
        case 0:
            screenBuffer[i] = LIGHT_BLOCK;
            break;
        case 1:
            screenBuffer[i] = MEDIUM_BLOCK;
            break;
        case 2:
            screenBuffer[i] = DARK_BLOCK;
            break;
        case 3:
            screenBuffer[i] = FULL_BLOCK;
            break;
        } // switch

        if (++cnt > 3) cnt = 0;
    }
    screenBuffer[yMax*xMax] = L'\0';
    // draw_map();
    output_screen_buffer();


    int c;
    while((c = getch()) != 'q')
    {
        output_screen_buffer();
        usleep(10);
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