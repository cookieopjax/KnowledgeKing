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

char question[quesLine][5][quesLong];//�D�ؤβĤ@�G�T�|�ﶵ
int r_ans[quesLine], counts=0;

void loadQues(void) {
    char fpath[] = "ques.txt"; //�}���D�خw
    FILE *f = fopen(fpath, "r");
    while(counts<110){//�N�D�ؤοﶵŪ�J�}�C
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
        printf("       ���Ѥ��Ʀ�]\n\n");
        int max = playerNum < MAXPLAYER + 1 ? playerNum : MAXPLAYER + 1;
        setColor(0, 0xA);
        for (int i = 1; i < max; ++i) {
            setColor(0, 0xF - i + 1);
            printf("       * ��%i�W:%s\t\t\t����:%.1lf\n", i, playerName[i], playerScore[i]);
        }
        setColor(0, 0xE);
        printf(
            "\n       1.�������a\n"
            "       0.��^���\n"
            "       �п�� : "
        );

    retry:
        action = getch();
        if (action == '1') {
            putchar(action);
            setColor(0, 0xA);
            printf("\n       �п�J�ƦW (0����): ");
            scanf(" %c", &target);
            if (target == '0') {
                continue;
            } else if (target < '1' || target > playerNum - 1 + 48) {
                setColor(0, 0xC);
                printf("       �L�����a\n");
                Sleep(800);
            } else {
                setColor(0, 0xC);
                clearKeyBuf();
                printf("       �z�T�w��? (y/n): ");
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

void NewGame() {//menu���1�h�i�J��
    loadRank();
    writeRank();
    playerName[0][0] = '\0';
    playerScore[0] = 0.0;
    while (1) {
        cls();
        printf("\n       �п�J�ʺ� (0�^�W�@��): ");
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
    //printf("       �A���ʺ٬O: ");
    //puts(playerName[0]);//��X��~��J���ʺ�
    if (currentPlayerId == 0) {
        printf("\n       �w��s���a�[�J�A%s!", playerName[0]);
    } else {
        printf("\n       �w��^�� %s! �z�ثeí�~��%i�W!", playerName[0], currentPlayerId);
    }
    Sleep(1500);

    while (1) {
        cls();
        printf("\n       �п�ܼҦ�:\n");
        setColor(0, 0xE);printf("       1.��H�n���Ҧ�\n");setColor(0, 0x7);
        setColor(0, 0xE);printf("       2.�Ӥ��ΰ�QQ\n");setColor(0, 0x7);
        char mode;
        printf("       �п��: ");
        scanf(" %c", &mode);
        if (mode == '1')
            break;
        else if (mode == '2') {
            printf("       �O�b���o?\n");
            Sleep(1000);
        }
        else {
            printf("       ERROR\n");
            Sleep(500);
        }
    }
    personal_mode();
}

void personal_mode(){//��H�n���Ҧ�
    char select_hard;//��ܤU�C�|������
    while (1) {
        cls();
        clearKeyBuf();
        printf("\n       �п������ (���ƥ[��):\n");
        setColor(0, 0xE);
        printf("       1:²�� 0.8\n"//1
               "       2:���� 1.3\n"//2
               "       3:�x�� 1.6\n"//3
               "       4:���� 2.0\n");//4
        setColor(0, 0x7);
        printf("       �п��: ");
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
    Say("��H�n���Ҧ��Y�N�}�l\n", 100);
    fivetoone();//�˼�54321���ʵe
    //�D�w���t��10-50-50
    select_level(select_hard);//��w�ഫ�U�����D�ؼƶq

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
    for(int i = 0; i < 5; i++) { //�@��
        clearKeyBuf();
        printf("\n%i. %s\n", i + 1, question[ques[i]][0]);
        getAns(2500, 0);
        printf("%s\n%s\n%s\n%s\t\t\t\t\t����:%.1lf\n\n",
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
        if ( msLeft > 0 && (pressedKey - 48 == r_ans[ques[i]]) ) {//����ɡA�p�����
            playerScore[0] += msLeft/100*difficultyMultiplier;//�o�쪺���ƧY���ɶ�/10
            gotoxy(0, 1, -3, 0);
            printf("%s\t\t\t\t\t����:%.1lf\n\n\n",
                question[ques[i]][4],
                playerScore[0]
            );
            setColor(0, 0xA);
            printf("����! (O) ");
            setColor(0, 7);
        } else {
            setColor(0, 0xC);
            printf("����! (X) ");
            setColor(0, 7);
        }

        printf("����: %s", question[ ques[i] ][ r_ans[ ques[i] ] ]);
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
    printf("\n\n�q�A�o���o��F%.1f���r", playerScore[0]);
    if (currentPlayerId == 0 || (currentPlayerId > 0 && playerScore[0] > playerScore[currentPlayerId])) {
        setColor(0, 0x9);
        printf("��s�ӤH�̷s����! ");
    }
    if (playerScore[currentPlayerId] >= playerScore[rank[0]]) {
        setColor(0, 0xA);
        printf("��s�C�������̷s����!!");
    }
    setColor(0, 7);
    printf("\n\n�����N��^��D���");
    getch();
    if (currentPlayerId > 0) {
        if (playerScore[0] < playerScore[currentPlayerId])
            playerScore[0] = playerScore[currentPlayerId];
        playerName[currentPlayerId][0] = '\0';
    }
    writeRank();
}

void select_level(int level){
    if(level==1){//²��
        choose_level[1]=3; choose_level[2]=2; choose_level[3]=0; difficultyMultiplier=0.8;}//���B�N��²��T�D�B�������D�B���T�D�A�H�U�P�z
    else if(level==2){//����
        choose_level[1]=1; choose_level[2]=3; choose_level[3]=1; difficultyMultiplier=1.3;}
    else if(level==3){//��
        choose_level[1]=0; choose_level[2]=2; choose_level[3]=3; difficultyMultiplier=1.6;}
    else if(level==4){//����
        choose_level[1]=0; choose_level[2]=0; choose_level[3]=5; difficultyMultiplier=2.0;}
}

// �榡����ܶ}�o�W��
void producer(void) {
    cls();
    char fpath[] = "resources\\devloper_info.txt";
    FILE *f = fopen(fpath, "r");
    if (f == NULL) {
        puts("�ɮפ��s�b!");
        return;
    }
    setColor(0, 0xF);
    printf("       ��ĳ��HackMD�s�����㪩��\n\n");
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
    printf("\n\n       �����N��^��D���");
    getch();
}