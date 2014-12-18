// TG1_JA.cpp : Defines the entry point for the console application.
//

/*	Título: Jogo de Aventura - Trabalho de Grupo 1
	Autores: André Lascas & Sabino Corrêa
	Versão: 12/12/2014
*/


#include "stdafx.h"
#include "stdlib.h"
#include "locale.h"
#include "string.h"
#include "stdio.h"
#include "time.h"
#include "iostream"
#include "fstream"
#include <sstream>
#include <windows.h>

#define MAX_NAME 40
#define MAX_HEALTH 100
#define MAX_HEALTH_SUPER 1000
#define INITIAL_ROOM 0
#define BOSS_ROOM 13
#define MAX_ROOM_NAME 50
#define MAX_DESC 500
#define TREASURE 0
#define PROMPT_STR 10
#define MAX_ROOM_CELS 14
#define MAX_LINE 100

//char mainhero[7] = { 'B', 'u', 'f', 'e', 't', 'a', '\0' };
char main_hero[] = "Bufeta";
char status_cmd[] = "status";
char loot_cmd[] = "loot";
char end_cmd[] = "end";
bool endgame;
bool death = false;
bool super;
FILE *map;
char prompt[PROMPT_STR];
char option[PROMPT_STR];
char color[] = "COLOR ";
char option_color[]= "";
char south[] = "s";
char north[] = "n";
char east[] = "e";
char west[] = "o";
int nRooms;
int move = 0;
int boss_move = 0;



struct Player{
	char name[MAX_NAME];
	int health;
	int room;
	int treasure;
};

struct Location{
	int north;
	int south;
	int east;
	int west;
	int treasure;
	char description[MAX_DESC];
	
};

struct BossMonster{
	char name[MAX_NAME];
	int health;
	int room;
	bool dead;
};

void PlayerStatus(Player *pPlayer,BossMonster *pBoss, bool super){
	printf("Nome: %s %s\n", pPlayer->name, main_hero);
	printf("Vida: %d\n", pPlayer->health);
	printf("Sala: %d\n", pPlayer->room);
	if (pPlayer->treasure == 0)
		printf("Tesouro: Não encontrado!\n\n");
	else
		printf("Tesouro: Corneta dos 7 Ventos\n\n");
	if (super == true){
		if (pPlayer->treasure == 0)
			printf("Boss:%s - Sala: %d", pBoss->name, BOSS_ROOM);
	}
	else{
		if (pPlayer->treasure == 1)
			printf("Boss:%s - Sala: %d", pBoss->room);
	}
}

void MainHistory(){
	int i;
	for (i = 0; i < 55; i++){
		if (i < 54)
			printf("#");
		else
			printf("#\n");
}
	printf("#       Bufeta e o Reino Perdido do Vento Tóxico      #\n");
	for (i = 0; i < 55; i++){
		if (i < 54)
			printf("#");
		else
			printf("#\n");
	}
	printf("\n");
	printf("Há muito tempo atrás existia um vasto reino chamado Brisa do Ar Puro,\n"
		"onde todos viviam em harmonia e prosperidade. \n"
		"Certo dia, um feiticeiro das trevas chamado QualquerCoisaAgoraNãoMeLembro\n"
		"invadiu o reino e através da sua grande magia negra tentou transformar todos em estátuas de pó tóxico...\n"
		"\n"
		"Mas um guerreiro de seu nome Bufeta fez frente de forma heróica a esta ameaça... \n"
		"Porém o poder do feiticeiro era tão... tão... bem deveras repugnante\n"
		"ao ponto do nosso herói ser derrotado facilmente... :'(\n"
		"\n"
		"Muitos anos mais tarde, um jovem rapaz que vivia na aldeia Aldeia dos Cheiros\n"
		"soube que ele era o filho do herói Bufeta e decidiu começar a sua aventura.\n"
		"O seu objectivo era vingar o seu pai e nisso redimir\n"
		"o nome da sua familia Bufeta e regressar à sua aldeia natal.\n"
		"\n"
		"Para tal primeiro terá de ficar forte e ir em busca do único artefacto\n"
		"capaz de matar o QualquerCoisaAgoraNãoMeLembro, a Corneta dos 7 Ventos!\n"
		"Que comece a sua grande aventura!!!\n\n");
	
}

