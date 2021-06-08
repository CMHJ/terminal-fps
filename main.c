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

#include <ncurses.h>

/* Global variables */
wchar_t *screenBuffer;
wchar_t *map;
int yMax, xMax;

float fPlayerX = 0.0;
float fPlayerY = 0.0;
float fPlayerA = 0.0;

int nMapHeight = 16;
int nMapWidth = 16;

void setup(void)
{
    setlocale(LC_ALL, "");
    initscr();
    noecho();
    cbreak();
    // nodelay(stdscr, true);

    xMax = 1;
    yMax = 1;
    getmaxyx(stdscr, yMax, xMax);

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
    move(0, 0);
    for (int i = 0; screenBuffer[i]; i++) { printw("%lc", screenBuffer[i]); }
    refresh();
}

int main(int argc, char **argv)
{
    setup();

    for (int i = 0; i < yMax*xMax; i++) {
        screenBuffer[i] = L'\u2591';
    }
    screenBuffer[yMax*xMax] = L'\0';
    output_screen_buffer();

    int c;
    char buf[20];
    while ((c = getch()) != 'q')
    {
        static int lc = 0;
        static int cnt = 0;
        if (c == -1)
        {
            usleep(10000);
            continue;
        }

        if (c == lc)
        {
            cnt++;
        }
        else
        {
            cnt = 0;
        }
        lc = c;

        memset(buf, 0, sizeof(buf));
        sprintf(buf, "You pressed: %c! %d", c, cnt);
        mvwprintw(stdscr, yMax/2, xMax/2, buf);
        refresh();
        // usleep(1000);
    }
    // while(1)
    // {

    //     output_screen_buffer();
    //     usleep(1000);
    // }

    // clear();
    // mvprintw(0, 0, "Press any key to exit...");
    // nodelay(stdscr, false);
    // getch();
    free(screenBuffer);
    endwin();

    return EXIT_SUCCESS;
}
