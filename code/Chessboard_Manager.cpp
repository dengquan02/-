#include "Chessboard_Manager.h"

//构造
Chessboard_Manager::Chessboard_Manager()
{
	//初始化分数
	totalScore = 0;
	//初始化棋盘，随机放入7颗不同色的棋子
	init_Chessboard();
	//加载往届记录
	load_Record();
}


//菜单功能
void Chessboard_Manager::show_Menu()
{
	cout << "********************************************" << endl;
	cout << "****************  五子连珠  ****************" << endl;
	cout << "********************************************" << endl;
	cout << "**************** 1.开始游戏 ****************" << endl;
	cout << "**************** 2.游戏说明 ****************" << endl;
	cout << "**************** 3.荣誉榜单 ****************" << endl;
	cout << "**************** 4.清空记录 ****************" << endl;
	cout << "**************** 0.退出游戏 ****************" << endl;
	cout << "********************************************" << endl;
	cout << "********************************************" << endl;
	cout << endl;
}


//退出游戏
void Chessboard_Manager::exit_Game()
{
	cout << "欢迎下次使用！" << endl;
	exit(0);
}


//初始化棋盘
void Chessboard_Manager::init_Chessboard()
{
	//随机数种子
	srand((unsigned int)time(NULL));

	//全部单元置为空格符
	memset(Chessboard, ' ', sizeof(Chessboard));

	//放入7颗不同色的棋子
	int num = 0;
	while (num < 7) {
		int i = rand() % ROW;
		int j = rand() % COLUMN;
		if (Chessboard[i][j] == ' ') {
			Chessboard[i][j] = chessPieces[rand() % 7];
			num++;
		}
	}
}


//开始游戏
void Chessboard_Manager::start_Game()
{
	//初始化
	totalScore = 0;
	init_Chessboard();

	cout << "请输入您的游戏昵称：";
	cin >> playerName;
	cout << "游戏开始！！！" << endl;
	system("pause");

	//打印 9×9 棋盘
	print_Chessboard();
	
	while (true)
	{
		//移动棋子
		move_Chess();

		//消除连珠棋子
		//得分只可能出现在移动的棋子身上，终止坐标to传入eliminate_Chess，只对移动后的棋子进行判断和计算
		bool is_successful = eliminate_Chess(To);

		//如果消除了棋子，结算得分并打印新棋盘，否则分配3个随机颜色棋子分布到棋盘任意空位
		if (is_successful) {
			get_Score();
			cout << "干得漂亮！ 消除了" << cnt << "个棋子，得到" << score << "分。";
			Sleep(1000);
			print_Chessboard();
		}
		else {
			cout << "-------------- 系统随机分配棋子中 --------------" << endl;
			Sleep(1000);
			generate_Chess();
			print_Chessboard();

			//检查3个棋子是否全部分配，否则说明棋盘已满，游戏结束
			if (randomPieces[2] == Chessboard_cell()) {
				break;
			}

			//消除连珠，若消除则打印新棋盘，否则检查棋盘是否已满
			bool sign = false;
			for (int i = 0; i < 3; i++) {
				if (eliminate_Chess(randomPieces[i])) {
					sign = true;
				}
			}
			if (sign) {
				cout << "消除了" << cnt << "个棋子，未得分。";
				Sleep(1000);
				print_Chessboard();
			}
			else if (is_fill()) {
				break;
			}
		}
	}

	cout << "棋盘已被占满，游戏结束！！！" << endl;
	cout << "您的最终得分为：" << totalScore << endl;
	//保存游戏记录并更新
	save_Record();
	load_Record();
}


//打印棋盘
void Chessboard_Manager::print_Chessboard()
{
	//清屏
	system("cls");

	cout << "     1   2   3   4   5   6   7   8   9  " << endl;
	cout << "   |---|---|---|---|---|---|---|---|---|" << endl;
	for (int i = 0; i < ROW; i++) {
		cout << " " << i + 1;
		for (int j = 0; j < COLUMN; j++) {
			cout << " | " << Chessboard[i][j];
		}
		cout << " |" << endl;
		cout << "   |---|---|---|---|---|---|---|---|---|" << endl;
	}
	cout << "\t最高分：";
	if (Record.empty()) {
		cout << 0;
	}
	else {
		cout << Record.begin()->first;
	}
	cout << "\t当前得分：" << totalScore << endl << endl;
}