void Continue(){
	printf("Pressiona Enter para continuar\n");
	while (getchar() != '\n');
}


void SaveGame(Player *pPlayer, BossMonster *pBoss, Location cel[]){
	FILE *savegame;
	savegame = fopen("save.txt", "w");

	fprintf(savegame, "%s\n", pPlayer->name);
	fprintf(savegame, "%d\n", pPlayer->room);
	fprintf(savegame, "%d\n", pPlayer->health);
	fprintf(savegame, "%d\n", pPlayer->treasure);
	fprintf(savegame, "\n");
	fprintf(savegame, "%s\n", pBoss->name);
	fprintf(savegame, "%d\n", pBoss->health);
	fprintf(savegame, "\n");
	fprintf(savegame, "Corneta dos 7 Ventos\n");

	if (pPlayer->treasure == 0)
	{
		for (int i = 0; i < MAX_ROOM_CELS; i++)
		{
			if (cel[i].treasure == 1)
			{
				fprintf(savegame, "%d", i);
			}
		}
	}

	fclose(savegame);
	printf("Jogo salvo com sucesso!\n");
}

void LoadGame(Player *pPlayer, BossMonster *pBoss, Location cel[]){
	FILE *loadGame;
	char data[MAX_LINE];

	loadGame = fopen("save.txt", "r");

	fgets(data, MAX_LINE, loadGame);
	data[strcspn(data, "\n")] = '\0';
	strcpy(pPlayer->name, data);

	fgets(data, MAX_LINE, loadGame);
	pPlayer->room = atoi(data);

	fgets(data, MAX_LINE, loadGame);
	pPlayer->health = atoi(data);

	fgets(data, MAX_LINE, loadGame);
	pPlayer->treasure = atoi(data);

	fgets(data, MAX_LINE, loadGame);
	pBoss->room = atoi(data);

	fgets(data, MAX_LINE, loadGame);
	for (int i = 0; i < MAX_ROOM_CELS; i++)
	{
		cel[i].treasure = 0;
	}

	if (pPlayer->treasure == 0)
	{
		cel[atoi(data)].treasure = 1;
	}

	printf("Voltaste %s %s! Estavas pela última vez na sala %d. Vamos para a vitória!\n", pPlayer->name, main_hero, pPlayer->room);
	system("pause");
}
/*
void LerTxt( Location cel[])
{
	FILE *map;
	char line[MAX_LINE];

	map = fopen("map.txt", "r");

	if (map == NULL)
	{
		printf("O ficheiro não se encontra na directoria\n");
	}
	else
	{
		for (int i = 0; i < MAX_ROOM_CELS; i++)
		{
			fgets(line, MAX_LINE, map);
			cel[i].north = atoi(line);
			fgets(line, MAX_LINE, map);
			cel[i].south = atoi(line);
			fgets(line, MAX_LINE, map);
			cel[i].east = atoi(line);
			fgets(line, MAX_LINE, map);
			cel[i].west = atoi(line);
			fgets(line, MAX_LINE, map);
			cel[i].treasure = atoi(line);
			fgets(line, MAX_LINE, map);
			strcpy(cel[i].description, line);
		}
		fclose(map);
	}
}*/

