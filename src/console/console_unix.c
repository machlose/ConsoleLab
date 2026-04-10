#pragma once
#ifndef _WIN32

#include "console.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <string.h>

static struct termios oldt;

void ConsoleInit(ConsoleLabConsoleAPI* console)
{
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ConsoleGetScreenSize(console);
}

vec2 ConsoleGetScreenSize(ConsoleLabConsoleAPI* console)
{
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    console->data.screenSize.x = w.ws_col;
    console->data.screenSize.y = w.ws_row;

    return console->data.screenSize;
}

void ConsoleHandleEvents(ConsoleLabConsoleAPI* console)
{
    // tu później możesz czytać STDIN i parsować escape sequences
}

#endif