//移动棋子
void Chessboard_Manager::move_Chess()
{
	while (true) {
		cout << "请输入 要移动的棋子的坐标（先行后列）：";
		cin >> From.r >> From.c;
		cout << "请输入 目的坐标：                      ";
		cin >> To.r >> To.c;

		//修正输入坐标（打印的棋盘行列数从1开始数，而Chessboard 数组的行列数从0开始数）
		From.r--; From.c--;
		To.r--; To.c--;

		//检查输入
		if (From.r < 0 || From.r >= 9 || From.c < 0 || From.c >= 9
			|| To.r < 0 || To.r >= 9 || To.c < 0 || To.c >= 9) {
			cout << "输入坐标不在棋盘上！请重新输入" << endl;
			continue;
		}
		if (Chessboard[From.r][From.c] == ' ') {
			cout << "起始坐标上没有棋子！请重新输入" << endl;
			continue;
		}
		if (Chessboard[To.r][To.c] != ' ') {
			cout << "目标坐标上已有棋子！请重新输入" << endl;
			continue;
		}

		//有效路径判断，有则显示棋子移动动画并退出循环，无则提示重新输入
		if (judge_Path()) {
			break;
		}
		else {
			cout << "此路不通！请重新输入" << endl;
		}
	}
}


//消除连珠
bool Chessboard_Manager::eliminate_Chess(Chessboard_cell center)
{
	/*计算各方向的连珠数，并记录连珠方向上的头尾两个邻单元的位置，
达到条件就消除（并记录总消除数和消除方向数，以便最后计算得分）*/
	directionNum = 0;
	cnt = 0;
	char flag = Chessboard[center.r][center.c];

	//行
	int L = 1;
	int R = 1;
	while (Chessboard[center.r][center.c - L] == flag) {
		L++;
	}
	while (Chessboard[center.r][center.c + R] == flag) {
		R++;
	}
	int cnt_Row = L + R - 1;
	if (cnt_Row >= 5) {
		directionNum++;
		cnt += cnt_Row;
		for (int i = -(L - 1); i <= R - 1; i++) {
			Chessboard[center.r][center.c + i] = ' ';
		}
	}

	//列
	int Up = 1;
	int Down = 1;
	while (Chessboard[center.r - Up][center.c] == flag) {
		Up++;
	}
	while (Chessboard[center.r + Down][center.c] == flag) {
		Down++;
	}
	int cnt_Column = Up + Down - 1;
	if (cnt_Column >= 5) {
		directionNum++;
		cnt += cnt_Column;
		for (int i = -(Up - 1); i <= Down - 1; i++) {
			Chessboard[center.r + i][center.c] = ' ';
		}
	}

	//主对角线（从左上至右下）
	int L_Up = 1;
	int R_Down = 1;
	while (Chessboard[center.r - L_Up][center.c - L_Up] == flag) {
		L_Up++;
	}
	while (Chessboard[center.r + R_Down][center.c + R_Down] == flag) {
		R_Down++;
	}
	int cnt_leadingDiagonal = L_Up + R_Down - 1;
	if (cnt_leadingDiagonal >= 5) {
		directionNum++;
		cnt += cnt_leadingDiagonal;
		for (int i = -(L_Up - 1); i <= R_Down - 1; i++) {
			Chessboard[center.r + i][center.c + i] = ' ';
		}
	}

	//副对角线（从左下至右上）
	int L_Down = 1;
	int R_Up = 1;
	while (Chessboard[center.r + L_Down][center.c - L_Down] == flag) {
		L_Down++;
	}
	while (Chessboard[center.r - R_Up][center.c + R_Up] == flag) {
		R_Up++;
	}
	int cnt_counterDiagonal = L_Down + R_Up - 1;
	if (cnt_counterDiagonal >= 5) {
		directionNum++;
		cnt += cnt_counterDiagonal;
		for (int i = -(L_Down - 1); i <= R_Up - 1; i++) {
			Chessboard[center.r - i][center.c + i] = ' ';
		}
	}

	//返回是否成功消除
	if (directionNum == 0) {
		return false;
	}
	return true;
}


