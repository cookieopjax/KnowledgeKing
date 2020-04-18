#include <stdio.h>
#include <windows.h>
#include <string.h>
#include "dogkiller.h"

int width, height, size, half;
int flip = 1;
CHAR_INFO *bufText, *bufCls;
HANDLE h[3];
CONSOLE_SCREEN_BUFFER_INFO csbi;
COORD bufStartPoint = {0, 0};
COORD bufLength;
SMALL_RECT rectangle = {0, 0};

// 計算顏色
int Color(int bColor, int fColor) {
	return 16 * bColor + fColor;
}

// 填滿指定緩衝區
void fillBuf(char c, int bufColor, CHAR_INFO *buf) {
	int index;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
			index = width * i + j;
            buf[index].Char.AsciiChar = c;
		    buf[index].Attributes = bufColor;
        }
    }
}

// 切換至頁面
void switchPage(int pageID) {
    if (!SetConsoleActiveScreenBuffer(h[pageID])) {
		printf("SetConsoleActiveScreenBuffer failed - (%ld)\n", GetLastError());
	}
}

// 初始化相關項目
int InitBuf(void) {
	// HANDLE h[3];
	h[0] = GetStdHandle(STD_OUTPUT_HANDLE);
	h[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	h[2] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (h[1] == INVALID_HANDLE_VALUE || h[2] == INVALID_HANDLE_VALUE) {
		printf("CreateConsoleScreenBuffer failed - (%ld)\n", GetLastError());
		return 1;
	}

	// CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(h[0], &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	size = width * height;
	half = width / 2;

	// COORD bufStartPoint = {0, 0};
	// COORD bufSize = {width, height};
	bufLength.X = width;
	bufLength.Y = height;
	SetConsoleScreenBufferSize(h[1], bufLength);
	SetConsoleScreenBufferSize(h[2], bufLength);

	// SMALL_RECT rectangle = {0, 0, width, height};
	rectangle.Right = width;
	rectangle.Bottom = height;
	SetConsoleWindowInfo(h[1], 1, &rectangle);
	SetConsoleWindowInfo(h[2], 1, &rectangle);

	//CHAR_INFO bufText[width * height];
	bufText = (CHAR_INFO *) calloc(size, sizeof(CHAR_INFO));
	bufCls = (CHAR_INFO *) calloc(size, sizeof(CHAR_INFO));
	return 0;
}

// 將緩衝區寫入頁面並切換至該頁面
void flush(void) {
	flip = 3 - flip;
	WriteConsoleOutputA(h[flip], bufText, bufLength, bufStartPoint, &rectangle);
	if (!SetConsoleActiveScreenBuffer(h[flip])) {
		printf("SetConsoleActiveScreenBuffer failed - (%ld)\n", GetLastError());
	}
}

// 將字串寫入緩衝區
void addText(int x, int y, char *str, int bufColor) {
	if (bufColor == 0)
		bufColor = Color(0, 7);
	int index;
	for (int i = 0; str[i] != '\0'; ++i) {
		if (str[i] == '\n') {
			++y;
			x = 0;
			continue;
		}
		index = width * y + x;
		bufText[index].Char.AsciiChar = str[i];
		bufText[index].Attributes = bufColor;
		++x;
	}
}

// addText置中
void addMid(int y, char *str, int bufColor) {
	int len = strlen(str);
	addText(half - len/2 - 1, y, str, bufColor);
}

// 填滿切換緩衝區
void fillBackground(char c, int bufColor) {
	fillBuf(c, bufColor, bufText);
}

// 填滿邊界
void fillBorder(char* c, int bufColor) {
	for (int i = 0 ; i < width; ++i) {
		addText(i, 0, c, bufColor);
		addText(i, height - 1, c, bufColor);
	}
	for (int i = 1; i < height; ++i) {
		addText(0, i, c, bufColor);
		addText(width - 1, i, c, bufColor);
	}
}

// 移動游標
void gotoxy(int x, int y) {
	SetConsoleCursorPosition(h[flip], (COORD) {x, y});
}

// 切換至初始頁面
void back(void) {
    switchPage(0);
}

// 清除頁面
void cls(void) {
	memcpy(bufText, bufCls, size * sizeof(CHAR_INFO));
    fillBuf(0, 0, bufText);
	for (int i = 0; i < 3; ++i) {
		SetConsoleCursorPosition(h[i], (COORD) {0, 0});
	}
	WriteConsoleOutputA(h[0], bufCls, bufLength, bufStartPoint, &rectangle);
	SetConsoleActiveScreenBuffer(h[0]);
}