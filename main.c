#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<ctype.h>
#include<cmath>
#include<algorithm>
#include<climits>
#include<Windows.h>

#define M INT_MAX-1
//极限
#define DEPTH 0
//机器搜索深度

using namespace std;

const int N=5;
//棋盘大小
const int N2=N*N;

char board[N][N];
char piece; char newgame; char grid[N*N+2];
int Identity[3] = { 0,1,2 }; int playeri; int playerid[3];
int availiblegrids=N2; int psb[N+N+3]; int occupied[N2+2];

void initialBoard(char board[][N]);
//初始化棋盘
void printBoard(const char board[][N]);   
//打印棋盘
char getPiece(void);
//获取棋子位置
int move(char board[][N], int player, char grid);  
//落子
void remove(char board[][N], char grid);
//移除一个棋子
int judge(const char board[][N], int player, char cc);  
//判断胜负
int alphabeta(char board[][N], int playr, int availiblegrids, \
              int alpha, int beta, int search_depth,char *machine_move);
//机器选择落子位置
int getScore(const char bd[][N], int player, char grid);
//为一步棋赋分
void possible(const char board[][N],int *possibility);
//判断可能赢的路线
void printdata(void);
//打印数据（调试用）

int main(void) {
	newgame = 'Y';
	do {
		initialBoard(board);
		printBoard(board);
		printf("请选择先手（1.人 2.电脑）");
		do {
			scanf("%d", &playerid[1]);
		} while (playerid[1] != 1 && playerid[1] != 2);
		printf("请选择后手（1.人 2.电脑）");
		do {
			scanf("%d", &playerid[2]);
		} while (playerid[2] != 1 && playerid[2] != 2);

		for (int i = 0; i < N * N; ++i) {
			playeri = i % 2 + 1;
			if (Identity[playerid[playeri]] == 1) {
				printf("输入落子位置：  ");
				piece = getPiece();
				while(move(board, playeri, piece)==-1){
					piece = getPiece();
					printBoard(board);
				}
				occupied[piece - 'a'] = 1;
			}
			else if (Identity[playerid[playeri]] == 2) {
				printBoard(board);
				//printdata();
				alphabeta(board,playeri,availiblegrids,-M,M,0,&piece);
				Sleep(500);
				move(board, playeri, piece);
			}
			printBoard(board);
			possible(board,psb);
			if(judge(board, playeri, piece)==-1) {
				printf("平局\n");
				break;
			}
			else if(judge(board, playeri, piece)==1){
				printf("玩家 %d 获胜!\\((*^▽^*))/\n",playeri);
				break;
			}
		}
		printf("再玩一次?(Y/N)\n");
		getchar();
		scanf("%c", &newgame, 1);
	} while (toupper(newgame) == 'Y');
	return 0;
}

void initialBoard(char bd[][N]) { 
	char ch = 'a';
	int i=0;
	for (i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			bd[i][j] = ' ';
		}
	}
	for (i = 0; i < N * N; ++i)
		grid[i] = ch + i;
	for(i=0;i<N2;++i)occupied[i]=0;
	for(i=0;i<N+N+2;++i)psb[i]=1;
	return;
}
//初始化 

void printBoard(const char board[][N])
{
	system("cls");
	printf("=======================  =======================\n");
	printf(" | %c | %c | %c | %c | %c |    | A | B | C | D | E |\n", board[0][0], board[0][1], board[0][2], board[0][3], board[0][4]);
	printf(" +---+---+---+---+---+    +---+---+---+---+---+\n");
	printf(" | %c | %c | %c | %c | %c |    | F | G | H | I | J |\n", board[1][0], board[1][1], board[1][2], board[1][3], board[1][4]);
	printf(" +---+---+---+---+---+    +---+---+---+---+---+\n");
	printf(" | %c | %c | %c | %c | %c |    | K | L | M | N | 0 |\n", board[2][0], board[2][1], board[2][2], board[2][3], board[2][4]);
	printf(" +---+---+---+---+---+    +---+---+---+---+---+\n");
	printf(" | %c | %c | %c | %c | %c |    | P | Q | R | S | T |\n", board[3][0], board[3][1], board[3][2], board[3][3], board[3][4]);
	printf(" +---+---+---+---+---+    +---+---+---+---+---+\n");
	printf(" | %c | %c | %c | %c | %c |    | U | V | W | X | Y |\n", board[4][0], board[4][1], board[4][2], board[4][3], board[4][4]);
	printf("=======================  =======================\n");
}
//打印棋盘 

