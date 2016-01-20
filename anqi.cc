/*****************************************************************************\
 * Theory of Computer Games: Fall 2013
 * Chinese Dark Chess Library by You-cheng Syu
 *
 * This file may not be used out of the class unless asking
 * for permission first.
 *
 * Modify by Hung-Jui Chang, December 2013
\*****************************************************************************/
#include<cassert>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include"anqi.hh"
#include "chinese.h"
static const char *tbl="KGMRNCPkgmrncpX-";
#include <iostream>
using namespace std;
int compare_mov(const void * lhs, const void * rhs) {
	if(((MOV *) lhs)->is_eat_move )
		return -1;
	if(((MOV *)rhs)->is_eat_move)
		return 1;
	return 0;
}
extern const char *nam[16];
extern const char * strings[5];
static const POS ADJ[32][4]={
	{ 1,-1,-1, 4},{ 2,-1, 0, 5},{ 3,-1, 1, 6},{-1,-1, 2, 7},
	{ 5, 0,-1, 8},{ 6, 1, 4, 9},{ 7, 2, 5,10},{-1, 3, 6,11},
	{ 9, 4,-1,12},{10, 5, 8,13},{11, 6, 9,14},{-1, 7,10,15},
	{13, 8,-1,16},{14, 9,12,17},{15,10,13,18},{-1,11,14,19},
	{17,12,-1,20},{18,13,16,21},{19,14,17,22},{-1,15,18,23},
	{21,16,-1,24},{22,17,20,25},{23,18,21,26},{-1,19,22,27},
	{25,20,-1,28},{26,21,24,29},{27,22,25,30},{-1,23,26,31},
	{29,24,-1,-1},{30,25,28,-1},{31,26,29,-1},{-1,27,30,-1}
};
const int order_table[16][16]= {
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,1 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,1 },
	{1 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{0 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{0 ,0 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{0 ,0 ,0 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{0 ,0 ,0 ,0 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{1 ,1 ,1 ,1 ,1 ,1 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{1 ,0 ,0 ,0 ,0 ,0 ,1 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 },
	{0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 }};

CLR GetColor(FIN f) {
	return f<FIN_X?f/7:-1;
}

LVL GetLevel(FIN f) {
	assert(f<FIN_X);
	return LVL(f%7);
}

bool ChkEats(FIN fa,FIN fb) {
	return order_table[fa][fb];
}

static void Output(FILE *fp,POS p) {
	fprintf(fp,"%c%d\n",'a'+p%4,8-(p/4));
}

void Output(MOV m) {
	FILE *fp=fopen("move.txt","w");
	assert(fp!=NULL);
	if(m.ed!=m.st) {
		fputs("0\n",fp);
		Output(fp,m.st);
		Output(fp,m.ed);
		fputs("0\n",fp);
	} else {
		fputs("1\n",fp);
		Output(fp,m.st);
		fputs("0\n",fp);
		fputs("0\n",fp);
	}
	fclose(fp);
}

void BOARD::NewGame() {
	static const int tbl[]={1,2,2,2,2,2,5};
	who=-1;
	total_sum[0] = total_sum[1] = 16;
	for(POS p=0;p<32;p++)fin[p]=FIN_X;
	for(int i=0;i<14;i++){
		cnt[i]=tbl[GetLevel(FIN(i))];
		total_cnt[i] = initial_pieces[i];
	}

}

static FIN find(char c) {
	return FIN(strchr(tbl,c)-tbl);
}

static POS LoadGameConv(const char *s) {
	return (8-(s[1]-'0'))*4+(s[0]-'a');
}

static void LoadGameReplay(BOARD &brd,const char *cmd) {
	if(cmd[2]!='-')brd.Flip(LoadGameConv(cmd),find(cmd[3]));
	else brd.Move(MOV(LoadGameConv(cmd),LoadGameConv(cmd+3)));
}

static POS mkpos(int x,int y) {
	return x*4+y;
}

void BOARD::Init(int Board[32], int Piece[14], int Color) {
    for (int i = 0 ; i < 14; ++i) {
			cnt[i] = Piece[i];
			total_cnt[i] = initial_pieces[i];
		}
		total_sum [0] = total_sum[1] = 16;
    for (int i = 0 ; i < 32; ++i) {
			switch(Board[i]) {
			    case 0: fin[i] = FIN_E;break;
			    case 1: fin[i] = FIN_K;cnt[FIN_K]--; key ^= zobrist_table[i][FIN_K]; check ^= check_table[i][FIN_K];break;
			    case 2: fin[i] = FIN_G;cnt[FIN_G]--; key ^= zobrist_table[i][FIN_G]; check ^= check_table[i][FIN_G];break;
			    case 3: fin[i] = FIN_M;cnt[FIN_M]--; key ^= zobrist_table[i][FIN_M]; check ^= check_table[i][FIN_M];break;
			    case 4: fin[i] = FIN_R;cnt[FIN_R]--; key ^= zobrist_table[i][FIN_R]; check ^= check_table[i][FIN_R];break;
			    case 5: fin[i] = FIN_N;cnt[FIN_N]--; key ^= zobrist_table[i][FIN_N]; check ^= check_table[i][FIN_N];break;
			    case 6: fin[i] = FIN_C;cnt[FIN_C]--; key ^= zobrist_table[i][FIN_C]; check ^= check_table[i][FIN_C];break;
			    case 7: fin[i] = FIN_P;cnt[FIN_P]--; key ^= zobrist_table[i][FIN_P]; check ^= check_table[i][FIN_P];break;
			    case 8: fin[i] = FIN_X;break;
			    case 9: fin[i] = FIN_k;cnt[FIN_k]--;key ^= zobrist_table[i][FIN_k]; check ^= check_table[i][FIN_k];break;
			    case 10: fin[i] = FIN_g;cnt[FIN_g]--;key ^= zobrist_table[i][FIN_g]; check ^= check_table[i][FIN_g];break;
			    case 11: fin[i] = FIN_m;cnt[FIN_m]--;key ^= zobrist_table[i][FIN_m]; check ^= check_table[i][FIN_m];break;
			    case 12: fin[i] = FIN_r;cnt[FIN_r]--;key ^= zobrist_table[i][FIN_r]; check ^= check_table[i][FIN_r];break;
			    case 13: fin[i] = FIN_n;cnt[FIN_n]--;key ^= zobrist_table[i][FIN_n]; check ^= check_table[i][FIN_n];break;
			    case 14: fin[i] = FIN_c;cnt[FIN_c]--;key ^= zobrist_table[i][FIN_c]; check ^= check_table[i][FIN_c];break;
			    case 15: fin[i] = FIN_p;cnt[FIN_p]--;key ^= zobrist_table[i][FIN_p]; check ^= check_table[i][FIN_p];break;
			}
    }
    who = Color;
}

void BOARD::Init(char Board[32], int Piece[14], int Color) {
    key = 0;
    check =0 ;
    for (int i = 0 ; i < 14; ++i) {
			cnt[i] = Piece[i];
			total_cnt[i] = initial_pieces[i];
    }
		total_sum[0] = total_sum[1] =16;
		for (int i = 0 ; i < 32; ++i) {
			switch(Board[i]) {
			    case '-': fin[i] = FIN_E;break;
			    case 'K': fin[i] = FIN_K;cnt[FIN_K]--; key ^= zobrist_table[i][FIN_K]; check ^= check_table[i][FIN_K];break;
			    case 'G': fin[i] = FIN_G;cnt[FIN_G]--; key ^= zobrist_table[i][FIN_G]; check ^= check_table[i][FIN_G];break;
			    case 'M': fin[i] = FIN_M;cnt[FIN_M]--; key ^= zobrist_table[i][FIN_M]; check ^= check_table[i][FIN_M];break;
			    case 'R': fin[i] = FIN_R;cnt[FIN_R]--; key ^= zobrist_table[i][FIN_R]; check ^= check_table[i][FIN_R];break;
			    case 'N': fin[i] = FIN_N;cnt[FIN_N]--; key ^= zobrist_table[i][FIN_N]; check ^= check_table[i][FIN_N];break;
			    case 'C': fin[i] = FIN_C;cnt[FIN_C]--; key ^= zobrist_table[i][FIN_C]; check ^= check_table[i][FIN_C];break;
			    case 'P': fin[i] = FIN_P;cnt[FIN_P]--; key ^= zobrist_table[i][FIN_P]; check ^= check_table[i][FIN_P];break;
			    case 'X': fin[i] = FIN_X;break;
			    case 'k': fin[i] = FIN_k;cnt[FIN_k]--;key ^= zobrist_table[i][FIN_k]; check ^= check_table[i][FIN_k];break;
			    case 'g': fin[i] = FIN_g;cnt[FIN_g]--;key ^= zobrist_table[i][FIN_g]; check ^= check_table[i][FIN_g];break;
			    case 'm': fin[i] = FIN_m;cnt[FIN_m]--;key ^= zobrist_table[i][FIN_m]; check ^= check_table[i][FIN_m];break;
			    case 'r': fin[i] = FIN_r;cnt[FIN_r]--;key ^= zobrist_table[i][FIN_r]; check ^= check_table[i][FIN_r];break;
			    case 'n': fin[i] = FIN_n;cnt[FIN_n]--;key ^= zobrist_table[i][FIN_n]; check ^= check_table[i][FIN_n];break;
			    case 'c': fin[i] = FIN_c;cnt[FIN_c]--;key ^= zobrist_table[i][FIN_c]; check ^= check_table[i][FIN_c];break;
			    case 'p': fin[i] = FIN_p;cnt[FIN_p]--;key ^= zobrist_table[i][FIN_p]; check ^= check_table[i][FIN_p];break;
			}
    }
		for (int i =0 ;i < 14 ;i ++){
			for (int j=0 ; j < 14 ;j ++){
				if(order_table[i][j]){
					if(order_table[j][i]){
							eat_cnt[i].num_cs ++;
					}else{
							eat_cnt[i].num_ucs++;
					}
				}
			}
		}
    who = Color;
		key ^= turn[who];
		check ^= turn[who];
}

int BOARD::LoadGame(const char *fn) {
	FILE *fp=fopen(fn,"r");
	assert(fp!=NULL);

	while(fgetc(fp)!='\n');

	while(fgetc(fp)!='\n');
	total_sum[0] = total_sum[1] = 16;
	fscanf(fp," %*c");
	for(int i=0;i<14;i++)fscanf(fp,"%d",cnt+i);
	for (int i = 0 ; i < 14; ++i) {
		total_cnt[i] = initial_pieces[i];
	}
	for(int i=0;i<8;i++) {
		fscanf(fp," %*c");
		for(int j=0;j<4;j++) {
			char c;
			fscanf(fp," %c",&c);
			fin[mkpos(i,j)]=find(c);
		}
	}

	int r;
	fscanf(fp," %*c%*s%d" ,&r);
	who=(r==0||r==1?r:-1);
	fscanf(fp," %*c%*s%d ",&r);

	for(char buf[64];fgets(buf,sizeof(buf),fp);) {
		if(buf[2]<'0'||buf[2]>'9')break;
		char xxx[16],yyy[16];
		const int n=sscanf(buf+2,"%*s%s%s",xxx,yyy);
		if(n>=1)LoadGameReplay(*this,xxx);
		if(n>=2)LoadGameReplay(*this,yyy);
	}

	fclose(fp);
	return r;
}

void BOARD::Display() const {
#ifdef _WINDOWS
	HANDLE hErr=GetStdHandle(STD_ERROR_HANDLE);
#endif
	for(int i=0;i<8;i++) {
#ifdef _WINDOWS
		SetConsoleTextAttribute(hErr,8);
#endif
		for(int j=0;j<4;j++)fprintf(stderr,"[%02d]",mkpos(7-i,j));
		if(i==2) {
#ifdef _WINDOWS
			SetConsoleTextAttribute(hErr,12);
#endif
			fputs("  ",stderr);
			for(int j=0;j<7;j++)for(int k=0;k<cnt[j];k++)for(int j=7;j<14;j++)for(int k=0;k<cnt[j];k++)fprintf(stderr,ANSI_COLOR_RED "%s" ANSI_COLOR_RESET ,nam[j]);
		}
		fputc('\n',stderr);
		for(int j=0;j<4;j++) {
			const FIN f=fin[mkpos(7-i,j)];
			const CLR c=GetColor(f);
#ifdef _WINDOWS
			SetConsoleTextAttribute(hErr,(c!=-1?12-c*2:7));
#endif
		if(c==0)
			fprintf(stderr, ANSI_COLOR_RED " %s " ANSI_COLOR_RESET,nam[fin[mkpos(7-i,j)]]);
		else if(c==1)
			fprintf(stderr, ANSI_COLOR_BLUE " %s " ANSI_COLOR_RESET,nam[fin[mkpos(7-i,j)]]);
		else
			fprintf(stderr, " %s " ,nam[fin[mkpos(7-i,j)]]);

		}
		if(i==0) {
#ifdef _WINDOWS
			SetConsoleTextAttribute(hErr,7);
#endif
			fputs(strings[0],stderr);
			if(who==0) {
#ifdef _WINDOWS
				SetConsoleTextAttribute(hErr,12);
#endif

				fprintf(stderr,ANSI_COLOR_RED "%s" ANSI_COLOR_RESET ,strings[1]);
			} else if(who==1) {
#ifdef _WINDOWS
				SetConsoleTextAttribute(hErr,10);
#endif

				fprintf(stderr,ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET ,strings[2]);
			} else {
				fputs(strings[3],stderr);
			}
		} else if(i==1) {
#ifdef _WINDOWS
			SetConsoleTextAttribute(hErr,7);
#endif
			fputs(strings[4],stderr);
		} else if(i==2) {
#ifdef _WINDOWS
			SetConsoleTextAttribute(hErr,10);
#endif
			fputs("  ",stderr);
			for(int j=7;j<14;j++)for(int k=0;k<cnt[j];k++)fprintf(stderr,ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET ,nam[j]);
		}
		fputc('\n',stderr);
	}
#ifdef _WINDOWS
	SetConsoleTextAttribute(hErr,7);
#endif
	for (int i=0;i < 14 ;i++){
		fprintf(stderr,"%d ",total_cnt[i]);
	}
	fputc('\n',stderr);
	for (int i=0;i < 14 ;i++){
		fprintf(stderr,"%d ",cnt[i]);
	}
	fputc('\n',stderr);
	fprintf(stderr,"red = %d , black = %d\n",total_sum[0],total_sum[1]);
}

int BOARD::MoveGen(MOVLST &lst) const {
	if(who==-1)return false;

	for(POS p=0;p<32;p++) {
		const FIN pf=fin[p];
		if(GetColor(pf)!=who)continue; // if not own piece , continue

		const LVL pl=GetLevel(pf);

		if(pl!=LVL_C){
			for(int z=0;z<4;z++) {
				bool eat = false;
				const POS q=ADJ[p][z];
				if(q==-1)
					continue;
				const FIN qf=fin[q];
				if(!ChkEats(pf,qf))
					continue;
				lst.mov[lst.num++]=MOV(p,q,qf!=FIN_E);
			}
		}else{
			for(int z=0;z<4;z++) {
				int c=0;
				POS q = ADJ[p][z];
				if(q==-1)
					break;
				if(fin[q] ==FIN_E){
					lst.mov[lst.num++] = MOV(p,q,false);
				}


				do{
					const FIN qf=fin[q];

					if(qf==FIN_E||++c!=2) // count the second non_empty slot
						continue;

					if(qf!=FIN_X &&	GetColor(qf)!=who){
						lst.mov[lst.num++]=MOV(p,q,true);
					}

					break;
				}while((q=ADJ[q][z])!=-1);

			}
		}


	}
	//qsort(lst.mov,lst.num,sizeof(MOV),compare_mov);
	lst.sort();
	return lst.num;
}

bool BOARD::ChkLose() const {
	if(who==-1)return false;

	bool fDark=false;
	for(int i=0;i<14;i++) {
		if(cnt[i]==0)continue;
		if(GetColor(FIN(i))==who)return false; // if i have at least one piece, not lost
		fDark=true;	// at least one dark
	}

	bool fLive=false;
	for(POS p=0;p<32;p++)
		if(GetColor(fin[p])==who){	// if at least one alive return true
			fLive=true;
			break;
		}

	if(!fLive)return true;	// if no alive piece , return true

	MOVLST lst;
	return !fDark&&MoveGen(lst)==0;
}
bool BOARD::ChkWin() const {
	if(who==-1)return false;
	int oppo = who^1;
	bool fDark=false;
	for(int i=0;i<14;i++) {
		if(cnt[i]==0)continue;
		if(GetColor(FIN(i))==oppo)return false; // if i have at least one piece, not lost
		fDark=true;	// at least one dark
	}

	bool fLive=false;
	for(POS p=0;p<32;p++)
		if(GetColor(fin[p])==oppo){	// if at least one alive return true
			fLive=true;
			break;
		}

	if(!fLive)return true;	// if no alive piece , return true

	MOVLST lst;
	return !fDark&&MoveGen(lst)==0;
}

bool BOARD::ChkValid(MOV m) const {
	if(m.ed!=m.st) {
		MOVLST lst;
		MoveGen(lst);
		for(int i=0;i<lst.num;i++)if(m==lst.mov[i])return true;
	} else {
		if(m.st<0||m.st>=32)return false;
		if(fin[m.st]!=FIN_X)return false;
		for(int i=0;i<14;i++)if(cnt[i]!=0)return true;
	}
	return false;
}

void BOARD::Flip(POS p,FIN f) {
	if(f==FIN_X) {
		int i,sum=0;
		for(i=0;i<14;i++)    sum+=cnt[i];
		sum=rand()%sum;
		for(i=0;i<14;i++)
			if((sum-=cnt[i])<0)
				break;
		f=FIN(i);
	}
	fin[p]=f;
	cnt[f]--;
	if(who==-1)
		who=GetColor(f);
	who^=1;
	key^= turn[who];
	check^= turn[who];
}

void BOARD::Move(MOV m) {
	if(m.ed!=m.st) {

		FIN st = fin[m.st];
		FIN ed = fin[m.ed];
		if(ed != FIN_E){ // remove the effect of removed piece
			key ^= zobrist_table[m.ed][ed];
			check ^= check_table[m.ed][ed];
			//cerr<<" st = "<<st<<" ed= " << ed<<endl;
			total_cnt[ed]--;
			total_sum[GetColor(ed)] --;
			for(int x= 0; x < 14 ;x++){
				if(order_table[x][ed]){
					if(order_table[ed][x])
						eat_cnt[x].num_cs--;
					else
						eat_cnt[x].num_ucs--;
				}
			}
		}
		// remove the effect of moved piece
		key ^= zobrist_table[m.st][st];
		check ^= check_table[m.st][st];
		// add the effect of moved piece
		key ^= zobrist_table[m.ed][st];
		check ^= check_table[m.ed][st];

		fin[m.ed]=fin[m.st];
		fin[m.st]=FIN_E;
		who^=1;
		key^= turn[who];
		check ^= turn[who];
	} else {

		Flip(m.st);
		key ^= zobrist_table[m.st][fin[m.st]];
		check ^= check_table[m.st][fin[m.st]];
	}
}

void BOARD::DoMove(MOV m, FIN f) {
    if (m.ed!=m.st) {

			FIN st = fin[m.st];
			FIN ed = fin[m.ed];
			if(ed != FIN_E){ // remove the effect of removed piece
				key ^= zobrist_table[m.ed][ed];
				check ^= check_table[m.ed][ed];
				total_cnt[ed]--;
				total_sum[GetColor(ed)] --;

				for(int x= 0; x < 14 ;x++){
					if(order_table[x][ed]){
						if(order_table[ed][x])
							eat_cnt[x].num_cs--;
						else
							eat_cnt[x].num_ucs--;
					}
				}

			}
			// remove the effect of moved piece
			key ^= zobrist_table[m.st][st];
			check ^= check_table[m.st][st];
			// add the effect of moved piece
			key ^= zobrist_table[m.ed][st];
			check ^= check_table[m.ed][st];

			fin[m.ed]=fin[m.st];
			fin[m.st]=FIN_E;
			who^=1;
			key^= turn[who];
			check ^= turn[who];
		}
    else {
			Flip(m.st, f);
			key ^= zobrist_table[m.st][f];
			check ^= check_table[m.st][f];
    }
}

SCORE BOARD::Eval2() const {
	int cnt2[2]={0,0};
	for(POS p=0;p<32;p++){const CLR c=GetColor(fin[p]);if(c!=-1)cnt2[c]++;}
	for(int i=0;i<14;i++)cnt2[GetColor(FIN(i))]+=cnt[i];
	return cnt2[who]-cnt2[who^1];

}

SCORE BOARD::Eval() const {
	int ret_cnt[2] = {0,0};
	int piece_value[14] ;
	for (int i = 0; i < 14 ;i ++){
		piece_value[i]= 0;
		if(i==5 || i == 12 ){
		//	ret_cnt[GetColor(FIN(i))]+= 4 * total_cnt[i]
			continue;
		}else{
			piece_value[i] = total_cnt[i]*GetValue(i);
		}
			//ret_cnt[GetColor(FIN(i))]+= total_cnt[i] * (eat_cnt[i].score()+50);
	}
	for (int i= 0 ;i < 14 ;i++){
		ret_cnt[GetColor(FIN(i))]+= total_cnt[i] * (piece_value[i]+50);
	}
	piece_value[5] = (piece_value[1]* 4 )/15;
	piece_value[5] = (piece_value[1]* 4 )/15;

	return ret_cnt[who]-ret_cnt[who^1];
}
int BOARD::GetValue(int f) const{
	int ret =0 ;
	for (int j = 0; j< 14 ;j++){
		if(order_table[f][j]){
			if(j==1 ||  j== 5){
				ret+= 4*total_sum[GetColor(FIN(j))]+total_sum[GetColor(FIN(j))^1];
			}else
				ret += (eat_cnt[j].score() * total_cnt[j]);
		}
	}
	return ret;
}
