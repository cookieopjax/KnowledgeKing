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
    int menu;//�̤j�����
    Init();//���o����cmd���v��
	setTitle("���Ѥ�");
    loadQues();
    printf("\n");

    if (lamp() == -1) //�e�X�i�J�C���ɪ��O�w�A�bdogkiller.c�������禡
        return - 1;
    printf("\n\n");
    setColor(0, 0xF);printf("      �����N��i�J���\n");
    getch();

	while(1){//�{���j�[�cby�L�m��,���N�ץ�by���êl
        cls();
        printf("\n");
        setColor(0, 0xE);
        printf(
            "      ��������������������������������\n"
            "      ��  ��  ��  ��  ��\n"
            "      ��������������������������������\n");
        setColor(0, 0x7);
        printf(
            "�ССССССССССССС�\n\n"
            "       1.�}�l�C��\n"
            "       2.�d�ݱƦ�]\n"
            "       3.��ܻs�@�̦W��\n"
            "       4.�]�w\n"
            "       0.�����C��\n\n"
            "�ССССССССССССС�\n"
            "       �п�J : ");
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
                setColor(0, 0xB);printf("�ڥ��S�o�تF��B��\n\n");setColor(0, 0x7);
                printf("�����N��^��D���");
                getch();
                continue;
            case 0:
                cls();
                printf( "          ___     ___      ___  \n"
                        "         | __|   | _ |    | __| \n"
                        "         | _|   | (_) |   | _|  \n"
                        "         |___|   |___|    |_|     " ); // �Ϥ�
                Sleep(1500);
                break;
            default:
                goto pass;
        }
        break;
	}
	return 0;
}