//结算分数
void Chessboard_Manager::get_Score()
{
	//修正总消除数（若消除不止一个方向，会重复计入消除数）
	if (directionNum > 1) {
		cnt = cnt - directionNum + 1;
	}
	//计算分数并累计到成员属性m_Score中去
	score = cnt * 2;
	totalScore += score;
}


//生成3个随机颜色棋子分布到棋盘任意空位
void Chessboard_Manager::generate_Chess()
{
	//随机数种子
	srand((unsigned int)time(NULL));

	//初始化临时位置数组
	for (int i = 0; i < 3; i++) {
		randomPieces[i] = Chessboard_cell();
	}

	//放入3颗不同色的棋子
	int cnt = 0;
	while (cnt < 3) {
		if (is_fill()) {
			break;
		}
		int i = rand() % ROW;
		int j = rand() % COLUMN;
		int index = rand() % 7;
		if (Chessboard[i][j] == ' ') {
			Chessboard[i][j] = chessPieces[index];
			randomPieces[cnt].r = i;
			randomPieces[cnt].c = j;
			cnt++;
		}
	}
	
	////记录空位坐标
	//vector<Chessboard_cell>v;		
	//for (int i = 0; i < ROW; i++) {
	//	for (int j = 0; j < COLUMN; j++) {
	//		if (Chessboard[i][j] == ' ') {
	//			v.push_back(Chessboard_cell(i, j));
	//		}
	//	}
	//}
	////放入随机棋子
	//for (int num = 0; num < 3 && !v.empty(); num++) {
	//	char ch = chessPieces[rand() % 7];
	//	vector<Chessboard_cell>::iterator it = v.begin() + rand() % v.size();
	//	Chessboard[it->r][it->c] = ch;
	//	randomPieces[num] = Chessboard_cell(it->r, it->c);
	//	v.erase(it);
	//}
}


//判断棋盘是否被占满
bool Chessboard_Manager::is_fill()
{
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			if (Chessboard[i][j] == ' ') {
				return false;
			}
		}
	}
	return true;
}


//广度优先搜索
bool Chessboard_Manager::judge_Path()
{
	bool visited[ROW][COLUMN];			//记录棋盘上的单元格是否走到过
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COLUMN; j++) {
			visited[i][j] = false;
		}
	}

	//起始位置加入队列
	queue<Chessboard_cell>q;
	visited[From.r][From.c] = true;
	q.push(From);

	Chessboard_cell head = q.front();
	while (!q.empty()) {
		//更新队头元素值
		head = q.front();
		//找到终点为止
		if (head.r == To.r && head.c == To.c) {
			break;
		}
		//容器扩展相邻的元素（需判重）
		Chessboard_cell* ptr = new Chessboard_cell(head);
		Chessboard_cell p[4] = {
			Chessboard_cell(head.r - 1, head.c, ptr),
			Chessboard_cell(head.r + 1, head.c, ptr),
			Chessboard_cell(head.r, head.c - 1, ptr),
			Chessboard_cell(head.r, head.c + 1, ptr),
		};
		for (int i = 0; i < 4; i++) {
			if ((p[i].r < 0 || p[i].r >= 9 || p[i].c < 0 || p[i].c >= 9)
				|| Chessboard[p[i].r][p[i].c] != ' ') {
				continue;
			}
			if (visited[p[i].r][p[i].c]) {
				continue;
			}
			visited[p[i].r][p[i].c] = true;
			q.push(p[i]);
		}
		//删除队头元素
		q.pop();
	}

	//如果找到终点，打印路径
	if (!q.empty()) {
		deque<Chessboard_cell> path;

		Chessboard_cell* p = &head;
		while (p != NULL) {
			path.push_front(*p);
			p = p->f;
		}
		for (deque<Chessboard_cell>::iterator it = path.begin() + 1; it != path.end(); it++) {
			Chessboard[it->r][it->c] = Chessboard[(it - 1)->r][(it - 1)->c];
			Chessboard[(it - 1)->r][(it - 1)->c] = ' ';
			print_Chessboard();
			Sleep(50);
		}
		return true;
	}
	return false;
}


