#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cmath>
#define N 5
using std::cin;
using std::cout;
using std::endl;

char board[N][N],piece;
int playeri;

void initialBoard(char a[][N]);     //初始化棋盘
void printBoard(char a[][N]);     //打印棋盘
char getpiece(void);
int move(char a[][N], int player, char ch);     //走一步
int judge(char a[][N], int player, char cc);     //判断是否结束
int main(void) {
	initialBoard(board);
	printBoard(board);
	for (int i = 0; i < N * N; ++i) {
		playeri = playeri % 2 + 1;
		while (!isalpha(piece = getpiece()));
		while(!move(board, playeri, piece))piece=getpiece();
		printBoard(board);
		if (judge(board, playeri, piece)==1) {
			printf("Player %d win!\n", playeri);
			break;
		}
	}
	return 0;
}
void initialBoard(char a[][N]) {     //用25个字母标记
	char cnt = 'a';
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			a[i][j] = cnt++;
		}
	}
	return;
}
void printBoard(char a[][N]) {
	system("cls");
	printf("\n");
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N - 1; ++j) {
			printf(" %c |",a[i][j]);
		}
		printf(" %c |\n",a[i][N-1]);
		if (i != N - 1) {
			for (int k = 0; k < N; ++k) {
				printf("---");
				if (k != N - 1)printf("+");
			}
			printf("\n");
		}
	}
	return;
}
char getpiece(void) {
	char ch = getchar();
	while(!isalpha(ch))ch = getchar();
	return ch;
}
int move(char a[][N], int player, char ch) {
	if (ch < 'a' || ch > 'a' + N * N - 1) {
		printf("Illegal move!\n");
		return 0;
	}
	else ch -= 'a';
	int i = ch / 5, j = ch % 5;
	cout << player << endl;
	if (a[i][j] != 'O' && a[i][j] != 'X') {
		if (player ==1)a[i][j] = 'O';
		else a[i][j] = 'X';
	}
	else {
		printf("This Gird has been occupied.\n");
		return 0;
	}
	return 1;
}

int judge(char a[][N], int player, char cc) {     //根据最后一颗棋子判断是否结束
	int x = (cc - 'a') / 5, y = (cc - 'a') % 5;
	char ch = player == 1 ? 'O' : 'X';
	int winn = 1;
	for (int i = 0; i < N; ++i) {     //列
		if (a[i][y] != ch) {
			winn = 0;break;
		}
	}
	if (winn)return winn;
	winn = 1;
	for (int i = 0; i < N; ++i) {     //行
		if (a[x][i] != ch) {
			winn = 0; break;
		}
	}
	if (winn)return winn;
	if (x == y) {     //正斜线
		winn = 1;
		for (int i = 0; i < N; ++i) {
			if (a[i][i] != ch) {
				winn = 0; break;
			}
		}
	}
	else if (x == N - 1 - y) {     //反斜线
		winn = 1;
		for (int i = 0; i < N; ++i) {
			if (a[i][N - 1 - i] != ch) {
				winn = 0; break;
			}
		}
	}
	return winn;
}
