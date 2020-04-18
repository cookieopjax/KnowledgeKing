//designed by 宋振宇
#pragma once
#include <windows.h>

////////////////////////////////////////////////////////////////
//                          dogkiller
////////////////////////////////////////////////////////////////

// 此為時間及倒數的結構
extern int msLeft;
extern char pressedKey; //輸入的按鍵
extern int width;
extern int height;
extern HANDLE hStdout;
extern char* countdownString;

int Init(void);
void clearKeyBuf(void);
void getAns(int ms, int showText);
void setColor(int bColor, int fColor);
void setTitle(char* title);
void Say(char* str, int interval);
int randRange(int low, int high);
int randChoose(int* chances);
void randoms(int start, int end, int *randomTable, int n);
void showCursor(int Show);
void gotoxy(int x, int xAbs, int y, int yAbs);
void cls(void);
void bindSortDouble(double *arr, int *rank, int len);
void gotoMid(int len);
int lamp(void);
void fivetoone();