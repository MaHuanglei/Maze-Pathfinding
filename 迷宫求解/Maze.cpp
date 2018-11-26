#include "stdafx.h"
#include "Maze.h"

extern HWND g_hWnd;
extern HINSTANCE hInst;
POINT last;
std::vector<Node> myvtr;
std::vector<POINT> taboo;

Maze::Maze()
{
	memset(map, 0, 1600);
	redFlag = false;
	greenFlag = false;
	redPt.x = redPt.y = -1;
	greenPt.x = greenPt.y = -1;
}

void Maze::PaintButton()
{
	CreateWindow(_T("static"), _T("材料："), WS_CHILD | WS_VISIBLE,
		500, 50, 180, 50, g_hWnd, (HMENU)1001, hInst, NULL);
	CreateWindow(_T("button"), _T("通道"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP,
		550, 50, 75, 50, g_hWnd, (HMENU)1002, hInst, NULL);
	CreateWindow(_T("button"), _T("墙体"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		625, 50, 75, 50, g_hWnd, (HMENU)1003, hInst, NULL);
	SendMessage(GetDlgItem(g_hWnd, 1003), BM_SETCHECK, 1, 0);
	SetColor(BLACK_B);

	CreateWindow(_T("static"), _T("出入口："), WS_CHILD | WS_VISIBLE,
		500, 130, 180, 50, g_hWnd, (HMENU)2001, hInst, NULL);
	CreateWindow(_T("button"), _T("入口"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		550, 130, 75, 50, g_hWnd, (HMENU)2002, hInst, NULL);
	CreateWindow(_T("button"), _T("出口"), WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		625, 130, 75, 50, g_hWnd, (HMENU)2003, hInst, NULL);

	CreateWindow(_T("button"), _T("选择地图"), WS_CHILD | WS_VISIBLE,
		505, 210, 75, 60, g_hWnd, (HMENU)3000, hInst, NULL);
	CreateWindow(_T("button"), _T("重画地图"), WS_CHILD | WS_VISIBLE,
		620, 210, 75, 60, g_hWnd, (HMENU)3001, hInst, NULL);
	CreateWindow(_T("button"), _T("保存地图"), WS_CHILD | WS_VISIBLE,
		505, 300, 75, 60, g_hWnd, (HMENU)3002, hInst, NULL);
	CreateWindow(_T("button"), _T("迷宫寻路"), WS_CHILD | WS_VISIBLE,
		620, 300, 75, 60, g_hWnd, (HMENU)3003, hInst, NULL);
	CreateWindow(_T("button"), _T("退出程序"), WS_CHILD | WS_VISIBLE,
		520, 400, 150, 40, g_hWnd, (HMENU)3004, hInst, NULL);
}

void Maze::PaintMap(HWND hWnd)
{
	HDC hdc = GetDC(hWnd);
	Rectangle(hdc, 50, 50, 451, 451);
	for (int i = 1; i < 20; i++)
	{
		MoveToEx(hdc, 50, 20 * i + 50, NULL);
		LineTo(hdc, 451, 20 * i + 50);

		MoveToEx(hdc, 20 * i + 50, 50, NULL);
		LineTo(hdc, 20 * i + 50, 451);
	}
	
	int *tour = (int*)map;
	HBRUSH brush[6];
	RECT rect;
	brush[2] = CreateSolidBrush(BLACK_B);
	brush[3] = CreateSolidBrush(RED_B);
	brush[4] = CreateSolidBrush(GREEN_B);
	brush[5] = CreateSolidBrush(BLUE_B);
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			if (*tour == 1)
			{
				tour += 1;
				continue;
			}
			rect.left = 50 + x * 20 + 1;
			rect.top = 50 + y * 20 + 1;
			rect.right = rect.left + 19;
			rect.bottom = rect.top + 19;
			FillRect(hdc, &rect, brush[*tour]);
			if (*tour == 3)
			{
				redPt.x = x;
				redPt.y = y;
				redFlag = true;
			}
			else if (*tour == 4)
			{
				greenPt.x = x;
				greenPt.y = y;
				greenFlag = true;
			}
			tour += 1;
		}
	}

	for (int i = 0; i < 6; i++)
		DeleteObject(brush[i]);
	ReleaseDC(hWnd, hdc);
}

void Maze::RePaint()
{
	memset(map, 0, 1600);
	redFlag = false;
	greenFlag = false;
	redPt.x = redPt.y = -1;
	greenPt.x = greenPt.y = -1;
	PaintMap(g_hWnd);
}

void Maze::PaintRect(POINT temp)
{
	HDC hdc = GetDC(g_hWnd);
	HBRUSH hr = CreateSolidBrush(FLAG_COLOR); //实心画刷
	SelectObject(hdc, hr);
	RECT rect;
	rect.left = temp.x * 20 + 51;
	rect.top = temp.y * 20 + 51;
	rect.right = rect.left + 19;
	rect.bottom = rect.top + 19;
	FillRect(hdc, &rect, hr);
	DeleteObject(hr);
	if (FLAG_COLOR == WHITE_B)
		map[temp.y][temp.x] = 1;
	else if (FLAG_COLOR == BLACK_B)
		map[temp.y][temp.x] = 2;
	else if (FLAG_COLOR == RED_B)
		map[temp.y][temp.x] = 3;
	else if (FLAG_COLOR == GREEN_B)
		map[temp.y][temp.x] = 4;

	if (FLAG_COLOR != RED_B && temp.x == redPt.x && temp.y == redPt.y)
	{
		redPt.x = -1;
		redPt.y = -1;
		redFlag = false;
	}
	else if (FLAG_COLOR != GREEN_B && temp.x == greenPt.x && temp.y == greenPt.y)
	{
		greenPt.x = -1;
		greenPt.y = -1;
		greenFlag = false;
	}

	if (FLAG_COLOR == RED_B)
	{
		if (redFlag && (redPt.x != temp.x || redPt.y != temp.y))
		{
			hr = CreateSolidBrush(WHITE_B);
			SelectObject(hdc, hr);
			rect.left = redPt.x * 20 + 51;
			rect.top = redPt.y * 20 + 51;
			rect.right = rect.left + 19;
			rect.bottom = rect.top + 19;
			FillRect(hdc, &rect, hr);
			DeleteObject(hr);
			map[redPt.y][redPt.x] = 1;

			redPt.x = temp.x;
			redPt.y = temp.y;
		}
		else
		{
			redPt.x = temp.x;
			redPt.y = temp.y;
			redFlag = true;
		}
	}
	else if (FLAG_COLOR == GREEN_B)
	{
		if (greenFlag && (greenPt.x != temp.x || greenPt.y != temp.y))
		{
			hr = CreateSolidBrush(WHITE_B);
			SelectObject(hdc, hr);
			rect.left = greenPt.x * 20 + 51;
			rect.top = greenPt.y * 20 + 51;
			rect.right = rect.left + 19;
			rect.bottom = rect.top + 19;
			FillRect(hdc, &rect, hr);
			DeleteObject(hr);
			map[greenPt.y][greenPt.x] = 1;
			greenPt.x = temp.x;
			greenPt.y = temp.y;
		}
		else
		{
			greenPt.x = temp.x;
			greenPt.y = temp.y;
			greenFlag = true;
		}
	}
	ReleaseDC(g_hWnd, hdc);
}

void Maze::SetColor(int color)
{
	FLAG_COLOR = color;
}

void Maze::SelectFileMap()
{
	TCHAR szFileFilter[40] = L"源文件(*.map|*.map|)";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_NOCHANGEDIR, szFileFilter);

	CString fileName;
	if (dlg.DoModal() == IDOK)
	{
		fileName = dlg.GetFileName();
	}
	else return;

	std::ifstream ifile(fileName);
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			ifile >> map[y][x];
		}
	}
	redFlag = false;
	greenFlag = false;
	redPt.x = redPt.y = -1;
	greenPt.x = greenPt.y = -1;
	PaintMap(g_hWnd);
}