void FightOrDie(Player *pPlayer, BossMonster *pBoss){

	printf("%s %s: %d Vida\n", pPlayer->name, main_hero, pPlayer->health);
	printf("%s: %d Vida\n", pBoss->name, pBoss->health);

	while (death == false)
	{
		int rand_dmg = rand() % 15+1;
		int player_dmg = rand_dmg;
		int boss_dmg = rand_dmg;
		int rand_num = rand() % 10+1;

		if (rand_num % 2 == 0){
			pBoss->health = pBoss->health - player_dmg;
			printf("Atingiste o teu adversário com sucesso por %d dano! %s tem %d de vida!", player_dmg, pBoss->name, pBoss->health);
			system("pause");
		}
		else{
			pPlayer->health = pPlayer->health - boss_dmg;
			printf("A malvadez é superior! Recebeste %d de dano. Tens %d de vida!", boss_dmg, pPlayer->health);
			system("pause");
		}

		if (pPlayer->health <= 0){
			printf("O %s conseguiu aniquilar-te... O reinado continuará para sempre assombrado! MUAHAHAHAHA!\n\n", pBoss->name);
			death = true;
			endgame = true;
		}
		if (pBoss->health <= 0){
			printf("Muito bem %s %s! Finalmente conseguiste pôr um fim ao malvado %s! És o Herói dos 7 ventos! :D\n\n", pPlayer->name, main_hero, pBoss->name);
			death = true;
	}
	
	}
}

void StartPlayer(Player *pPlayer, bool super){
	printf("Bem-vindo aventureiro! Qual é o teu nome?\n");
	scanf("%s", pPlayer->name);
	pPlayer->room = INITIAL_ROOM;
	pPlayer->treasure = TREASURE;
	if (super == false) {
		pPlayer->health = MAX_HEALTH;
		printf("Que nome estranho O.o...Mas deixa lá isso,a tua aventura começa agora!\n"
			"-Para veres o teu status a qualquer momento escreve 'status'(óbvio!)\n"
			"-Para te movimentares escreve n(norte), s(sul), e(este) ou o(oeste)\n"
			"-Para apanhares o item escreve loot\n"
			"-Para gravares a qualquer altura escreve 'save'\n"
			"Boa sorte!(Bem vais precisar eheh)\n\n");
	}
	else{
		pPlayer->health = MAX_HEALTH_SUPER;
		printf("Que nome estranho O.o...Mas deixa lá isso,a tua aventura começa agora!\n"
			"-Para veres o teu status a qualquer momento escreve 'status'(óbvio!)\n"
			"-Para te movimentares escreve n(norte), s(sul), e(este) ou o(oeste)\n"
			"-Para apanhares o item escreve loot\n"
			"-Para gravares a qualquer altura escreve 'save'\n"
			"Boa sor... Nah tás em modo super-sayan matas tudo! HUEHUE\n\n");
	}
}

void StartBossMonster( BossMonster *pBoss){
	strcpy(pBoss->name, "QualquerCoisaAgoraNãoMeLembro");
	pBoss->health = MAX_HEALTH;
	pBoss->room = 13;

}

