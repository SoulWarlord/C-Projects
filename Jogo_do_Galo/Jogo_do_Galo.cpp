// Jogo_do_Galo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <locale.h>

//1-Requerimentos iniciais
char player_name[2][30];//array 2 dimensões para os nomes
int chance;//probabilidade de jogar

int box;//caixa em que o jogador se encontra
char play[3][3];// valores do jogador enquanto joga
int turns; // contar as probabilidades de jogar

int move(char play[3][3], int box, int player, int key);//método de mover jogador
void playinbox(char play[3][3], char XO, int box);
void movetobox(int box);
void showframe(int posx, int posy);
void showbox(int ch, int box);
int checkforwin(char[3][3]);
int boxesleft(char a[3][3]);
void gotoxy(short x, short y);


int main()
{
	//setlocale(LC_ALL, "Portuguese");
	showframe(35, 10);
	gotoxy(32, 18);
	system("pause");
	
	return 0;
}

void gotoxy(short x, short y)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };

	SetConsoleCursorPosition(hStdout, position);
}
void showframe(int posx, int posy){

	int hr = 196;// ASCII characters
	int vr = 179;
	int crossbr = 197;
	int x = posx;
	int y = posy;
	int x1 = 35;
	int y1 = 5;
	int i, j;

	gotoxy(x1, y1);
	cprintf("JOGO DO GALO");
	gotoxy(x1, y1+1);
	
	for (i = 0; i <= 11; i++)
		cprintf("%c", 42);

	for (i = 0; i < 2; i++){
		for (j = 0; j <= 11; j++){
			gotoxy(x, y);
			printf("%c", hr);
			x++;
		}
		x = posx;
		y += 2; // y = y + 2
	}
	x = posx + 4;
	y = posy - 1;

	for (i = 0; i < 2; i++){
		for (j = 0; j <= 4; j++){
			gotoxy(x, y);
			printf("%c", vr);
			y++;
		}
		x += 3;
		y = posy - 1;
	}

		x = posx + 4;
		y = posy;
		gotoxy(x, y);
		printf("%c", crossbr);

		x = posx + 7;
		y = posy;
		gotoxy(x, y);
		printf("%c", crossbr);

		x = posx + 4;
		y = posy + 2;
		gotoxy(x, y);
		printf("%c", crossbr);

		x = posx + 7;
		y = posy+ 2;
		gotoxy(x, y);
		printf("%c", crossbr);
}