void Maze::SaveMapToFile()
{
	CTime time = CTime::GetCurrentTime();
	CString fileName;
	fileName.Format(L"%s%s", time.Format("%Y%m%d%H%M%S"), L".map");

	HDC hdc = GetDC(g_hWnd);
	std::string str = "";
	HANDLE hfileHandle = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	int color;
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 20; x++)
		{
			color = GetPixel(hdc, 50 + x * 20 + 10, 50 + y * 20 + 10);
			switch (color)
			{
			case BLACK_B:
				str += "2 ";
				break;
			case RED_B:
				str += "3 ";
				break;
			case GREEN_B:
				str += "4 ";
				break;
			default:
				str += "1 ";
				break;
			}
		}
		str += "\r\n";
		WriteFile(hfileHandle, str.c_str(), str.length(), 0, 0);
		str = "";
	}
	MessageBox(g_hWnd, L"保存成功", L"提示", MB_OK);
	CloseHandle(hfileHandle);
}

void Maze::FindRoute()
{
	if (!redFlag)
	{
		MessageBox(g_hWnd, L"地图缺少入口", L"提示", MB_OK);
		return;
	}
	else if (!greenFlag)
	{
		MessageBox(g_hWnd, L"地图缺少出口", L"提示", MB_OK);
		return;
	}
	myvtr.clear();
	taboo.clear();
	Node starNode;
	starNode.pt = redPt;
	_SetDerect(starNode);
	Node curNode = starNode, nextNode;
	curNode.derect = 0;
	if (!_SelectNextNode(curNode, nextNode))
	{
		MessageBox(g_hWnd, L"没有通路", L"提示", MB_OK);
		return;
	}
	else
	{
		myvtr.push_back(curNode);
		curNode = nextNode;
	}
	last.x = last.y = -1;
	bool flag = false;
	while (1)
	{
		if (curNode.pt.x == greenPt.x && curNode.pt.y == greenPt.y)
		{
			flag = true;
			break;
		}
		_PaintCurNode(curNode.pt);
		if (!_SelectNextNode(curNode, nextNode))
		{
			if (curNode.pt.x == starNode.pt.x&&curNode.pt.y == starNode.pt.y)
			{
				break;
			}
			if (curNode.isPop)taboo.push_back(curNode.pt);
			curNode = myvtr.back();
			myvtr.pop_back();
			_SetFrontTaboo(curNode);
			curNode.isPop = true;
		}
		else
		{
			myvtr.push_back(curNode);
			curNode = nextNode;
		}
	}
	if (flag)
	{
		HDC hdc = GetDC(g_hWnd);
		HBRUSH hr2 = CreateSolidBrush(WHITE_B);
		RECT rect;
		rect.left = last.x * 20 + 51;
		rect.top = last.y * 20 + 51;
		rect.right = rect.left + 19;
		rect.bottom = rect.top + 19;
		FillRect(hdc, &rect, hr2);
		DeleteObject(hr2);
		ReleaseDC(g_hWnd, hdc);
		_PaintPassage();
	}
	else
	{
		MessageBox(g_hWnd, L"没有通路", L"提示", MB_OK);
	}
}

