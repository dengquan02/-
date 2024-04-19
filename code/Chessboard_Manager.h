#pragma once
#include <iostream>
#include <fstream>
#include <cstdlib> 
#include <ctime>
#include <windows.h>
#include <map>
#include <vector>
#include <queue>
#include <string>
#include "Chessboard_cell.h"

#define ROW  9
#define COLUMN 9
#define FILENAME "HonoursList.txt"

using namespace std;


class Chessboard_Manager
{
public:
	//构造
	Chessboard_Manager();

	//菜单功能
	void show_Menu();

	//退出游戏
	void exit_Game();

	//初始化棋盘
	void init_Chessboard();


	//开始游戏
	void start_Game();

	//打印棋盘
	void print_Chessboard();

	//移动棋子
	void move_Chess();

	//消除连珠棋子，并返回是否成功消除的结果
	bool eliminate_Chess(Chessboard_cell cell);

	//结算回合得分
	void get_Score();

	//分配3个随机颜色棋子
	void generate_Chess();

	//判断棋盘是否被占满
	bool is_fill();

	//有效路径判断
	bool judge_Path();


	//打印游戏说明
	void show_GameRule();

	//保存记录
	void save_Record();
	//读取记录
	void load_Record();
	//删除记录
	void clear_Record();

	//显示高分榜
	void show_HonoursList();


	//析构
	~Chessboard_Manager();


private:
	char chessPieces[7] = { 'A','B','C','D','E','F','G' };      //7种棋子
	char Chessboard[ROW][COLUMN];								//存放棋盘每个单元
	int totalScore;												//总得分
	string playerName;								//玩家昵称
	class greater
	{
	public:
		bool operator()(int v1, int v2) const
		{
			return v1 > v2;
		}
	};
	multimap<int, string, greater>Record;           //存放往届游戏记录

	//临时属性
	Chessboard_cell From;					//起始坐标
	Chessboard_cell To;						//终止坐标
	Chessboard_cell randomPieces[3];		//存放每回合系统随机分配的三个棋子的坐标
	int directionNum = 0;					//每回合可消除方向的数量
	int cnt = 0;							//每回合消除的棋子数
	int score = 0;							//每回合的得分
};