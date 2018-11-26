// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif
#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
//#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO:  在此处引用程序需要的其他头文件

//#include <string>
#include <fstream>
//#include <stack>
#include <vector>

#define WND_WIDTH   750
#define WND_HEIGHT  550
#define BLACK_B     0X00000000
#define WHITE_B     0X00ffffff
#define RED_B       0X000000ff
#define GREEN_B     0X00008000
#define BLUE_B      0X00ff0000
