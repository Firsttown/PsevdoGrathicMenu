include "Menu.h"
#include <iostream>
#ifdef _WIN32
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hin = GetStdHandle(STD_INPUT_HANDLE);
#else
static struct termios orig_termios;
#endif
void clearScreen() {
#ifdef _WIN32
    COORD topLeft = { 0, 0 };
    DWORD infoChars = 0;
    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(hout, &screen);
    DWORD consoleSize = screen.dwSize.X * screen.dwSize.Y;
    FillConsoleOutputCharacter(hout, ' ', consoleSize, topLeft, &infoChars);
    FillConsoleOutputAttribute(hout, 7, consoleSize, topLeft, &infoChars);
    SetConsoleCursorPosition(hout, topLeft);
#else
    std::cout << "\033[2J\033[H" << std::flush;
#endif
}
#ifdef _WIN32
char getKey() {
    return _getch();
}
#else
void setTerminalMode(bool enableRaw) {
    static bool initialized = false;
    if (!initialized) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        initialized = true;
    }
    
    struct termios newt = orig_termios;
    if (enableRaw) {
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    }
}
int getKey() {
    setTerminalMode(true);
    char ch = 0;
    
    if (read(STDIN_FILENO, &ch, 1) <= 0) {
        setTerminalMode(false);
        return 0;
    }
    
    if (ch == 27) {
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) <= 0) return 27;
        if (read(STDIN_FILENO, &seq[1], 1) <= 0) return 27;
        if (seq[0] == '[') {
            if (seq[1] == 'A') return 72;    // Up arrow
            if (seq[1] == 'B') return 80;    // Down arrow
        }
        return 27;
    }
    
    if (ch == 10) ch = 13; // Convert Linux Enter to Windows-style
    
    setTerminalMode(false);
    return ch;
}
#endif
int select_menu(short x, short y, const std::vector<std::string>& items, short height, int color) {
    int index = 0;
    while (true) {
        for (size_t i = 0; i < items.size(); ++i) {
#ifdef _WIN32
            SetConsoleCursorPosition(hout, { x, static_cast<short>(y + i * height) });
            if (i == index) {
                SetConsoleTextAttribute(hout, color);
                std::cout << "> " << items[i];
                SetConsoleTextAttribute(hout, 15);
            } else {
                std::cout << "  " << items[i];
            }
            std::cout << std::endl;
#else
            // Position cursor and print item
            std::cout << "\033[" << (y + i * height) << ";" << x << "H";
            if (i == index) {
                // Convert Windows color to ANSI
                int fg = color & 0x0F;
                int bg = (color & 0xF0) >> 4;
                
                // ANSI color mapping (simplified)
                const int fg_map[] = {30, 34, 32, 36, 31, 35, 33, 37, 90, 94, 92, 96, 91, 95, 93, 97};
                const int bg_map[] = {40, 44, 42, 46, 41, 45, 43, 47, 100, 104, 102, 106, 101, 105, 103, 107};
                
                if (fg >= 0 && fg < 16) std::cout << "\033[" << fg_map[fg] << "m";
                if (bg >= 0 && bg < 16) std::cout << "\033[" << bg_map[bg] << "m";
                
                std::cout << "> " << items[i] << "\033[0m";
            } else {
                std::cout << "  " << items[i];
            }
            std::cout << std::flush;
#endif
        }
        int key = getKey();
        switch (key) {
        case 72: // Up arrow
            if (index > 0) index--;
            break;
        case 80: // Down arrow
            if (index < static_cast<int>(items.size()) - 1) index++;
            break;
        case 13: // Enter
            return index;
        }
    }
}
