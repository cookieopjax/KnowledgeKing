#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "feature.h"
#include "dogkiller.h"
#define MAXPLAYER 10
#define quesLine 120
#define quesLong 200

char playerName[MAXPLAYER + 1][128];
double playerScore[MAXPLAYER + 1];
int rank[MAXPLAYER + 1];
int choose_level[3];
int currentPlayerId;
int playerNum;
double difficultyMultiplier = 1.0;

char question[quesLine][5][quesLong];//題目及第一二三四選項
int r_ans[quesLine], counts=0;

void loadQues(void) {
    char fpath[] = "ques.txt"; //開啟題目庫
    FILE *f = fopen(fpath, "r");
    while(counts<110){//將題目及選項讀入陣列
        fscanf(f, "%*d;%[^;];%[^;];%[^;];%[^;];%[^;];%d",
            question[counts][0],
            question[counts][1],
            question[counts][2],
            question[counts][3],
            question[counts][4],
            &r_ans[counts]
        );
        counts++;
    }
    fclose(f);
}

void loadRank(void) {
    playerNum = 1;
    char fpath[] = "player.ini";
    FILE *f = fopen(fpath, "r");
    while (fscanf(f, "%[^;];%lf\n", playerName[playerNum], &playerScore[playerNum]) != EOF)
        ++playerNum;
    fclose(f);
}

void writeRank(void) {
    bindSortDouble(playerScore, rank, playerNum);
    char fpath[] = "player.ini";
    FILE *f = fopen(fpath, "w");
    for (int i = 0; i < (playerNum < MAXPLAYER + 1 ? playerNum : MAXPLAYER + 1); ++i)
        if (playerName[rank[i]][0] != '\0')
            fprintf(f, "%s;%.1lf\n", playerName[rank[i]], playerScore[rank[i]]);
    fclose(f);
}

void showRank(void) {
    int action;
    char target;
    char confirm;

    while (1) {
        cls();
        loadRank();
        setColor(0, 7);
        printf("       知識王排行榜\n\n");
        int max = playerNum < MAXPLAYER + 1 ? playerNum : MAXPLAYER + 1;
        setColor(0, 0xA);
        for (int i = 1; i < max; ++i) {
            setColor(0, 0xF - i + 1);
            printf("       * 第%i名:%s\t\t\t分數:%.1lf\n", i, playerName[i], playerScore[i]);
        }
        setColor(0, 0xE);
        printf(
            "\n       1.移除玩家\n"
            "       0.返回菜單\n"
            "       請選擇 : "
        );

    retry:
        action = getch();
        if (action == '1') {
            putchar(action);
            setColor(0, 0xA);
            printf("\n       請輸入排名 (0取消): ");
            scanf(" %c", &target);
            if (target == '0') {
                continue;
            } else if (target < '1' || target > playerNum - 1 + 48) {
                setColor(0, 0xC);
                printf("       無此玩家\n");
                Sleep(800);
            } else {
                setColor(0, 0xC);
                clearKeyBuf();
                printf("       您確定嗎? (y/n): ");
                scanf(" %c", &confirm);
                if (confirm == 'Y' || confirm == 'y') {
                    playerName[target - 48][0] = '\0';
                    writeRank();
                }
            }
        }
        else if (action == '0')
            break;
        else
            goto retry;
    }
}

void NewGame() {//menu選擇1則進入此
    loadRank();
    writeRank();
    playerName[0][0] = '\0';
    playerScore[0] = 0.0;
    while (1) {
        cls();
        printf("\n       請輸入暱稱 (0回上一頁): ");
        clearKeyBuf();
        scanf("\n%[^\n]", playerName[0]);
        if (playerName[0][0] == '\0')
            printf("       ERROR\n");
        else
            break;
    }
    if (playerName[0][0] == '0' && playerName[0][1] == '\0') {
        playerName[0][0] = '\0';
        return;
    }
    currentPlayerId = 0;
    for (int i = 1; i < playerNum; ++i) {
        if (strcmp(playerName[0], playerName[i]) == 0) {
            currentPlayerId = i;
            break;
        }
    }
    //printf("       你的暱稱是: ");
    //puts(playerName[0]);//輸出剛才輸入的暱稱
    if (currentPlayerId == 0) {
        printf("\n       歡迎新玩家加入，%s!", playerName[0]);
    } else {
        printf("\n       歡迎回來 %s! 您目前穩居第%i名!", playerName[0], currentPlayerId);
    }
    Sleep(1500);

    while (1) {
        cls();
        printf("\n       請選擇模式:\n");
        setColor(0, 0xE);printf("       1.單人積分模式\n");setColor(0, 0x7);
        setColor(0, 0xE);printf("       2.來不及做QQ\n");setColor(0, 0x7);
        char mode;
        printf("       請選擇: ");
        scanf(" %c", &mode);
        if (mode == '1')
            break;
        else if (mode == '2') {
            printf("       是在哈囉?\n");
            Sleep(1000);
        }
        else {
            printf("       ERROR\n");
            Sleep(500);
        }
    }
    personal_mode();
}