void StartLocation( Location cel[], int *pnRooms){
	cel[0].north = 1;
	cel[0].south = -1;
	cel[0].west = -1;
	cel[0].east = -1;
	cel[0].treasure = 0;
	strcpy(cel[0].description, "Estás de frente a um portão gigante, fortemente fortificado.\nPresumes ser a entrada para algum tipo de reino secreto.\n\n");
	
	cel[1].north = 4;
	cel[1].south = 0;
	cel[1].west = 2;
	cel[1].east = 3;
	cel[1].treasure = 0;
	strcpy(cel[1].description, "Entraste no reino. À tua volta encontras um riacho(oeste), a entrada de uma floresta(norte) e uma cabana(este).\n\n");

	cel[2].north = -1;
	cel[2].south = -1;
	cel[2].west = -1;
	cel[2].east = 1;
	cel[2].treasure = 0;
	strcpy(cel[2].description, "Chegaste ao topo do riacho. Aqui tens uma excelente vista do reinado mas sem sinal do teu arqui-enimigo...\n\n");

	cel[3].north = -1;
	cel[3].south = -1;
	cel[3].west = 1;
	cel[3].east = -1;
	cel[3].treasure = 0;
	strcpy(cel[3].description, "Esta cabana dá-te arrepios... Não sabes se é do frio ou de algo que paira no ar... UHHHHH!\n\n");

	cel[4].north = 5;
	cel[4].south = 1;
	cel[4].west = -1;
	cel[4].east = -1;
	cel[4].treasure = 0;
	strcpy(cel[4].description, "Uma floresta sombria cheia de criaturas um pouco horripilantes :O É melhor fugir daqui depressa!\n"
		"Em frente(norte) consegues avistar um pouco de luz.\n\n");

	cel[5].north = 6;
	cel[5].south = 4;
	cel[5].west = -1;
	cel[5].east = -1;
	cel[5].treasure = 0;
	strcpy(cel[5].description, "Finalmente luz! Encontraste o lago das Fadas do Vento. Uma das fadas parece sentir algo em ti...\n"
		"E diz-te a sussurrar que para encontrar a 'luz' tens primeiro de encontrar a 'escuridão'. Só faltava um enigma!\n"
		"Seguimos para norte!\n\n");

	cel[6].north = -1;
	cel[6].south = 5;
	cel[6].west = 7;
	cel[6].east = 8;
	cel[6].treasure = 0;
	strcpy(cel[6].description, "Dás por ti numa vasta planície que mais parece um deserto... Oh maldita sorte!\n"
		"E pelos vistos só podes ir para este ou oeste -_-\n\n");

	cel[7].north = 11;
	cel[7].south = -1;
	cel[7].west = -1;
	cel[7].east = 6;
	cel[7].treasure = 0;
	strcpy(cel[7].description, "A oeste encontras uma grande ponte suspensa de madeira... E aparenta estar em mau estado...\n"
		"Mas temos de encontrar o QualquerCoisaAgoraNãoMeLembro! Para norte!\n\n");

	cel[8].north = -1;
	cel[8].south = -1;
	cel[8].west = 6;
	cel[8].east = 9;
	cel[8].treasure = 0;
	strcpy(cel[8].description, "A este encontras uma grande montanha com uma só entrada. Por cima da entrada lês o aviso:\n"
		"'AQUELES DE CORAÇÃO FRACO NÃO SOBREVIVERÃO À MONTANHA VENTANIA! ESPECIALMENTE SE FORES PARA ESTE!\n\n");

	cel[9].north = -1;
	cel[9].south = 10;
	cel[9].west = 8;
	cel[9].east = -1;
	cel[9].treasure = 0;
	strcpy(cel[9].description, "Andas à horas às voltas pela montanha, cada vez te sentes mais desesperado...\n"
		"Decides tirar à sorte tipo 'Um Dó Li Tá'... Calhou sul, bora!\n\n");

	cel[10].north = 9;
	cel[10].south = -1;
	cel[10].west = -1;
	cel[10].east = -1;
	cel[10].treasure = 1;
	strcpy(cel[10].description, "Existe uma brisa forte assim que te aproximas do final da montanha. Pareces avistar um objecto lendário ao longe...\n\n"
		"É a Corneta dos 7 Ventos!");

	cel[11].north = 12;
	cel[11].south = 7;
	cel[11].west = -1;
	cel[11].east = -1;
	cel[11].treasure = 0;
	strcpy(cel[11].description, "Depois de uma ponta suspensa, encontras agora um abismo cheio de pedras afiadas. Só azar...\n"
		"Mas por sorte (quem diria!) uma fada seguiu-te e com a sua magia ajudou-te na tua aventura :D! O único caminho segue para norte.\n\n");

	cel[12].north = -1;
	cel[12].south = 11;
	cel[12].west = -1;
	cel[12].east = 13;
	cel[12].treasure = 0;
	strcpy(cel[12].description, "Tás diante de uma grande escadaria, a qual liga para o castelo onde o QualquerCoisaAgoraNãoMeLembro supostamente tem reinado...\n"
		"Segue a escada para este.\n\n");

	cel[13].north = -1;
	cel[13].south = -1;
	cel[13].west = 12;
	cel[13].east = -1;
	cel[13].treasure = 0;
	strcpy(cel[13].description, "Por fim entras no castelo amaldiçoado. Sentes um ar pesado e sombrio, onde estará o QualquerCoisaAgoraNãoMeLembro??!\n\n");

	*pnRooms = 14;
}