//加载游戏说明
void Chessboard_Manager::show_GameRule()
{
	cout << "\n游戏规则如下：\n\n"
		<< "棋盘大小是9X9，初始状态棋盘上随机分布着7个不同色的棋子。通过移动棋子将同色的棋子连在一起来得分（同色棋子用相同字符来代替）。棋子的移动通过命令行输入起始坐标和终止坐标来模拟完成。\n\n"
		<< "1、 一次只允许移动一个棋子，且必须存在可移动路径时方可移动棋子，即在移动路径上存在棋子阻挡就无法移动。" << endl
		<< "2、 每移动一次棋子后，将出现3个随机颜色棋子分布到棋盘任意空置的位置上。" << endl
		<< "3、 当同色的棋子有5颗及以上连在一起排成横向、纵向或者斜向时，游戏者可以得分，得分为：消除棋子数 * 2，并获得一次移动棋子的机会，同时这些棋子从棋盘上消失。" << endl
		<< "5、 如果移动一个棋子之后，有多个方向都可以消除，则两个方向的所有棋子都消除。" << endl
		<< "7、 如果系统随机产生的棋子正好能凑成了同色的5颗及以上一起排成横向、纵向或者斜向，则这几颗同向的棋子自行消去，但不得分。" << endl
		<< "8、 当棋盘被棋子占满时游戏结束。" << endl;
}


//保存记录
void Chessboard_Manager::save_Record()
{
	ofstream ofs;
	ofs.open(FILENAME, ios::out | ios::app);	//用追加的方式写文件
	ofs << endl << totalScore << "," << playerName << ",";
	cout << "此次游戏记录保存成功！" << endl;
	ofs.close();
}


//读取记录
void Chessboard_Manager::load_Record()
{
	Record.clear();
	ifstream ifs(FILENAME, ios::in);

	if (!ifs.is_open()) {
		//cout << "文件不存在" << endl;
		ifs.close();
		return;
	}

	char ch;
	ifs >> ch;
	if (ifs.eof()) {
		//cout << "文件为空" << endl;		
		ifs.close();
		return;
	}

	//文件不为空
	ifs.putback(ch);
	string data;
	while (ifs >> data) {	
		int start = 0;
		int point;
		string name;
		for (int i = 0; i < 2; i++) {
			int pos = data.find(",", start);		//查到","位置的变量	
			if (i == 0) {
				point = stoi(data.substr(start, pos - start));
				start = pos + 1;
			}
			else {
				name = data.substr(start, pos - start);
				Record.insert(make_pair(point, name));
			}
		}
	}

	ifs.close();
}

//删除记录
void Chessboard_Manager::clear_Record()
{
	cout << "确认清空当前所有游戏记录？" << endl
		<< "1、确认" << endl
		<< "2、返回" << endl;

	int select = 0;
	cin >> select;
	if (select == 1)
	{
		//如果存在 删除文件并重新创建
		ofstream ofs(FILENAME, ios::trunc);
		ofs.close();

		//更新记录
		load_Record();

		cout << "清空成功！" << endl;
	}
}

//显示高分榜（前十名）
void Chessboard_Manager::show_HonoursList()
{
	if (!Record.empty()) {
		int i = 0;
		for (multimap<int, string, greater>::iterator it = Record.begin(); it != Record.end() && i < 10; it++, i++) {
			cout << "第" << i + 1 << "名\t得分：" << it->first << " \t玩家：" << it->second << endl;
		}
	}
	else {
		cout << "当前无游戏记录！" << endl;
	}
}



//析构
Chessboard_Manager::~Chessboard_Manager()
{
}
