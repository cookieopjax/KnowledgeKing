//designed by 宋振宇
#include <stdio.h>
#include <windows.h> // For console interaction，含有讀取寫入位置的函數
#include <pthread.h> // For multithreading
#include <time.h> // For Sleep()
#include <conio.h> // For getch()
#include <string.h> // For strlen()
#include "dogkiller.h"
#include "feature.h"

////////////////////////////////////////////////////////////////
//                         初始化
////////////////////////////////////////////////////////////////

int msLeft;
char pressedKey;
char* countdownString = "倒數計時: ";
int countdownStringLength;
int y;
HANDLE hStdout;

////////////////////////////////////////////////////////////////
//                       內部必須函數
////////////////////////////////////////////////////////////////

// 取得游標座標
COORD GetConsoleCursorPosition(HANDLE hConsoleOutput) {
	CONSOLE_SCREEN_BUFFER_INFO cbsi;
	if (GetConsoleScreenBufferInfo(hConsoleOutput, &cbsi)) {
		return cbsi.dwCursorPosition;
	} else {
		// The function failed. Call GetLastError() for details.
		return (COORD) { 0, 0 };
	}
}

// 清空輸入緩衝區
void clearKeyBuf(void) {
	while(kbhit()) getch();
}

// Swap
void swap(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

////////////////////////////////////////////////////////////////
//                          單人模式
////////////////////////////////////////////////////////////////

// 時間倒數 (子線程)
void* timer(void* ptr) {
	printf("%s", countdownString);
	//y = GetConsoleCursorPosition(hStdout).Y;
	//COORD coordCountdown = { countdownStringLength, y - 1 };

	//SetConsoleCursorPosition(hStdout, coordCountdown);
	while (msLeft > 0) {
		printf("%.1f", msLeft/1000.0);
		Sleep(100);
		if (pressedKey == '\0') {
			if (msLeft > 10000)
				printf("\b");
			else if (msLeft == 10000) {
				printf("\b");
				printf(" ");
				printf("\b");
			}
			printf("\b\b\b");
			msLeft -= 100;
		} else
			break;
	}
	if (pressedKey == '\0')
		printf("0.0"); // 倒數結束校正顯示
}

void* silentTimer(void* ptr) {
	while (msLeft > 0) {
		Sleep(100);
		msLeft -= 100;
	}
}

// 取得答案 (主線程)
void getAns(int ms, int showText) {
	pressedKey = '\0';
	msLeft = ms;
	// Init threads
 	pthread_t t1;
	if (showText)
		pthread_create(&t1, NULL, timer, NULL);
	else
		pthread_create(&t1, NULL, silentTimer, NULL);
	while ((pressedKey == '\0' || !showText) && msLeft > 0) {
		for (int key = 1; key < 5; ++key) {
			if (GetAsyncKeyState(key + 0x30) < 0 || GetAsyncKeyState(key + 0x60) < 0) {
				pressedKey = key + 48;
				break;
			}
		}
	}
	Sleep(100);
	clearKeyBuf();
}

////////////////////////////////////////////////////////////////
//                          Utils
////////////////////////////////////////////////////////////////

// 設定終端機顏色
void setColor(int bColor, int fColor) {
	int color = 16 * bColor + fColor;
    SetConsoleTextAttribute(hStdout, color);
}

// 設定終端機標題列
void setTitle(char* title) {
	SetConsoleTitle(title);
}

// 模擬打字動畫顯示
void Say(char* str, int interval) {
	for (int i = 0; str[i] != '\0'; ++i) {
		putchar(str[i]);
		if (str[i] != '\n')
			Sleep(interval);
	}
}

// 亂數產生器
int randRange(int low, int high) {
	return rand() % (high - low + 1) + low;
}

// 機率產生器
int randChoose(int* chances) {
	int r = randRange(0, 100);
	int sum = 0;
	for (int i = 0; sum <= 100; ++i) {
		sum += chances[i];
		if (r <= sum)
			return i;
	}
	return 0;
}

// 不重複機率產生器
void randoms(int start, int end, int *randomTable, int n) {
    int items = end - start + 1;
    int box[items];
    for (int i = 0; i < items; ++i) {
        box[i] = i + start;
    }
    int trash = 0;
    int randomIndex;
    for (int i = 0; i < n; ++i) {
        randomIndex = randRange(trash, items - 1);
        randomTable[i] = box[randomIndex];
        swap(&box[randomIndex], &box[trash]);
        ++trash;
    }
}

// 顯示隱藏游標
void showCursor(int Show) {
	CONSOLE_CURSOR_INFO cursorInfo;
	for (int i = 0; i < 3; ++i) {
		GetConsoleCursorInfo(hStdout, &cursorInfo);
		cursorInfo.bVisible = Show;
		SetConsoleCursorInfo(hStdout, &cursorInfo);
	}
}

int width, height, size, half;
// 取得視窗寬高
void getWindowSize(void) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hStdout, &csbi);
	width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	size = width * height;
	half = width / 2;
}

