/*****************************************************************************\
 * Theory of Computer Games: Fall 2012
 * Chinese Dark Chess Search Engine Template by You-cheng Syu
 *
 * This file may not be used out of the class unless asking
 * for permission first.
 *
 * Modify by Hung-Jui Chang, December 2013
\*****************************************************************************/
#include<cstdio>
#include<cstdlib>
#include <utility>
#include"anqi.hh"
#include "trans_table.h"
#include"Protocol.h"
#include"ClientSocket.h"
#include <iostream>
//#define MAX_HASH 999989
using namespace std;
#ifdef _WINDOWS
#include<windows.h>
#else
#include<ctime>
#endif
int max_length;
#ifdef _WINDOWS
DWORD Tick;     // �}�l�ɨ�
int   TimeOut;  // �ɭ�
#else
clock_t Tick;     // �}�l�ɨ�
clock_t TimeOut;  // �ɭ�
#endif
MOV   BestMove; // �j�X�Ӫ��̨ε۪k

bool TimesUp() {
#ifdef _WINDOWS
	return GetTickCount()-Tick>=TimeOut;
#else
	return clock() - Tick > TimeOut;
#endif
}
void init_table(){
	for (int i =0 ; i < MAX_HASH ; i++){
		if(trans_table[i]!=NULL)
		{
			cerr<<"Error Trans Table Initialization"<<i<<endl;
			exit(1);
		}
	}
}

SCORE nega_scout2(const BOARD & B,int alpha , int beta, int depth , int cut){
		MOVLST lst;
		SCORE t;
		SCORE n = beta;
		SCORE m = -INF;
		trans_node *tn = trans_table[B.key%MAX_HASH];

		if(tn!=NULL){ //not null
			if(B.key == tn->key && B.check == tn->check){ // hash hit
				if(tn->is_exact && cut <= tn->search_depth){
							return tn->best_value;
				}else{
					m  =  tn->best_value;
				}
			}else{ //Colllision will be replaced automatically

			}
		}else{ // not initialized yet
			tn = (trans_node *) malloc (sizeof(trans_node));
		}




		if(cut > max_length)
		{
			max_length = cut;
			cerr<<"New Depth : "<<cut<<endl;
		}

		if(B.ChkLose()){
			tn->set_node(B.key,B.check,cut,-WIN,MOV(),true);
			return -WIN;
		}
		if(B.ChkWin()){
			tn->set_node(B.key,B.check,cut,WIN,MOV(),true);
			return WIN;
		}
		if(cut==depth || TimesUp() ||B.MoveGen(lst)==0){
			tn->set_node(B.key,B.check,cut,B.Eval(),MOV(),true);
			return B.Eval();
		}
		//lst.sort();
		BOARD boards[68] ;
		for (int i=0; i < lst.num;i++){
			boards[i] = B;
			boards[i].Move(lst.mov[i]);
			lst.scores[i]= boards[i].Eval();
		}
		lst.sort();
		for (int i =0; i <lst.num ;i++){

			SCORE t = -nega_scout2(boards[i],-n,-get_max(alpha,m),depth,cut+1);
			if(t>m){
				if(n==beta || depth -cut <3 || t >= beta){
					m = t;
				}else{
					m = -nega_scout2(boards[i],-beta,-t,depth,cut+1);
				}
			}
			if(m>=beta){
				tn->set_node(B.key,B.check,cut,m,lst.mov[i],false);
				return m;
			}
			n = get_max(alpha,m)+1;
		}
		tn->set_node(B.key,B.check,cut,m,MOV(),true);
		return m;
}
SCORE Search_Max(const BOARD & B,int depth){
	MOVLST lst;
	SCORE alpha = -INF;
	SCORE beta = INF;
	SCORE n = beta;
	SCORE m = -INF;

	int cut =0 ;
	B.MoveGen(lst);
	if(lst.num ==0 ){
		BestMove =  MOV();
		return m;
	}
	BOARD boards[68] ;
	for (int i=0; i < lst.num;i++){
		boards[i] = B;
		boards[i].Move(lst.mov[i]);
		lst.scores[i]= boards[i].Eval();
	}
	lst.sort();
	BestMove = lst.mov[0];
	for (int i=0 ;i < lst.num;i++){
		SCORE t =  -nega_scout2(boards[i],-n,-get_max(alpha,m),depth,cut+1); // null window search
		if(t>m){ // if failed high
			if(n==beta || t>= beta){
				m = t;
			}else{
				m  = -nega_scout2(boards[i],-beta,-t,depth,cut+1); //research
			}
			BestMove = lst.mov[i];
		}
		if(m>=beta){ //cut off
			BestMove = lst.mov[i];
			return m;
		}
		n=get_max(alpha,m)+1;
	}
	/*for (int i = 0 ; i< lst.num;i++){
		BOARD N (B);
		N.Move(lst.mov[i]);

		SCORE t =  -nega_scout2(N,-n,-get_max(alpha,m),depth,cut+1); // null window search
		if(t>m){ // if failed high
			if(n==beta || t>= beta){
				m = t;
			}else{
				m  = -nega_scout2(N,-beta,-t,depth,cut+1); //research
			}
			BestMove = lst.mov[i];
		}
		if(m>=beta){ //cut off
			BestMove = lst.mov[i];
			return m;
		}
		n=get_max(alpha,m)+1;
	}*/

	return m;

}


