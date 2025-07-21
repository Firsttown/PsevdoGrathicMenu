#ifndef MENU_H
#define MENU_H
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
extern HANDLE hin;
extern HANDLE hout;
#else
#include <termios.h>
#include <unistd.h>
#endif
struct MenuItem {
    size_t labelIndex;
    std::vector<MenuItem>* submenu;
};
void clearScreen();
int select_menu(short x, short y, const std::vector<std::string>& items, short height, int color = 23);
std::vector<std::string> generateLabels(int count);
#endif