void Maze::_PaintPassage()
{
	HDC dc = GetDC(g_hWnd);
	HBRUSH brush = CreateSolidBrush(BLUE_B);
	RECT rect;
	int size = myvtr.size();
	for (int i = 1; i < size; i++)
	{
		rect.left = myvtr[i].pt.x * 20 + 51;
		rect.top = myvtr[i].pt.y * 20 + 51;
		rect.right = rect.left + 19;
		rect.bottom = rect.top + 19;
		FillRect(dc, &rect, brush);
		map[myvtr[i].pt.y][myvtr[i].pt.x] = 5;
		Sleep(70);
	}
	DeleteObject(brush);
	ReleaseDC(g_hWnd, dc);
}

void Maze::_SetDerect(Node &obj)
{
	if (obj.pt.x > 0 && map[obj.pt.y][obj.pt.x - 1] != 2)obj.left = true;
	else obj.left = false;
	if (obj.pt.x < 19 && map[obj.pt.y][obj.pt.x + 1] != 2)obj.right = true;
	else obj.right = false;
	if (obj.pt.y > 0 && map[obj.pt.y - 1][obj.pt.x] != 2)obj.up = true;
	else obj.up = false;
	if (obj.pt.y < 19 && map[obj.pt.y + 1][obj.pt.x] != 2)obj.down = true;
	else obj.down = false;

	if (obj.pt.y < greenPt.y)obj.dre[0] = 2;
	else if (obj.pt.y > greenPt.y)obj.dre[0] = 1;
	else if (obj.pt.y == greenPt.y)
	{
		if (obj.pt.x < greenPt.x)obj.dre[0] = 4;
		else if (obj.pt.x > greenPt.x)obj.dre[0] = 3;
		else obj.dre[0] = 0;
	}

	if (obj.dre[0] == 1 || obj.dre[0] == 2)
	{
		if (obj.pt.x < greenPt.x)obj.dre[1] = 4;
		else if (obj.pt.x > greenPt.x)obj.dre[1] = 3;
		else obj.dre[1] = 0;
	}
	else if (obj.dre[0] == 3 || obj.dre[0] == 4)
	{
		if (obj.pt.y < greenPt.y)obj.dre[1] = 2;
		else if (obj.pt.y > greenPt.y)obj.dre[1] = 1;
		else obj.dre[1] = 0;
	}
	else obj.dre[1] = 0;

	obj.isPop = false;
}