MOV Play(const BOARD &B) {
#ifdef _WINDOWS
	Tick=GetTickCount();
	TimeOut = (DEFAULTTIME-3)*1000;
#else
	Tick=clock();
	TimeOut = (DEFAULTTIME-3)*CLOCKS_PER_SEC;
#endif
	POS p; int c=0;
	max_length = 0 ;
	int search_depth = 12;

	if(B.who==-1){p=rand()%32;printf("%d\n",p);return MOV(p,p);}

 	SCORE nega;
	for (int tt =1 ; tt <= search_depth ; tt++){
	  nega = Search_Max(B,tt) ;
		if(nega== WIN || TimesUp() )
			break;
	}
	if(BestMove.st !=-1){
		if(nega>B.Eval()){
				cerr << "find best move with value "<< nega << " move : " << (BestMove.st)<<" "<<(BestMove.ed) << endl;
				return BestMove;
		}else{
				cerr << "current best value "<<B.Eval()<< " is no less than nega_scout value "<< nega <<", fliping " << endl;
		}
	}
	SCORE m = -INF;
	SCORE n = - INF;
	MOV BestFlip = MOV();
	SCORE BestFlipScore = -INF;
	for (p =0 ; p < 32 ; p++){

		SCORE sum_p = 0;
		if(B.fin[p]==FIN_X){
			for(int i =0; i< 14; i ++){
				if(TimesUp()) break;
				if(B.cnt[i]==0) continue;
					double probo = (double ) B.cnt[i] / (B.dark_cnt[0]+B.dark_cnt[1]);
					BOARD N(B);
					N.Flip(p,FIN(i));
					SCORE t = -nega_scout2(N,-INF,-INF,6,0);
					sum_p += probo* t;
			}
			if(sum_p > BestFlipScore){
				BestFlipScore = sum_p;
				BestFlip = MOV(p,p);
			}
		}
	}


	if(BestFlipScore==-INF){
		if(BestMove.st!=-1)
			return BestMove;
		else
			cerr <<"GGGGGGGGG"<<endl;
	}


	return BestFlip;
}

FIN type2fin(int type) {
    switch(type) {
	case  1: return FIN_K;
	case  2: return FIN_G;
	case  3: return FIN_M;
	case  4: return FIN_R;
	case  5: return FIN_N;
	case  6: return FIN_C;
	case  7: return FIN_P;
	case  9: return FIN_k;
	case 10: return FIN_g;
	case 11: return FIN_m;
	case 12: return FIN_r;
	case 13: return FIN_n;
	case 14: return FIN_c;
	case 15: return FIN_p;
	default: return FIN_E;
    }
}
FIN chess2fin(char chess) {
    switch (chess) {
	case 'K': return FIN_K;
	case 'G': return FIN_G;
	case 'M': return FIN_M;
	case 'R': return FIN_R;
	case 'N': return FIN_N;
	case 'C': return FIN_C;
	case 'P': return FIN_P;
	case 'k': return FIN_k;
	case 'g': return FIN_g;
	case 'm': return FIN_m;
	case 'r': return FIN_r;
	case 'n': return FIN_n;
	case 'c': return FIN_c;
	case 'p': return FIN_p;
	default: return FIN_E;
    }
}

int main(int argc, char* argv[]) {
init_table();
printf("%lu\n",sizeof(trans_node));
fflush(stdout);
#ifdef _WINDOWS
	srand(Tick=GetTickCount());
#else
	srand(Tick=time(NULL));
#endif

	BOARD B;
	if (argc!=3) {
	    TimeOut=(B.LoadGame("board.txt")-3)*1000;
			B.Display();
			if(!B.ChkLose())Output(Play(B));
			B.Display();
	    return 0;
	}
	Protocol *protocol;
	protocol = new Protocol();
	protocol->init_protocol(argv[1],atoi(argv[2]));
	int iPieceCount[14];
	char iCurrentPosition[32];
	int type, remain_time;
	bool turn;
	PROTO_CLR color;

	char src[3], dst[3], mov[6];
	History moveRecord;
	protocol->init_board(iPieceCount, iCurrentPosition, moveRecord, remain_time);
	protocol->get_turn(turn,color);

	TimeOut = (DEFAULTTIME-3)*1000;

	B.Init(iCurrentPosition, iPieceCount, (color==2)?(-1):(int)color);

	MOV m;
	if(turn) // �ڥ�
	{
	    m = Play(B);
	    sprintf(src, "%c%c",(m.st%4)+'a', m.st/4+'1');
	    sprintf(dst, "%c%c",(m.ed%4)+'a', m.ed/4+'1');
	    protocol->send(src, dst);
	    protocol->recv(mov, remain_time);
	    if( color == 2)
				color = protocol->get_color(mov);
	    B.who = color;
	    B.DoMove(m, chess2fin(mov[3]));
	    protocol->recv(mov, remain_time);
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	}
	else // ������
	{
	    protocol->recv(mov, remain_time);
	    if( color == 2)
	    {
		color = protocol->get_color(mov);
		B.who = color;
	    }
	    else {
		B.who = color;
		B.who^=1;
	    }
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	}
	B.Display();
	while(1)
	{
	    m = Play(B);
	    sprintf(src, "%c%c",(m.st%4)+'a', m.st/4+'1');
	    sprintf(dst, "%c%c",(m.ed%4)+'a', m.ed/4+'1');
	    protocol->send(src, dst);
	    protocol->recv(mov, remain_time);
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	    B.Display();

	    protocol->recv(mov, remain_time);
	    m.st = mov[0] - 'a' + (mov[1] - '1')*4;
	    m.ed = (mov[2]=='(')?m.st:(mov[3] - 'a' + (mov[4] - '1')*4);
	    B.DoMove(m, chess2fin(mov[3]));
	    B.Display();
	}

	return 0;
}
