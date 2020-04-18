//designed by 宋振宇
#pragma once
#include <windows.h>

////////////////////////////////////////////////////////////////
//                          dogkiller
////////////////////////////////////////////////////////////////

// 此為時間及倒數的結構
extern int msLeft;
extern char pressedKey; //輸入的按鍵

void getAns(int ms);
void setColor(int bColor, int fColor);
void setTitle(char* title);
void Say(char* str, int interval);
int randChoose(int* chances);
void showCursor(int Show);

////////////////////////////////////////////////////////////////
//                          dogkillerBuf
////////////////////////////////////////////////////////////////

extern int width;
extern int height;
extern HANDLE h[];
extern CHAR_INFO *bufText;
int Color(int bColor, int fColor);
int InitBuf(void);
void addText(int x, int y, char *str, int bufColor);
void addMid(int y, char *str, int bufColor);
void flush(void);
void fillBackground(char c, int bufColor);
void fillBorder(char* c, int bufColor);
void switchPage(int pageID);
void gotoxy(int x, int y);
void back(void);
void cls(void);