char getPiece(void) {
	char ch = getchar();
	while (!isalpha(ch) || tolower(ch) == 'z') {
		ch = getchar();
	}
	return tolower(ch);
}
//获取玩家落子位置  

int move(char bd[][N], int player, char ch) {
	while (ch < 'a' || ch > 'a' + N * N - 1) {
		printf("非法输入!\n");
		return -1;
	}
	//判断输入是否合法
	ch -= 'a';
	int x = ch / N, y = ch % N;
	if (bd[x][y] != 'O' && bd[x][y] != 'X') {
		if (player == 1)bd[x][y] = 'O';
		else bd[x][y] = 'X';
		occupied[ch] = 1;
		--availiblegrids;
		return 1;
	}
	else {
		printf("位置非空\n",ch);
		return -1;
	}
	//判断位置是否可用
	return 0;
}
void remove(char bd[][N], char ch) {
	ch -= 'a';
	if(occupied[ch]!=0){
		bd[ch / N][ch % N] = ch + 'a';
		occupied[ch] = 0;
		++availiblegrids;
	}
	return;
}
//移除一颗棋子（机器搜索用）

int judge(const char bd[][N], int player, char cc) {    
	int x = (cc - 'a') / N, y = (cc - 'a') % N;
	char ch = player == 1 ? 'O' : 'X';
	int winn = 1,i=0,cnt=0;
	
	for(i=0;i<N+N+2;++i)cnt+=psb[i];
	if(cnt==0)return -1;
	
	for (i = 0; i < N; ++i) {   
		if (bd[i][y] != ch) {
			winn = 0; break;
		}
	}
	if (winn)return winn;
	winn = 1;
	for (i = 0; i < N; ++i) {    
		if (bd[x][i] != ch) {
			winn = 0; break;
		}
	}
	if (winn)return winn;
	if (x == y) {   
		winn = 1;
		for (int i = 0; i < N; ++i) {
			if (bd[i][i] != ch) {
				winn = 0; break;
			}
		}
	}
	else if (x == N - 1 - y) {  
		winn = 1;
		for (int i = 0; i < N; ++i) {
			if (bd[i][N - 1 - i] != ch) {
				winn = 0; break;
			}
		}
	}
	return winn;
}
//-1：平局 0：未结束 1：分出胜负

int alphabeta(char bd[][N], int plr, int ags, int alpha, int beta, int sd,char *mmv) {
	if(ags == 0 )return 0;
	int value=0; int pv=0;
	if(sd<=DEPTH){
		for(int i=0;i<N2;++i){
			if(occupied[i]==1)continue;
			pv=getScore(bd,plr,grid[i]);
			if(plr==1 && pv>value){
				value = pv;
				*mmv=grid[i];
			}
			else if(plr==2 && pv<value){
				value = pv;
				*mmv=grid[i];
			}
		}
		return value;
	}	
	if(plr == 1){
	  value=-M;
		for(int i = 0;i < N2;++i){
			if(occupied[i]!=0)continue;
			move(bd,plr,grid[i]);
			pv=alphabeta(bd,(plr%2)+1,ags-1,alpha,beta,sd+1,mmv);
			if(pv>value){
				value=pv;
				*mmv=grid[i];
			}
			alpha=max(value,alpha);
			remove(bd,grid[i]);
			if(alpha>=beta){
				return value;
				break;
			}
		}
	}
	else{
		value = M;
		for(int i=0;i<N2;++i){
			if(occupied[i]!=0)continue;
			move(bd,plr,grid[i]);
			pv = alphabeta(bd,plr%2+1,ags-1,alpha,beta,sd+1,mmv);
			if(pv < value){
				value=pv;
				*mmv=grid[i];
			}
			beta=min(value,beta);
			remove(bd,grid[i]);
			if(beta<=alpha){
				return value;
				break;
			}
		}
	}
	return value;
}
//Minimax + alpha-beta剪枝

