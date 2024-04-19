#include <iostream>
using namespace std;
#include "Chessboard_Manager.h"


int main()
{
    Chessboard_Manager cm;

    int choice;
    while (true) {
        cm.show_Menu();
        cout << "请输入您的选择：";
        cin >> choice;

        switch (choice)
        {
        case 1:         //1.开始游戏
            cm.start_Game();
            system("pause");
            system("cls");
            break;
        case 2:         //2.游戏说明
            cm.show_GameRule();
            system("pause");
            system("cls");
            break;
        case 3:         //3.荣誉榜单
            cm.show_HonoursList();
            system("pause");
            system("cls");
            break;
        case 4:         //4.清空记录
            cm.clear_Record();
            system("pause");
            system("cls");
            break;
        case 0:         //0.退出游戏
            cm.exit_Game();
            break;
        default:
            cout << "输入不合法，请重新输入" << endl;
            system("pause");
            system("cls");
            break;
        }
    }

    system("pause");
    return 0;
}
