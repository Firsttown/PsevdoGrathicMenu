# PsevdoGrathicMenu
просто меню для консольных проектов C++
В Menu.h есть пример использования
## пример использования
```
#include "Menu.h"
usning namespace std;
int main()
{
    //подменю
    vector<string> secondLabel = {
        "цвет", "шрифт", "назад"
    }
    //основное меню
    vector<string> mainLable = {
    "время", "настройки", "выход",
    }


    int x = 2; // отступ от левого края
    int y = 5; // отступ от верхнего края
    
    int interval = 2; // интервал между пунктами
    
    while(true)
    {
        clearScreen();
        cout << "главное мню" << endl;
        int choice = select_menu(x, y, mainLable, internal);
        
        switch(choice)
        {
            case 0:
                ...
                break;
            
            case 1:
                while(true)
                {
                    clearScreen();
                    cout << "настройки" << endl;
                    int subchoice = select_menu(x, y, secondLabel, internal);
                    
                    if(subchoice == 0)
                    {
                        ...
                    }
                    else if(subchoice == 1)
                    {
                        ...
                    }
                    else
                    {
                        break;
                    }
                }    
                break;
            
            case 2:
                cout << "выход из программы" << endl;
                return 0;
        }
    }
}
```