// 移動游標
void gotoxy(int x, int xAbs, int y, int yAbs) {
	COORD offset;
	if (!xAbs || !yAbs)
		offset = GetConsoleCursorPosition(hStdout);
	if (xAbs)
		offset.X = 0;
	if (yAbs)
		offset.Y = 0;
	SetConsoleCursorPosition(hStdout, (COORD) {offset.X + x, offset.Y + y});
}

// Cls
void cls(void) {
	system("cls");
}

// Move array for rank
void movList(int *arr, int start, int end) {
	for (int i = end; i > start; --i)
		arr[i] = arr[i - 1];
}

// Move array for bindSortDouble
void movListDouble(double *arr, int start, int end) {
	for (int i = end; i > start; --i)
		arr[i] = arr[i - 1];
}

// Bind data insertion sort
void bindSortDouble(double *arr, int *rank, int len) {
	double tmparr[len];
	double tmparrElement;
	int tmprankElement;
	for (int i = len - 1; i >= 0; --i)
        rank[i] = i;
	memcpy(tmparr, arr, sizeof(double) * len);
	for (int i = 1; i < len; ++i) {
		for (int j = 0; j < i; ++j) {
			if (tmparr[i] > tmparr[j]) {
				// Arr insert
				tmparrElement = tmparr[i];
				movListDouble(tmparr, j, i);
				tmparr[j] = tmparrElement;
				// Rank insert
				tmprankElement = rank[i];
				movList(rank, j, i);
				rank[j] = tmprankElement;
				break;
			}
		}
	}
}

// Print multiple characters
void printX(char c, int n) {
	for (int i = 0; i < n; ++i) {
		putchar(c);
	}
}

// Goto mid position
void gotoMid(int len) {
	getWindowSize();
	printX(' ', half - len/2 - 1);
}

// Border
void Border(char c) {
	getWindowSize();
	gotoxy(0, 1, 0, 1);
	printX(c, width);
}

// 顯示燈泡 (Jimmy Lin撰寫; Dogkiller87修訂)
int lamp(void) {
    char fpath[] = "resources\\lamp.txt";
    FILE *f = fopen(fpath, "r");
    if (f == NULL) {
        puts("檔案不存在!");
		getch();
        return - 1;
    }
    char line[100];
    while ( fgets(line, 100, f) ) {
		int i = 0;
		while (line[i] != '\0') {
			if (line[i] == '*') {
				setColor(0xE, 0);
			} else if (line[i] == '\n') {
				setColor(0, 7);
				printf("\n");
			} else if (line[i] == '#') {
				setColor(8, 0);
			} else {
				putchar(line[i]);
				++i;
				continue;
			}
			putchar(' ');
			++i;
		}
    }
	setColor(0, 7);
	return 0;
}

////////////////////////////////////////////////////////////////
//                         Main Init
////////////////////////////////////////////////////////////////

// Init Handle
int Init(void) {
	// Init console handle
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE) {
		printf("GetStdHandle failed with %ld\n", GetLastError());
		return -1;
	}
	srand(time(NULL));
	countdownStringLength = strlen(countdownString);
	return 0;
}

////////////////////////////////////////////////////////////////
//                         Animation
////////////////////////////////////////////////////////////////
void fivetoone(){//開始遊戲時的倒數計時顯示 by李秉泓
	printf("            〔 ");
    for(int i=5;i>0;i--){
        printf("%d 〕",i);
        Sleep(500);
		printf("\b\b\b\b");
    }
    cls();
}
