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
		for (int i =0; i <lst.num ;i++){
			BOARD N(B);
			N.Move(lst.mov[i]);
			SCORE t = -nega_scout2(N,-n,-get_max(alpha,m),depth,cut+1);
			if(t>m){
				if(n==beta || depth -cut <3 || t >= beta){
					m = t;
				}else{
					m = -nega_scout2(N,-beta,-t,depth,cut+1);
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
	}
	for (int i = 0 ; i< lst.num;i++){
		BOARD N (B);
		N.Move(lst.mov[i]);
		SCORE t =  -nega_scout2(N,-n,-get_max(alpha,m),depth,cut+1); // null window search
		if(t>m){ // if failed high
			if(n==beta || depth-cut < 3 || t>= beta){
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
	}

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

	for(p=0;p<32;p++)if(B.fin[p]==FIN_X)c++;

	if(c==0){
		if(BestMove.st!=-1)
			return BestMove;
		else
			cerr <<"GGGGGGGGG"<<endl;
	}
	c=rand()%c;
	for(p=0;p<32;p++)if(B.fin[p]==FIN_X&&--c<0)break;
	return MOV(p,p);
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
fflush(stdout);
#ifdef _WINDOWS
	srand(Tick=GetTickCount());
#else
	srand(Tick=time(NULL));
#endif

	BOARD B;
  if (argc!=3) {
      cout<<"const int order_table[16][16]= {";
      for(int i = 0 ; i < 16 ;i++){
        if(i!=0)
          cout<<",";
        cout<<"{";
        for(int j =0; j < 16 ;j++){
          if(j!=0)
            cout<<",";
          cout<< ChkEats(static_cast<FIN>(i),static_cast<FIN>(j)) <<" ";
        }
        cout<<"}";
      }
      cout<<"};"<<endl;
      /*TimeOut=(B.LoadGame("board.txt")-3)*1000;
			B.Display();
			if(!B.ChkLose())Output(Play(B));
			B.Display();*/
	    return 0;
	}

	return 0;
}
