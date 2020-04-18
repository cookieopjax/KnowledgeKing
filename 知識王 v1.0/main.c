#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <conio.h> // For getch()
#include <mmsystem.h> // For PlaySound()
#include <sys/stat.h> // For filesystem stat()
#include "libs\dogkiller.h"
#include "libs\feature.h"

int fileExist (char *filename) {
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

int main(void) {
    char BGM[] = "BGM.wav";
    if (fileExist(BGM))
        PlaySound(BGM, NULL, SND_ASYNC | SND_LOOP);
    int menu;//最大的選單
    Init();//取得控制cmd的權限
	setTitle("知識王");
    loadQues();
    printf("\n");

    if (lamp() == -1) //畫出進入遊戲時的燈泡，在dogkiller.c中有此函式
        return - 1;
    printf("\n\n");
    setColor(0, 0xF);printf("      按任意鍵進入選單\n");
    getch();

	while(1){//程式大架構by林峻億,美術修正by李秉泓
        cls();
        printf("\n");
        setColor(0, 0xE);
        printf(
            "      ╒══════════════╕\n"
            "      ║  知  識  王  ║\n"
            "      ╚══════════════╝\n");
        setColor(0, 0x7);
        printf(
            "－－－－－－－－－－－－－－\n\n"
            "       1.開始遊戲\n"
            "       2.查看排行榜\n"
            "       3.顯示製作者名單\n"
            "       4.設定\n"
            "       0.結束遊戲\n\n"
            "－－－－－－－－－－－－－－\n"
            "       請輸入 : ");
    pass:
        clearKeyBuf();
        menu = getch() - 48;
        switch(menu){
            case 1:
                NewGame();
                continue;
            case 2:
                showRank();
                continue;
            case 3:
                producer();
                continue;
            case 4:
                setColor(0, 0xB);printf("根本沒這種東西冰鳥\n\n");setColor(0, 0x7);
                printf("按任意鍵回到主選單");
                getch();
                continue;
            case 0:
                cls();
                printf( "          ___     ___      ___  \n"
                        "         | __|   | _ |    | __| \n"
                        "         | _|   | (_) |   | _|  \n"
                        "         |___|   |___|    |_|     " ); // 圖片
                Sleep(1500);
                break;
            default:
                goto pass;
        }
        break;
	}
	return 0;
}