bool Maze::_Judge(Node &obj, int m, Node &nextNode)
{
	bool flag=false;
	POINT temp;
	switch (m)
	{
	case 1:
		if (obj.up)
		{
			temp.x = obj.pt.x;
			temp.y = obj.pt.y - 1;
			if (_FindNodePtFromVtr(temp))break;
			else if (_FindNodePtFromTaboo(temp))break;
			nextNode.pt = temp;
			flag = true;
			obj.derect=nextNode.derect = 1;
			_SetDerect(nextNode);
		}
		break;
	case 2:
		if (obj.down)
		{
			temp.x = obj.pt.x;
			temp.y = obj.pt.y + 1;
			if (_FindNodePtFromVtr(temp))break;
			else if (_FindNodePtFromTaboo(temp))break;
			flag = true;
			nextNode.pt = temp;
			obj.derect = nextNode.derect = 2;
			_SetDerect(nextNode);
		}
		break;
	case 3:
		if (obj.left)
		{
			temp.x = obj.pt.x - 1;
			temp.y = obj.pt.y;
			if (_FindNodePtFromVtr(temp))break;
			else if (_FindNodePtFromTaboo(temp))break;
			flag = true;
			nextNode.pt = temp;
			obj.derect = nextNode.derect = 3;
			_SetDerect(nextNode);
		}
		break;
	case 4:
		if (obj.right)
		{
			temp.x = obj.pt.x + 1;
			temp.y = obj.pt.y;
			if (_FindNodePtFromVtr(temp))break;
			else if (_FindNodePtFromTaboo(temp))break;
			flag = true;
			nextNode.pt = temp;
			obj.derect = nextNode.derect = 4;
			_SetDerect(nextNode);
		}
		break;
	}
	return flag;
}

bool Maze::_SelectNextNode(Node &curNode, Node &nextNode)
{
	bool flag = false;
	if(!curNode.isPop)_SetReverseTaboo(curNode);
	if (curNode.dre[0] && _Judge(curNode, curNode.dre[0], nextNode))
	{
		flag = true;
	}
	else if (curNode.dre[1] && _Judge(curNode, curNode.dre[1], nextNode))
	{
		flag = true;
	}
	else
	{
		for (int i = 1; i < 5; i++)
		{
			if (_Judge(curNode, i, nextNode))
			{
				flag = true;
				break;
			}
		}
	}
	return flag;
}

void Maze::_SetFrontTaboo(Node &obj)
{
	switch (obj.derect)
	{
	case 1:
		obj.up = false;
		break;
	case 2:
		obj.down = false;
		break;
	case 3:
		obj.left = false;
		break;
	case 4:
		obj.right = false;
		break;
	}
}

void Maze::_SetReverseTaboo(Node &obj)
{
	switch (obj.derect)
	{
	case 1:
		obj.down = false;
		break;
	case 2:
		obj.up = false;
		break;
	case 3:
		obj.right = false;
		break;
	case 4:
		obj.left = false;
		break;
	default:
		break;
	}
}

bool Maze::_FindNodePtFromVtr(POINT pt)
{
	bool flag = false;
	std::vector<Node>::iterator it = myvtr.begin();
	for (; it != myvtr.end(); it++)
	{
		if (it->pt.x == pt.x&&it->pt.y == pt.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

bool Maze::_FindNodePtFromTaboo(POINT pt)
{
	bool flag = false;
	std::vector<POINT>::iterator it = taboo.begin();
	for (; it != taboo.end(); it++)
	{
		if (it->x == pt.x&&it->y == pt.y)
		{
			flag = true;
			break;
		}
	}
	return flag;
}

void Maze::_PaintCurNode(POINT pt)
{
	HDC hdc = GetDC(g_hWnd);
	HBRUSH hr1 = CreateSolidBrush(BLUE_B);
	HBRUSH hr2 = CreateSolidBrush(WHITE_B);
	HBRUSH hr3 = CreateSolidBrush(RED_B);
	RECT rect;
	rect.left = pt.x * 20 + 51;
	rect.top = pt.y * 20 + 51;
	rect.right = rect.left + 19;
	rect.bottom = rect.top + 19;
	FillRect(hdc, &rect, hr1);
	if (last.x != -1)
	{
		rect.left = last.x * 20 + 51;
		rect.top = last.y * 20 + 51;
		rect.right = rect.left + 19;
		rect.bottom = rect.top + 19;
		if (map[last.y][last.x] == 3)
			FillRect(hdc, &rect, hr3);
		else FillRect(hdc, &rect, hr2);
	}
	last = pt;
	DeleteObject(hr1);
	DeleteObject(hr2);
	DeleteObject(hr3);
	ReleaseDC(g_hWnd, hdc);
	Sleep(40);
}

Maze::~Maze()
{

}