int getScore(const char bd[][N], int plr, char cc) {
	if(occupied[cc-'a']==1)return 0;
	int x = (cc - 'a') / N, y = (cc - 'a') % N;
	char ch1 = plr == 1 ? 'O' : 'X';
	char ch2 = plr == 1 ? 'X' : 'O';
	int score = 0, cnt = 0, i = 0;
	
	if(cc=='a'+N2/2)score=20;
	if(psb[x]){
		for (cnt = 1, i = y + 1; i < N && bd[x][i] == ch1; ++i, ++cnt);
		for (i = y - 1; i >= 0 && bd[x][i] == ch1; --i, ++cnt);
		score = max(score, 2 * cnt);
		for (cnt = 1, i = y + 1; i < N && bd[x][i] == ch2; ++i, ++cnt);
		for (i = y - 1; i >= 0 && bd[x][i] == ch2; --i, ++cnt);
		score = max(score, 2 * cnt - 1);
	}
	if(psb[N+y]){
		for (cnt = 1, i = x + 1; i < N && bd[i][y] == ch1; ++i, ++cnt);
		for (i = x - 1; i >= 0 && bd[i][y] == ch1; --i, ++cnt);
		score = max(score, 2 * cnt);
		for (cnt = 1, i = x + 1; i < N && bd[i][y] == ch2; ++i, ++cnt);
		for (i = x - 1; i >= 0 && bd[i][y] == ch2; --i, ++cnt);
		score = max(score, 2 * cnt - 1);
	}
	if (x == y && psb[N+N]) {
		for (cnt = 1, i = x + 1; i < N && bd[i][i] == ch1; ++i, ++cnt);
		for (i = x - 1; i >= 0 && bd[i][i] == ch1; --i, ++cnt);
		score = max(score, 2 * cnt);
		for (cnt = 1, i = x + 1; i < N && bd[i][i] == ch2; ++i, ++cnt);
		for (i = x - 1; i >= 0 && bd[i][i] == ch2; --i, ++cnt);
		score = max(score, 2 * cnt - 1);
	}
	if (x == N - 1 - y && psb[N+N+1]) {
		for (cnt = 1, i = x + 1; i < N && bd[i][N - 1 - i] == ch1; ++i, ++cnt);
		for (i = x - 1; i >= 0 && bd[i][N - 1 - i] == ch1; --i, ++cnt);
		score = max(score, 2 * cnt);
		for (cnt = 1, i = x + 1; i < N && bd[i][N - 1 - i] == ch2; ++i, ++cnt);
		for (i = x - 1; i >= 0 && bd[i][N - 1 - i] == ch2; --i, ++cnt);
		score = max(score, 2 * cnt - 1);
	}

	if (plr == 2)score *= -1;
	return score;
}
//赋分系统
//分数越大对玩家1越有利，越小对玩家2越有利

void possible(const char bd[][N],int *psb){
	int i=0,j=0,f1=0,f2=0;
	for(i=0;i<N;++i,f1=f2=0){
		if(psb[i]==1){
			for(j=0;j<N;++j){
				if(bd[i][j]=='O')f1=1;
				if(bd[i][j]=='X')f2=1;
			}
			if(f1&&f2)psb[i]=0;
		}
	}
	for(j=0;j<N;++j,f1=f2=0){
		if(psb[N+j]==1){
			for(i=0;i<N;++i){
				if(bd[i][j]=='O')f1=1;
				if(bd[i][j]=='X')f2=1;
			}
			if(f1&&f2)psb[N+j]=0;
		}
	}
	if(psb[N+N]==1){
		f1=f2=0;
		for(i=0;i<N;++i){
			if(bd[i][i]=='O')f1=1;
			if(bd[i][i]=='X')f2=1;
		}
		if(f1&&f2)psb[N+N]=0;
	}
	if(psb[N+N+1]==1){
		f1=f2=0;
		for(i=0;i<N;++i){
			if(bd[i][N-1-i]=='O')f1=1;
			if(bd[i][N-1-i]=='X')f2=1;
		}
		if(f1&&f2)psb[N+N+1]=0;
	}
}
//如果某条线上同时有O和X则不再考虑在这条线上落子

void printdata(void){
	int i=0,j=0;
	for(i=0;i<N;++i){
		for(j=0;j<N;++j){
			printf("%d ",getScore(board,playeri,grid[i*N+j]));
		}
		printf("\n");
	}
	printf("\n");
	for(i=0;i<N;++i){
		for(j=0;j<N;++j){
			printf("%d ",occupied[i*N+j]);
		}
		printf("\n");
	}
	for(i=0;i<N+N+2;++i)printf("%d ",psb[i]);
	getchar();
	return;
}
//输出检查
