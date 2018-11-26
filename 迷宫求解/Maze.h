#pragma once
#include "stdafx.h"

//���Ѱ·ʱ��ջ�������
typedef struct Node
{
	POINT pt;	//����
	int derect;	//��ǰ����
	bool up;		//��(�����Ƿ����ͨ��)
	bool down;	//��
	bool left;	//��
	bool right;	//��
	int dre[2];	//�����жϵ���������
	bool isPop;	//�Ƿ��ջ�ﵯ��
}Node;


#ifndef MAZE_H
#define MAZE_H

//�Թ�ϵͳ��
class Maze
{
private:
	int map[20][20];	//��ͼ
	int FLAG_COLOR;	//��ǰ��ɫ
	bool redFlag;		//�Ƿ������
	bool greenFlag;	//�Ƿ��г���
	POINT redPt;		//�������
	POINT greenPt;	//��������
public:
	Maze();		//���캯��
	void PaintButton();	//������ť
	void PaintMap(HWND hWnd);		//���ݵ�ͼ�����Թ�
	void RePaint();		//�ػ��Թ�
	void PaintRect(POINT temp);	//������
	void SetColor(int);		//ѡ�����
	void SelectFileMap();		//ѡ���ͼ�ļ�
	void SaveMapToFile();		//�����ͼ�ļ�
	void FindRoute();			//Ѱ·
	~Maze();		//��������
private:	//����Ϊ˽�г�Ա����
	void _PaintPassage();		//����ͨ·
	void _SetDerect(Node &obj);	//ѡ����
	bool _SelectNextNode(Node &curNode,Node &nextNode);	//ѡ����һ�����
	bool _Judge(Node &obj, int m, Node &nextNode);	//�ж�
	void _SetFrontTaboo(Node &obj);		//���ý��ɱ�
	void _SetReverseTaboo(Node &obj);	//��������ջ�����ɱ�
	bool _FindNodePtFromVtr(POINT pt);	
	bool _FindNodePtFromTaboo(POINT pt);
	void _PaintCurNode(POINT pt);		//������ǰ��������
};

#endif