void personal_mode(){//單人積分模式
    char select_hard;//選擇下列四種難度
    while (1) {
        cls();
        clearKeyBuf();
        printf("\n       請選擇難度 (分數加成):\n");
        setColor(0, 0xE);
        printf("       1:簡單 0.8\n"//1
               "       2:中等 1.3\n"//2
               "       3:困難 1.6\n"//3
               "       4:極限 2.0\n");//4
        setColor(0, 0x7);
        printf("       請選擇: ");
        scanf(" %c", &select_hard);
        if (select_hard != '1' && select_hard != '2' && select_hard != '3' && select_hard != '4') {
            printf("\n       ERROR\n");
            Sleep(700);
        } else {
            select_hard -= 48;
            break;
        }
    }

    showCursor(0);
    cls();
    printf("\n       ");
    Say("單人積分模式即將開始\n", 100);
    fivetoone();//倒數54321的動畫
    //題庫分配為10-50-50
    select_level(select_hard);//制定轉換各難度題目數量

    int ques[5];
    int quesTmp[5];
    int table[5];
    int quesNum = 0;

    if (choose_level[1] > 0) {
        randoms(0, 9, table, 3);
        for (int i = 0; i < choose_level[1]; ++i, ++quesNum)
            quesTmp[quesNum] = table[i];
    }
    if (choose_level[2] > 0) {
        randoms(10, 59, table, 3);
        for (int i = 0; i < choose_level[2]; ++i, ++quesNum)
            quesTmp[quesNum] = table[i];
    }
    if (choose_level[3] > 0) {
        randoms(60, 109, table, 5);
        for (int i = 0; i < choose_level[3]; ++i, ++quesNum)
            quesTmp[quesNum] = table[i];
    }
    randoms(0, 4, table, 5);
    for (int i = 0; i < 5; ++i)
        ques[i] = quesTmp[table[i]];

    playerScore[0] = 0;
    for(int i = 0; i < 5; i++) { //作答
        clearKeyBuf();
        printf("\n%i. %s\n", i + 1, question[ques[i]][0]);
        getAns(2500, 0);
        printf("%s\n%s\n%s\n%s\t\t\t\t\t分數:%.1lf\n\n",
            question[ques[i]][1],
            question[ques[i]][2],
            question[ques[i]][3],
            question[ques[i]][4],
            playerScore[0]
        );
        if (pressedKey == '\0')
            getAns(15000, 1);
        else {
            msLeft = 15000;
            printf("%s%.1lf", countdownString, msLeft/1000.0);
        }
        printf("\n");
        if ( msLeft > 0 && (pressedKey - 48 == r_ans[ques[i]]) ) {//答對時，計算分數
            playerScore[0] += msLeft/100*difficultyMultiplier;//得到的分數即為時間/10
            gotoxy(0, 1, -3, 0);
            printf("%s\t\t\t\t\t分數:%.1lf\n\n\n",
                question[ques[i]][4],
                playerScore[0]
            );
            setColor(0, 0xA);
            printf("答對! (O) ");
            setColor(0, 7);
        } else {
            setColor(0, 0xC);
            printf("答錯! (X) ");
            setColor(0, 7);
        }

        printf("正解: %s", question[ ques[i] ][ r_ans[ ques[i] ] ]);
        showCursor(1);
        if (i < 4)
            Sleep(1500);
        showCursor(0);
        printf("\n\n----------------------------------------------------------------");
        if (i < 4)
            printf("\n");
    }
    showCursor(1);

    setColor(0, 0xE);
    printf("\n\n〈你這次得到了%.1f分〉", playerScore[0]);
    if (currentPlayerId == 0 || (currentPlayerId > 0 && playerScore[0] > playerScore[currentPlayerId])) {
        setColor(0, 0x9);
        printf("刷新個人最新紀錄! ");
    }
    if (playerScore[currentPlayerId] >= playerScore[rank[0]]) {
        setColor(0, 0xA);
        printf("刷新遊戲全場最新紀錄!!");
    }
    setColor(0, 7);
    printf("\n\n按任意鍵回到主選單");
    getch();
    if (currentPlayerId > 0) {
        if (playerScore[0] < playerScore[currentPlayerId])
            playerScore[0] = playerScore[currentPlayerId];
        playerName[currentPlayerId][0] = '\0';
    }
    writeRank();
}

void select_level(int level){
    if(level==1){//簡單
        choose_level[1]=3; choose_level[2]=2; choose_level[3]=0; difficultyMultiplier=0.8;}//此處代表簡單三題、中等兩題、難三題，以下同理
    else if(level==2){//中等
        choose_level[1]=1; choose_level[2]=3; choose_level[3]=1; difficultyMultiplier=1.3;}
    else if(level==3){//難
        choose_level[1]=0; choose_level[2]=2; choose_level[3]=3; difficultyMultiplier=1.6;}
    else if(level==4){//極限
        choose_level[1]=0; choose_level[2]=0; choose_level[3]=5; difficultyMultiplier=2.0;}
}

// 格式化顯示開發名單
void producer(void) {
    cls();
    char fpath[] = "resources\\devloper_info.txt";
    FILE *f = fopen(fpath, "r");
    if (f == NULL) {
        puts("檔案不存在!");
        return;
    }
    setColor(0, 0xF);
    printf("       建議至HackMD瀏覽完整版本\n\n");
    char line[100];
    while ( fgets(line, 100, f) ) {
		int i = 0;
		while (line[i] != '\0') {
            // Text
			if (line[i] == '+') {
				setColor(0, 0xE);
			} else if (line[i] == '\n') {
				setColor(0, 7);
				printf("\n");
            // Name
			} else if (line[i] == '#') {
				setColor(0, 0xB);
            // File
            } else if (line[i] == '$') {
                setColor(0, 0xC);
			} else {
				putchar(line[i]);
			}
			++i;
		}
    }
	setColor(0, 0xF);
    printf("\n\n       按任意鍵回到主選單");
    getch();
}