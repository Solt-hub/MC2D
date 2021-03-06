#include <stdio.h>
#include<conio.h>
#include<string.h>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include<windows.h>
#include<Mmsystem.h>
#pragma comment(lib,"winmm.lib")
using namespace std;

char map[100][100] = { 0 };
int m = 61;
int n = 62;
int curx = 0, cury = 0;
vector<char> pbag{};//玩家背包
vector<int> pbnj{};
int hand = -1;//背包里的下标
bool isshift = false;
char hc4[2][2];
char hc9[3][3];
bool isredraw=false;
ifstream fin;
ofstream fout;
HWND hwnd = GetForegroundWindow();//使hwnd代表最前端的窗口 
string cpath;
BOOL playbgm()
{
	return PlaySound(TEXT("C:\\MC2D\\music\\bgm.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}
void hidecursor()
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
bool check(int xory)
{
	return xory >= 0 && xory <= m && map? true : false;
}
BOOL setcolor(int Forgc, int Backc)
{
	WORD wcolor = ((Backc & 0x0F) << 4) + (Forgc & 0x0F);
	return SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wcolor);
}
void color(char ch)
{
	switch (ch)
	{
	case 'T':
		setcolor(0xF,2);
		break;
	case '#':
		setcolor(0xF, 6);
		break;
	case '=':
		setcolor(0xF, 8);
		break;
	case '@':
		setcolor(0xF, 4);
		break;
	case '*':
		setcolor(0xF, 0);
		break;
	case 'A':
		setcolor(0xF, 3);
		break;
	case '0':
		setcolor(0xF, 5);
		break;
	    default:
		{
            break;
		}
		   
	}
}
char hecheng(char strs[2][2])
{
	if (strs[0][0] == 'T' && strs[1][0] == 'T')
	{
		return 'W';
	}
	if (strs[0][0] == '=' && strs[1][0] == 'T')
	{
		return 'S';
	}
	if (strs[0][0] == '@' && strs[1][0] == 'T')
	{
		return 'I';
	}
	return 0;
}
void gotoxy(int oldx,int oldy,int newx,int newy)
{
	isredraw = true;
	COORD pos{};
	pos.X = oldx;
	pos.Y = oldy;
	color('*');
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
	printf("*");
	map[oldy][oldx] = '*';
	color('A');
	if (!isshift)
	{
		map[newy][newx] = 'A';
		COORD pos2{};
		pos2.X = newx;
		pos2.Y = newy;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos2);
		printf("A");
	}
	else
	{
		map[newy][newx] = 'a';
		COORD pos2{};
		pos2.X = newx;
		pos2.Y = newy;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos2);
		printf("a");
	}
	setcolor(0xF, 0);
}
void out()
{
	if (isredraw==false)
	{
		system("cls");
		for (int i = 0; i <= m; i++)
		{
			for (int j = 0; j <= n; j++)
			{
				color(map[i][j]);
				cout << map[i][j];
			}
			setcolor(0xF, 0);
			cout << "\n";
		}
	}
	return;
}
void suiji(int seed)
{
	srand((unsigned)seed);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j <= n; j++)
		{
			map[i][j] = '*';
		}
	}
	for (int i = 0; i <= n; i++)
	{
		int t = rand() % 3;
		if (t == 0)
		{
			map[3][i] = 'T';//tree
		}
		else if(t == 2)
		{
			map[3][i] = '#';
		}
		else
		{
			map[3][i] = '*';
		}
	}
	for (int i = 4; i <= m; i++)
	{
		for (int j =0; j <= n; j++)
		{
			int t = rand() % 6;
			if (t == 0)
			{
				map[i][j] = '*';//air
			}
			if (t == 1)
			{
				map[i][j] = '#';//mud
			}
			if (t == 2)
			{
				map[i][j] = 'T';//stone
			}
			if (t == 3)
			{
				map[i][j] = '=';//stone
			}
			if (t == 4)
			{
				if (i>=10)
				{
					map[i][j] = '@';//iron
				}
				else
				{
					map[i][j] = '=';//stone
				}
			}
			if (t==5)
			{
				map[i][j] = '*';//air
			}
		}
	}
}
void move(char c)
{
	int nx = 0, ny = 0;
	switch (c)
	{
	case 'a':
		nx = curx - 1;
		if (check(nx))
		{
			if (map[cury][nx] == '*')
			{
				gotoxy(curx, cury, nx, cury);
				curx--;
			}
		}
		break;
	case 'd':
		nx = curx + 1;
		if (check(nx))
		{
			if (map[cury][nx] == '*')
			{
				gotoxy(curx, cury, nx, cury);
				curx++;
			}
		}
		break;
	case 'w':
		ny = cury - 1;
		if (check(ny))
		{
			if (map[ny][curx] == '*')
			{
				gotoxy(curx, cury, curx, ny);
				cury--;
			}
		}
		break;
	case 's':
		ny = cury + 1;
		if (check(ny))
		{
			if (map[ny][curx] == '*')
			{
				gotoxy(curx, cury, curx, ny);
				cury++;
			}
		}
		break;
	case 'j':
		isredraw = false;
		nx = curx - 1;
		if (check(nx))
		{
			if (map[cury][nx] == '*')
			{
				if (hand != -1 && pbag[hand] != 'W' && pbag[hand] != 'S' && pbag[hand] != 'I')
				{
					map[cury][nx] = pbag[hand];
					pbag.erase(pbag.begin() + hand);
					hand--;
				}
			}
			else
			{
				if (hand == -1)//手
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					pbnj.push_back(-1);
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}
				else if (pbag[hand] == 'W')//木镐
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[cury][nx] == '=')
					{
						pbag.push_back('=');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '=' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}
				else if (pbag[hand] == 'S')//石镐
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[cury][nx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[cury][nx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '=' || map[cury][nx] == '@' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}
				else if (pbag[hand] == 'I')//铁镐
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[cury][nx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[cury][nx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '=' || map[cury][nx] == '@' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}

			}
		}
		break;
	case 'l':
		nx = curx + 1;
		isredraw = false;
		if (check(nx))
		{
			if (map[cury][nx] == '*')
			{
				if (hand != -1 && pbag[hand] != 'W' && pbag[hand] != 'S' && pbag[hand] != 'I')
				{
					map[cury][nx] = pbag[hand];
					pbag.erase(pbag.begin() + hand);
					hand--;
				}
			}
			else
			{
				if (hand == -1)//手
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					pbnj.push_back(-1);
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}
				else if (pbag[hand] == 'W')//木镐
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[cury][nx] == '=')
					{
						pbag.push_back('=');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '=' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}
				else if (pbag[hand] == 'S')//石镐
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[cury][nx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[cury][nx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '=' || map[cury][nx] == '@' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}
				else if (pbag[hand] == 'I')//铁镐
				{
					if (map[cury][nx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[cury][nx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[cury][nx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[cury][nx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[cury][nx] = map[cury][nx] == 'T' || map[cury][nx] == '=' || map[cury][nx] == '@' || map[cury][nx] == '#' ? '*' : map[cury][nx];
					hand++;
				}

			}
		}
		break;
	case 'i':
		isredraw = false;
		ny = cury - 1;
		if (check(ny))
		{
			if (map[ny][curx] == '*')
			{
				if (hand != -1 && pbag[hand] != 'W' && pbag[hand] != 'S' && pbag[hand] != 'I')
				{
					map[ny][curx] = pbag[hand];
					pbag.erase(pbag.begin() + hand);
					hand--;
				}
			}
			else
			{
				if (hand == -1)//手
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
				else if (pbag[hand] == 'W')//木镐
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[ny][curx] == '=')
					{
						pbag.push_back('=');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '=' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
				else if (pbag[hand] == 'S')//石镐
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[ny][curx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[ny][curx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '=' || map[ny][curx] == '@' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
				else if (pbag[hand] == 'I')//铁镐
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[ny][curx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[ny][curx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '=' || map[ny][curx] == '@' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
			}
		}
		break;
	case 'k':
		isredraw = false;
		ny = cury + 1;
		if (check(ny))
		{
			if (map[ny][curx] == '*')
			{
				if (hand != -1 && pbag[hand] != 'W' && pbag[hand] != 'S' && pbag[hand] != 'I')
				{
					map[ny][curx] = pbag[hand];
					pbag.erase(pbag.begin() + hand);
					hand--;
				}
			}
			else
			{
				if (hand == -1)//手
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					pbnj.push_back(-1);
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
				else if (pbag[hand] == 'W')//木镐
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[ny][curx] == '=')
					{
						pbag.push_back('=');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '=' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
				else if (pbag[hand] == 'S')//石镐
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[ny][curx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[ny][curx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '=' || map[ny][curx] == '@' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}
				else if (pbag[hand] == 'I')//铁镐
				{
					if (map[ny][curx] == 'T')
					{
						pbag.push_back('T');
					}
					if (map[ny][curx] == '#')
					{
						pbag.push_back('#');
					}
					if (map[ny][curx] == '=')
					{
						pbag.push_back('=');
					}
					if (map[ny][curx] == '@')
					{
						pbag.push_back('@');
					}
					pbnj.push_back(-1);
					pbnj[hand]--;
					map[ny][curx] = map[ny][curx] == 'T' || map[ny][curx] == '=' || map[ny][curx] == '@' || map[ny][curx] == '#' ? '*' : map[ny][curx];
					hand++;
				}

			}
		}
		break;
	case 'b':
		isredraw = false;
		system("cls");
		if (pbag.size() > 0)
		{
			for (int i = 1; i <= pbag.size(); i++)
			{
				printf("%3d", i);
			}
			printf("\n");
			for (int i = 0; i < pbag.size(); i++)
			{
				printf("%3c", pbag[i]);
			}
			printf("\n");
			for (int i = 0; i < pbnj.size(); i++)
			{
				printf("%3d", pbnj[i]);
			}
			printf("\n");
			system("pause");
			printf("请选择你要干什么？\n[1]丢弃物品\n[2]设置手中物品\n[3]合成\n\[4]空手\n请选择");
			int ch = 0, ch2 = 0, ch3 = 0, ch4 = 0;
			char t = 0, jg = 0;
			vector<char> tuihuan{};
			vector<int> tihuan2{};
			scanf_s("%d", &ch);
			switch (ch)
			{
			case 1:
				printf("第几个？\n");
				ch2 = 0;
				scanf_s("%d", &ch2);
				if (ch2 > pbag.size())
				{
					printf("无效。");
					system("pause");
					return;
				}
				else
				{
					pbnj.erase(pbnj.begin() + ch2);
					pbag.erase(pbag.begin() + ch2);
					system("pause");
					return;
				}
				break;
			case 2:
				printf("第几个？\n");
				ch3 = 0;
				scanf_s("%d", &ch3);
				if (ch3 > pbag.size() || ch3 <= -1)
				{
					printf("无效。");
					system("pause");
					return;
				}
				else
				{
					hand = ch3 - 1;
					system("pause");
					return;
				}
				break;
			case 3:
				system("cls");
				printf("[ ][ ]\n");
				printf("[ ][ ]\n");
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						cout << "第" << i + 1 << "," << j + 1 << "个格子，放什么？(没有为'?')\n";
						cin >> t;
						vector<char>::iterator result = find(pbag.begin(), pbag.end(), t);
						int findd = -1;
						for (int i = 0; i < pbag.size(); i++)
						{
							if (pbag[i] == t)
							{
								findd = i;
								break;
							}
						}
						if (findd == -1 && t != '?')
						{
							cout << "史蒂猪的背包里没有这件物品！\n";
							system("pause");
							return;
						}
						else if (t != '?')
						{
							hc4[i][j] = t;
							tuihuan.push_back(t);
							tihuan2.push_back(pbnj[findd]);
							pbag.erase(pbag.begin() + findd);
							pbnj.erase(pbnj.begin() + findd);
						}
					}
				}
				system("cls");
				for (int i = 0; i < 2; i++)
				{
					for (int j = 0; j < 2; j++)
					{
						printf("[%c]", hc4[i][j] != '?' ? hc4[i][j] : ' ');
					}
					printf("\n");
				}
				printf("\n");
				jg = hecheng(hc4);
				if (jg != NULL)
				{
					printf("可合成的物品为%c，是否合成？(1/0)", jg);
					scanf_s("%d", &ch4);
					if (ch4 == 0)
					{
						system("pause");
						pbag.insert(pbag.end(), tuihuan.begin(), tuihuan.end());
						pbnj.insert(pbnj.end(), tihuan2.begin(), tihuan2.end());
						return;
					}
					else
					{
						pbag.push_back(jg);
						switch (jg)
						{
						case 'W':
						{
							pbnj.push_back(10);
							break;
						}
						case 'S':
						{
							pbnj.push_back(20);
							break;
						}
						case 'I':
						{
							pbnj.push_back(30);
							break;
						}
						default:
							break;
						}
						hand++;
					}
				}
				else
				{
					printf("出了什么问题？似乎有哪里不对劲...\n");
					pbag.insert(pbag.end(), tuihuan.begin(), tuihuan.end());
				}
				system("cls");
				system("pause");
				return;
				break;
			case 4:
				hand = -1;
				system("pause");
				return;
				break;
			}
		}
		else
		{
			printf("史蒂猪的背包里什么都没有...\n");
			system("pause");
			return;
		}
		break;
	case 'h':
	{
		hand = -1;
		break;
	}

	case 'o':
	{
		if (isshift)
		{
			isshift = false;
		}
		else
		{
			isshift = true;
		}
		break;
	}
	case 27:
	{
		int rs = MessageBox(hwnd, TEXT("你要退出吗?"), TEXT("提示"), MB_ICONINFORMATION | MB_OKCANCEL);
		if (rs == IDOK)
		{
			fout.open(cpath, ios::out | ios::app);
			if (pbag.size() == 0)
			{
				printf("?");
			}
			else
			{
				for (int i = 0; i < pbag.size(); i++)
				{
					fout << pbag[i];
				}
			}
			fout << "\n";
			for (int i = 0; i <= m; i++)
			{
				for (int j = 0; j <= n; j++)
				{
					if (map[i][j] == 'A')
					{
						map[i][j] = '*';
					}
					fout << map[i][j];
				}
			}
			fout << "\n";
			fout << curx << " " << cury;
			fout << "\n";
			for (int i = 0; i < pbnj.size(); i++)
			{
				fout << pbnj[i] << " ";
			}
			fout.close();
			exit(0);
		}
		break;
	}
	case 0:
	case -32:
	{
		char tmp = _getch();
		if (tmp >= 59 && tmp <= 68)
		{
			hand = tmp - 59;
		}
		break;
	}
	default:
		break;
	}
}

int main()
{
	ShowWindow(hwnd, SW_MAXIMIZE);
    ios::sync_with_stdio(false);
	hidecursor();
	playbgm();
	system("title MC2D");
	system("color 3F");
	printf("     Minecraft     \n");
	printf("    c++ edition    \n");
	setcolor(0xF, 6);
	printf("    作者is SB!     ");
	setcolor(0xF, 3);
	printf("\n");
	printf("[      1  玩      ]\n");
	printf("[      2  滚      ]\n");
	int a;
	scanf_s("%d", &a);
	system("color 0F");
	if (a == 2)
		exit(0);
	system("cls");
	printf("[      1  读取存档      ]\n");
	printf("[      2  新建存档      ]\n");
	int b;
	scanf("%d", &b);
	if (b == 1)
	{
		system("cls");
		char buffer[1024];
		vector<string> cd;
		ifstream fin2("C:\\MC2D\\cd.txt", ios::in);
		if (fin2.good())
		{
			while (fin2 >> buffer)
			{
				cd.push_back(buffer);
			}
			fin2.close();
			printf("存档有:\n");
			int ctt = 0, c;
			for (vector<string>::iterator it = cd.begin(); it != cd.end(); it++)
			{
				cout << ctt << " " << (*it) << endl;
				ctt++;
			}
			printf("第几个?\n");
			scanf("%d", &c);
			if (c>=cd.size())
			{
				MessageBox(hwnd, TEXT("此存档不存在!"), TEXT("阿哲"), MB_OK);
			}
			else
			{
				fin.open(cd[c], ios::in);
				if (fin.good())
				{
					string mp;
					string bg;
					fin >> bg >> mp >> curx >> cury;
					if (bg!="?")
					{
						pbag.resize(bg.size());
						pbag.assign(bg.begin(), bg.end());
						for (int i = 0; i < pbag.size(); i++)
						{
							int v;
							fin >> v;
							pbnj.push_back(v);
						}
					}
					for (int i = 0; i <= m; i++)
					{
						for (int j = 0; j <= n; j++)
						{
							map[i][j] = mp[static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * m + j];
						}
					}
					fin.close();
					cpath = cd[c];
				}
				else
				{
					MessageBox(hwnd, TEXT("你好像根本没有这个存档..."), TEXT("阿哲"), MB_OK);
				}
			}
		}
		else
		{
			MessageBox(hwnd, TEXT("你好像根本没有存档..."), TEXT("阿哲"), MB_OK);
		}
	}
	else
	{
		string d;
		printf("请输入存档名称\n");
		cin >> d;
		ofstream fout2("C:\\MC2D\\cd.txt", ios::out | ios::app);//打开存档信息
		fout2 << "C:\\MC2D\\" + d + ".txt" << "\n";//添加存档
		fout2.close();//end
		cpath = "C:\\MC2D\\" + d + ".txt";
		suiji(time(0));
	}
	while (1)
	{
		for (int i = 0; i < pbnj.size(); i++)
		{
			if (pbnj[i] == 0)
			{
				pbag.erase(pbag.begin() + i);
			}
		}
		out();
		char ch = _getch();
		move(ch);
	}
}
