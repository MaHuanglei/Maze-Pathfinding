#pragma once
#include "stdafx.h"

//深度寻路时入栈结点类型
typedef struct Node
{
	POINT pt;	//坐标
	int derect;	//当前方向
	bool up;		//上(向上是否可以通过)
	bool down;	//下
	bool left;	//左
	bool right;	//右
	int dre[2];	//优先判断的两个方向
	bool isPop;	//是否从栈里弹出
}Node;


#ifndef MAZE_H
#define MAZE_H

//迷宫系统类
class Maze
{
private:
	int map[20][20];	//地图
	int FLAG_COLOR;	//当前颜色
	bool redFlag;		//是否有入口
	bool greenFlag;	//是否有出口
	POINT redPt;		//入口坐标
	POINT greenPt;	//出口坐标
public:
	Maze();		//构造函数
	void PaintButton();	//创建按钮
	void PaintMap(HWND hWnd);		//根据地图画出迷宫
	void RePaint();		//重画迷宫
	void PaintRect(POINT temp);	//画矩形
	void SetColor(int);		//选择材料
	void SelectFileMap();		//选择地图文件
	void SaveMapToFile();		//保存地图文件
	void FindRoute();			//寻路
	~Maze();		//析构函数
private:	//以下为私有成员函数
	void _PaintPassage();		//画出通路
	void _SetDerect(Node &obj);	//选择方向
	bool _SelectNextNode(Node &curNode,Node &nextNode);	//选择下一个结点
	bool _Judge(Node &obj, int m, Node &nextNode);	//判断
	void _SetFrontTaboo(Node &obj);		//设置禁忌表
	void _SetReverseTaboo(Node &obj);	//设置已入栈结点禁忌表
	bool _FindNodePtFromVtr(POINT pt);	
	bool _FindNodePtFromTaboo(POINT pt);
	void _PaintCurNode(POINT pt);		//画出当前所在坐标
};

#endif