void ListMap( Location cel[], int *pnRooms){

	int i = 0;
	while (i != *pnRooms)
		printf("Norte: %d", cel[i].north);
		printf("Sul: %d", cel[i].south);
		printf("Este: %d", cel[i].east);
		printf("Oeste: %d", cel[i].west);
		printf("Descrição: %s", cel[i].description);
		i++;
}

void ActionCommand(Player *pPlayer, BossMonster *pBoss, Location cel[], bool super){
	
	if (pPlayer->health == MAX_HEALTH || pPlayer->health == MAX_HEALTH_SUPER){
		printf("%s\n", cel[pPlayer->room].description);
	}
	
	if (cel[pPlayer->room].treasure == 1 && pPlayer->treasure == 0){
		printf("Apanha a corneta! :O\n");
		scanf("%s", prompt);
		if (strcmp(loot_cmd, prompt) == 0){
			pPlayer->treasure = 1;
		}
	}
	if (super == true){
		if (pPlayer->treasure == 0 && pBoss->health == MAX_HEALTH)
			printf("O boss está na sala %d\n", BOSS_ROOM);
		
		if (pPlayer->treasure == 1 && pBoss->health > 0)
			printf("O boss está na sala %d\n", pBoss->room);

		if (pPlayer->health > 0 && pPlayer->health < 1000)
			printf("Saíste vitorioso! Voltemos ao inicio. Estás na sala %d.\n", pPlayer->room);

		if (pPlayer->health <= 0)
			printf("Acabou-se... GAME OVER! Insere 'end'\n");

		if (pPlayer->treasure == 0)
			printf("Estás na sala %d. Para onde queres ir?\n", pPlayer->room);

		if (pPlayer->treasure == 1 && pPlayer->health == MAX_HEALTH_SUPER)
			printf("O feiticeiro está em movimento... Estás na sala %d. Avancemos!\n", pPlayer->room);

	}
	else{

		if (pPlayer->health > 0 && pPlayer->health < 100)
			printf("Saíste vitorioso! Voltemos ao inicio. Estás na sala %d.\n", pPlayer->room);

		if (pPlayer->health <= 0)
			printf("Acabou-se... GAME OVER! Insere 'end'\n");

		if (pPlayer->treasure == 0)
			printf("Estás na sala %d. Para onde queres ir?\n", pPlayer->room);

		if (pPlayer->treasure == 1 && pPlayer->health == MAX_HEALTH)
			printf("O feiticeiro está em movimento... Estás na sala %d. Avancemos!\n", pPlayer->room);
	}


	scanf("%s", prompt);

	if (strcmp(status_cmd, prompt) == 0){
		PlayerStatus(pPlayer, pBoss, super);
	}

	if (strcmp("save", prompt) == 0){
		SaveGame(pPlayer, pBoss, cel);
	}

	if ( strcmp(end_cmd, prompt) == 0){
		endgame = true;
	}
}
void ActionPlayer( Location cel[], Player *pPlayer){
	
	if(strcmp(north, prompt) == 0){
		if (cel[pPlayer->room].north != -1){
			pPlayer->room = cel[pPlayer->room].north;
		}
		else{
			pPlayer->room = pPlayer->room;
			printf("Caminho bloqueado! Escolhe outra direcção...\n\n");
		}
		
	}
	if (strcmp(south, prompt) == 0){
		if (cel[pPlayer->room].south != -1){
			pPlayer->room = cel[pPlayer->room].south;
		}
		else{
			pPlayer->room = pPlayer->room;
			printf("Caminho bloqueado! Escolhe outra direcção...\n\n");
		}
	}
	if (strcmp(east, prompt) == 0){
		if (cel[pPlayer->room].east != -1){
			pPlayer->room = cel[pPlayer->room].east;
		}
		else{
			pPlayer->room = pPlayer->room;
			printf("Caminho bloqueado! Escolhe outra direcção...\n\n");
		}
	}
	if (strcmp(west, prompt) == 0){
		if (cel[pPlayer->room].west != -1){
			pPlayer->room = cel[pPlayer->room].west;
		}
		else{
			pPlayer->room = pPlayer->room;
			printf("Caminho bloqueado! Escolhe outra direcção...\n\n");
		}
	}
}

void ActionMonster(Location cel[], BossMonster *pBoss, bool super){
	int random = rand()%4+1;

	if (random == 1){
		if (cel[pBoss->room].north != -1){
			pBoss->room = cel[pBoss->room].north;
		}
		else{
			pBoss->room = pBoss->room;
		}
	}
	if (random == 2){
		if (cel[pBoss->room].south != -1){
			pBoss->room = cel[pBoss->room].south;
		}
		else{
			pBoss->room = pBoss->room;
		}
	}
	if (random == 3){
		if (cel[pBoss->room].west != -1){
			pBoss->room = cel[pBoss->room].west;
		}
		else{
			pBoss->room = pBoss->room;
		}
	}
	if (random == 4){
		if (cel[pBoss->room].east != -1){
			pBoss->room = cel[pBoss->room].east;
		}
		else{
			pBoss->room = pBoss->room;
		}
	}

}

void StartMenu(){
	printf("Novo jogo - 1\n");
	printf("Carregar jogo - 2\n");
	printf("Opções - 3\n");
	
	scanf("%s", prompt);
}

void Options(){
	printf("Aqui podes costumizar a cor de fundo e da fonte!\n");
	printf("Código de cores:\n"
		"0 = Black\n"
		"1 = Blue\n"
		"2 = Green\n"
		"3 = Aqua\n"
		"4 = Red\n"
		"5 = Purple\n"
		"6 = Yellow\n"
		"7 = White\n"
		"8 = Gray\n"
		"9 = Light Blue\n"
		"A = Light Green\n"
		"B = Light Aqua\n"
		"C = Light Red\n"
		"D = Light Purple\n"
		"E = Light Yellow\n"
		"F = Bright White\n");
		printf("Escolhe um esquema de cores! Ex: '0B' - 0 (cor de fundo) e B(cor da fonte)\n");
		scanf("%s", option);
		strcpy(option_color, color);
		strcat(option_color, option);
		
		system(option_color);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Portuguese");
	system("MODE con cols=150 lines=55");
	
	struct Player player;
	struct Location cel[MAX_ROOM_CELS];
	struct BossMonster boss;
	
	//Modo super-user
	if (argc > 1 && strcmp(argv[1], "su") == 0) {
		super = true;
	}
	else{
		super = false;
	}

	MainHistory();
	Continue();
	StartMenu();
	//ListMap(&cel[INITIAL_ROOM], &nRooms);
	while (strcmp("3", prompt) == 0){
		Options();
		system("cls");
		MainHistory();
		Continue();
		StartMenu();
	}

	if (strcmp("1", prompt) == 0){
		system("cls");
		StartPlayer(&player, super);
		StartLocation(&cel[INITIAL_ROOM], &nRooms);
		StartBossMonster(&boss);
	}
	if (strcmp("2", prompt) == 0){
		system("cls");
		LoadGame(&player, &boss, cel);
		StartLocation(cel, &nRooms);
		StartBossMonster(&boss);
	}

	
	
	while (endgame == false){
		ActionCommand(&player, &boss, cel, super);
		ActionPlayer(cel, &player);
		if (player.treasure == 1){
			ActionMonster(cel, &boss, super);

			if (player.room == boss.room && boss.health == 100){
				printf("Finalmente encontram-se frente a frente! Que comece a batalha pelo Reino Perdido do Vento Tóxico!\n\n");
				FightOrDie(&player, &boss);
				if (death == false){
					printf("Volta ao ponto de partida para terminares a tua jornada!\n\n");
					death = true;
				}

			}
		}
		if (death == true && player.room == 0)
			endgame = true;
	}
	printf("GAME FINISHED! Obrigado por jogares! :D\n");

	system("pause");

	return 0;
}

