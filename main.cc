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
#include <math.h>
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
int value[7]  = { 6095, 3047, 1523, 761, 380, 420, 200};
/*static const POS ADJ[32][4]={
	{ 1,-1,-1, 4},{ 2,-1, 0, 5},{ 3,-1, 1, 6},{-1,-1, 2, 7},
	{ 5, 0,-1, 8},{ 6, 1, 4, 9},{ 7, 2, 5,10},{-1, 3, 6,11},
	{ 9, 4,-1,12},{10, 5, 8,13},{11, 6, 9,14},{-1, 7,10,15},
	{13, 8,-1,16},{14, 9,12,17},{15,10,13,18},{-1,11,14,19},
	{17,12,-1,20},{18,13,16,21},{19,14,17,22},{-1,15,18,23},
	{21,16,-1,24},{22,17,20,25},{23,18,21,26},{-1,19,22,27},
	{25,20,-1,28},{26,21,24,29},{27,22,25,30},{-1,23,26,31},
	{29,24,-1,-1},{30,25,28,-1},{31,26,29,-1},{-1,27,30,-1}
};*/
extern const POS ADJ[32][4];
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
int search_route(int map[32],int our ,int enemy){
	int dist[32] = {0};
	int dist_ori[32] ={0};
	int dist_i[32] = {0};
	int dist_len[32] ={0};
	int index =0;
	int length =0;
	int history[20] ={0};
	int x =0;
	int now=our;
	int ori=0;
	int ret =0;
	while (index >= 0){
		for (int i = 0; i < 4; i++){
			if (ADJ[now][i] != -1){
				if (map[ADJ[now][i]] == 4){
					x = ADJ[now][i];
					for (int j = ori + 4; j >= 2; j--){
						for (int k = 0; k < 4; k++){
							if (map[ADJ[x][k]] == j){
								x = ADJ[x][k];
								if (j == 5)
									j = j - 2;
								if (j == 2){
									if (ori == 2){
										if ((ADJ[x][0] % 4 - 1 == enemy % 4 || ADJ[x][0] % 4 + 1 == enemy % 4) && (ADJ[x][0] / 4 - 1 == enemy / 4 || ADJ[x][0] / 4 + 1 == enemy / 4) && map[ADJ[x][0]] == 5){
											return 1;
										}
										if ((ADJ[x][1] % 4 - 1 == enemy % 4 || ADJ[x][1] % 4 + 1 == enemy % 4) && (ADJ[x][1] / 4 - 1 == enemy / 4 || ADJ[x][1] / 4 + 1 == enemy / 4) && map[ADJ[x][1]] == 5){
											return 2;
										}
										if ((ADJ[x][2] % 4 - 1 == enemy % 4 || ADJ[x][2] % 4 + 1 == enemy % 4) && (ADJ[x][2] / 4 - 1 == enemy / 4 || ADJ[x][2] / 4 + 1 == enemy / 4) && map[ADJ[x][2]] == 5){
											return 3;
										}
										if ((ADJ[x][3] % 4 - 1 == enemy % 4 || ADJ[x][3] % 4 + 1 == enemy % 4) && (ADJ[x][3] / 4 - 1 == enemy / 4 || ADJ[x][3] / 4 + 1 == enemy / 4) && map[ADJ[x][3]] == 5){
											return 4;
										}
									}
									if (k == 0){
										return 2;
									}
									else if (k == 1){
										return 3;
									}
									else if (k == 2){
										return 0;
									}
									else{
										return 1;
									}
								}
								break;
							}
						}
					}
					//return ori + 1;
				}
				if (map[ADJ[now][i]] == 0){
					map[ADJ[now][i]] = 5 + ori;
					dist_i[index] = i;
					dist_ori[index] = ADJ[now][i];
					dist_len[index] = ori + 1;
					dist[index++] = abs(ADJ[now][i] % 4 - enemy % 4) + abs(ADJ[now][i] / 4 - enemy / 4);
				}
			}
		}
		/*printf("\n");
		for (int i = 0; i < index; i++){
			printf("%d ", dist[i]);
		}
		printf("\n");*/
		int temp_dist = 0;
		int temp_dist_ori = 0;
		int temp_dist_i = 0;
		int temp_dist_len = 0;
		for (int i = 0; i < index; i++){
			int small_dist = 0;
			int small_ind = 0;
			for (int j = i; j < index; j++){
				if (dist[j] + dist_len[j] > small_dist){
					small_dist = dist[j] + dist_len[j];
					small_ind = j;
				}
			}
			temp_dist = dist[i];
			temp_dist_ori = dist_ori[i];
			temp_dist_i = dist_i[i];
			temp_dist_len = dist_len[i];
			dist[i] = dist[small_ind];
			dist_ori[i] = dist_ori[small_ind];
			dist_i[i] = dist_i[small_ind];
			dist_len[i] = dist_len[small_ind];
			dist[small_ind] = temp_dist;
			dist_ori[small_ind] = temp_dist_ori;
			dist_i[small_ind] = temp_dist_i;
			dist_len[small_ind] = temp_dist_len;
		}
		/*printf("\n");
		for (int i = 0; i < index; i++){
			printf("%d ", dist[i]);
		}
		printf("\n");
		for (int i = 0; i < index; i++){
			printf("%d ", dist_len[i]);
		}
		printf("\n");
		system("pause");*/
		//pop index-1
		now = dist_ori[index - 1];
		index--;
		ori = dist_len[index];
		if (ori == 1)
			ret = dist_i[index];
	}
	return -1;
}
int check_connect(const BOARD &B,int our ,int enemy){
	int map[32] = {0};
	int now = our;
	int x =0 ;
	map[our] = 2;
	map[enemy] =4;
	for (int i =0;i <32;i++){
			if(B.fin[i]!=FIN_E){
				if(GetColor(B.fin[i]) == B.who && i !=our)
					map[i] =1;
				else if(GetColor(B.fin[i]) == (B.who^1) && i!=enemy)
					map[i] =3;
			}
	}

	x = search_route(map,now,enemy);
	if(x!=-1)
		return x;
	return -1;
}
MOV SearchSpecial(const BOARD &B,int dark,int open_empty, int ours, int their){
	int flag,index;
	int info[4] = {-1,-1,-1,-1};
	flag = index =0;
	int has_paw =0 ;
	POS p;
	if(their ==0 ){
		for(p=0; p < 32;p++){
			if(GetLevel(B.fin[p]) == LVL_C){
				if(p%4+2 < 4 && B.fin[p+2] == FIN_X){
					has_paw =1;
					index =0;
					flag =0;
					if(p%4 +3 < 4 && GetColor(B.fin[p+3])==B.who)
						flag++;
					if(GetColor(B.fin[p+1])==B.who)
						flag++;
					if((p+2)/4+1 <8 && GetColor(B.fin[(p+2)+4]) == B.who )
						flag++;
					if((p+2)/4-1 >=0 && GetColor(B.fin[(p+2)-4]) == B.who )
						flag++;
					info[0]=flag;
				}
				if((!has_paw ||flag!=0) && p %4-2 >=0 && B.fin[p-2] == FIN_X){
					has_paw =1;
					index =1;
					flag =0 ;
					if(p%4 -3>=0 && GetColor(B.fin[p-3])==B.who)
						flag++;
					if(GetColor(B.fin[p-1])==B.who)
						flag++;
					if((p-2)/4+1 <8 && GetColor(B.fin[(p-2)+4]) == B.who )
						flag++;
					if((p-2)/4-1 >=0 && GetColor(B.fin[(p-2)-4]) == B.who )
						flag++;
					info[1]=flag;
				}
				if((!has_paw ||flag!=0) && p /4+2 <8 && B.fin[p+8] == FIN_X){
					has_paw =1;
					index =2;
					flag =0 ;
					if((p+8)%4+1 <4 && GetColor(B.fin[p+9])==B.who)
						flag++;
					if((p+8)/4-1>=0 && GetColor(B.fin[p+7])==B.who)
						flag++;
					if((p+8)/4+1 <8 && GetColor(B.fin[(p+8)+4]) == B.who )
						flag++;
					if((p+8)/4-1 >=0 && GetColor(B.fin[(p+8)-4]) == B.who )
						flag++;
					info[2]=flag;
				}
				if((!has_paw ||flag!=0) && p /4-2 >=0 && B.fin[p-8] == FIN_X){
					has_paw =1;
					index =3;
					flag =0 ;
					if((p-8)%4+1 <4 && GetColor(B.fin[p-7])==B.who)
						flag++;
					if((p-8)/4-1>=0 && GetColor(B.fin[p-9])==B.who)
						flag++;
					if((p-8)/4+1 <8 && GetColor(B.fin[(p-8)+4]) == B.who )
						flag++;
					if((p-8)/4-1 >=0 && GetColor(B.fin[(p-8)-4]) == B.who )
						flag++;
					info[3]=flag;
				}
				if(has_paw)
					break;
			}
		}
		if(has_paw){
			if(flag==0){
				if(index ==0)
					return MOV(p+2,p+2);
				else if(index ==1)
					return MOV(p-2,p-2);
				else if(index ==2)
					return MOV(p+8,p+8);
				else if(index ==3)
					return MOV(p-8,p-8);
			}else{
				int min =4 ,min_index =4;
				for(int i=0;i<4;i++){
					if(info[i] < min  && info[i] >=0){
						min = info[i];
						min_index = i;
					}
				}
				if(min_index ==0 )
					return MOV(p+2,p+2);
				else if(min_index==1)
					return MOV(p-2,p-2);
				else if(min_index ==2)
					return MOV(p+8,p+8);
				else if(min_index ==3)
					return MOV(p-8,p-8);
			}
		}
		flag = 1;
		for(p = 0; p<32;p++){
			if(B.fin[p]==FIN_X){
				flag = 0;
				if (p % 4 + 2 < 4 && GetColor(B.fin[p + 2]) == B.who)
					flag = 1;
				else if (p % 4 - 2 >= 0 && GetColor(B.fin[p - 2]) == B.who)
					flag = 1;
				else if (p / 4 + 2 < 8 && GetColor(B.fin[p + 8]) == B.who)
					flag = 1;
				else if (p / 4 - 2 >= 0 && GetColor(B.fin[p - 8]) == B.who)
					flag = 1;
				else if (p % 4 + 1 < 4 && GetColor(B.fin[p + 1]) == B.who)
					flag = 1;
				else if (p % 4 - 1 >= 0 && GetColor(B.fin[p - 1]) == B.who)
					flag = 1;
				else if (p / 4 + 1 < 8 && GetColor(B.fin[p + 4]) == B.who)
					flag = 1;
				else if (p / 4 - 1 >= 0 && GetColor(B.fin[p - 4]) == B.who)
					flag = 1;
				if (flag == 0)
					break;
			}
		}
		if(flag==0){
			return MOV(p,p);
		}else{
			int max = 1;
			for (int max = 1; max <= 6; max++){
				for (int p = 0; p < 32; p++){
					if (B.fin[p] == FIN_X){
						flag = 0;
						if (p % 4 + 1 < 4 && GetColor(B.fin[p + 1]) == B.who && B.fin[p + 1] % 7 == max)
							flag = 1;
						else if (p % 4 - 1 >= 0 && GetColor(B.fin[p - 1]) == B.who && B.fin[p - 1] % 7 == max)
							flag = 1;
						else if (p / 4 + 1 < 8 && GetColor(B.fin[p + 4]) == B.who && B.fin[p + 4] % 7 == max)
							flag = 1;
						else if (p / 4 - 1 >= 0 && GetColor(B.fin[p - 4]) == B.who && B.fin[p - 4] % 7 == max)
							flag = 1;
						if (flag)
							return MOV(p, p);
					}
				}
			}
		}
	}else if(ours ==0){
		for (p = 0; p <32 ;p++){
			if(B.fin[p] /7 == (B.who^1) && B.fin[p]%7 == LVL_C){
				flag =0;
				if(p%4 +1 < 4 && B.fin[p+1] == FIN_X){
					flag = 0;
					if ((p + 1) % 4 - 1 >= 0 && GetColor(B.fin[(p + 1) - 1]) == 1 - B.who && B.fin[(p + 1) - 1] % 7 < 5)
						flag = 1;
					else if ((p + 1) % 4 + 1 >= 0 && GetColor(B.fin[(p + 1) + 1]) == 1 - B.who && B.fin[(p + 1) + 1] % 7 < 5)
						flag = 1;
					else if ((p + 1) / 4 + 1 < 8 && GetColor(B.fin[(p + 1) + 4]) == 1 - B.who && B.fin[(p + 1) - 4] % 7 < 5)
						flag = 1;
					else if ((p + 1) / 4 - 1 >= 0 && GetColor(B.fin[(p + 1) - 4]) == 1 - B.who && B.fin[(p + 1) + 4] % 7 < 5)
						flag = 1;
					if (flag == 0)
						return MOV(p + 1, p + 1);

				}else if(p%4-1 >=0 && B.fin[p-1] == FIN_X){
					flag = 0;
					if ((p - 1) % 4 - 1 >= 0 && GetColor(B.fin[(p - 1) - 1]) == 1 - B.who && B.fin[(p - 1) - 1] % 7 < 5)
						flag = 1;
					else if ((p - 1) % 4 + 1 >= 0 && GetColor(B.fin[(p - 1) + 1]) == 1 - B.who && B.fin[(p - 1) + 1] % 7 < 5)
						flag = 1;
					else if ((p - 1) / 4 + 1 < 8 && GetColor(B.fin[(p - 1) + 4]) == 1 - B.who && B.fin[(p - 1) - 4] % 7 < 5)
						flag = 1;
					else if ((p - 1) / 4 - 1 >= 0 && GetColor(B.fin[(p - 1) - 4]) == 1 - B.who && B.fin[(p - 1) + 4] % 7 < 5)
						flag = 1;
					if (flag == 0)
						return MOV(p - 1, p - 1);
				}else if(p/4+1 <8 && B.fin[p+4] == FIN_X){
					flag = 0;
					if ((p + 4) % 4 + 1 < 4 && GetColor(B.fin[(p + 4) + 1]) == 1 - B.who && B.fin[(p + 4) + 1] % 7 < 5)
						flag = 1;
					else if ((p + 4) % 4 - 1 >= 0 && GetColor(B.fin[(p + 4) - 1]) == 1 - B.who && B.fin[(p + 4) - 1] % 7 < 5)
						flag = 1;
					else if ((p + 4) / 4 + 1 < 8 && GetColor(B.fin[(p + 4) + 4]) == 1 - B.who && B.fin[(p + 4) + 4] % 7 < 5)
						flag = 1;
					else if ((p + 4) / 4 - 1 >= 0 && GetColor(B.fin[(p + 4) - 4]) == 1 - B.who && B.fin[(p + 4) - 4] % 7 < 5)
						flag = 1;
					if (flag == 0)
						return MOV(p + 4, p + 4);
				}else if(p/4-1 >=0 && B.fin[p-4] == FIN_X){
					flag = 0;
					if ((p - 4) % 4 + 1 < 4 && GetColor(B.fin[(p - 4) + 1]) == 1 - B.who && B.fin[(p - 4) + 1] % 7 < 5)
						flag = 1;
					else if ((p - 4) % 4 - 1 >= 0 && GetColor(B.fin[(p - 4) - 1]) == 1 - B.who && B.fin[(p - 4) - 1] % 7 < 5)
						flag = 1;
					else if ((p - 4) / 4 + 1 < 8 && GetColor(B.fin[(p - 4) + 4]) == 1 - B.who && B.fin[(p - 4) + 4] % 7 < 5)
						flag = 1;
					else if ((p - 4) / 4 - 1 >= 0 && GetColor(B.fin[(p - 4) - 4]) == 1 - B.who && B.fin[(p - 4) - 4] % 7 < 5)
						flag = 1;
					if (flag == 0)
						return MOV(p - 4, p - 4);
				}

			}
		}
		int max =0 ;
		has_paw =0;
		for(int max =0; max<= 6 && has_paw ==0 ; max++){
			for (p = 0; p < 32; p++){
				if (B.fin[p] / 7 == 1 - B.who && B.fin[p] % 7 == max){
					if (p % 4 + 2 < 4 && B.fin[p + 2] == FIN_X){
						has_paw = 1;
						index = 0;
						flag = 0;
						if (p % 4 + 3 < 4 && GetColor(B.fin[p + 3]) == 1 - B.who)
							flag++;
						if (GetColor(B.fin[p + 1]) == 1 - B.who)
							flag++;
						if ((p + 2) / 4 + 1 < 8 && GetColor(B.fin[(p + 2) + 4]) == 1 - B.who)
							flag++;
						if ((p + 2) / 4 - 1 >= 0 && GetColor(B.fin[(p + 2) - 4]) == 1 - B.who)
							flag++;
						info[0] = flag;
					}
					if ((!has_paw || flag != 0) && p % 4 - 2 >= 0 && B.fin[p - 2] == FIN_X){
						has_paw = 1;
						index = 1;
						flag = 0;
						if (p % 4 - 3 >= 0 && GetColor(B.fin[p - 3]) == 1 - B.who)
							flag++;
						if (GetColor(B.fin[p - 1]) == 1 - B.who)
							flag++;
						if ((p - 2) / 4 + 1 < 8 && GetColor(B.fin[(p - 2) + 4]) == 1 - B.who)
							flag++;
						if ((p - 2) / 4 - 1 >= 0 && GetColor(B.fin[(p - 2) - 4]) == 1 - B.who)
							flag++;
						info[1] = flag;
					}
					if ((!has_paw || flag != 0) && p / 4 + 2 < 8 && B.fin[p + 8] == FIN_X){
						has_paw = 1;
						index = 2;
						flag = 0;
						if ((p + 8) % 4 + 1 < 4 && GetColor(B.fin[(p + 8) + 1]) == 1 - B.who)
							flag++;
						if ((p + 8) % 4 - 1 >= 0 && GetColor(B.fin[(p + 8) - 1]) == 1 - B.who)
							flag++;
						if ((p + 8) / 4 + 1 < 8 && GetColor(B.fin[(p + 8) + 4]) == 1 - B.who)
							flag++;
						if ((p + 8) / 4 - 1 >= 0 && GetColor(B.fin[(p + 8) - 4]) == 1 - B.who)
							flag++;
						info[2] = flag;
					}
					if ((!has_paw || flag != 0) && p / 4 - 2 >= 0 && B.fin[p - 8] == FIN_X){
						has_paw = 1;
						index = 3;
						flag = 0;
						if ((p - 8) % 4 + 1 < 4 && GetColor(B.fin[(p - 8) + 1]) == 1 - B.who)
							flag++;
						if ((p - 8) % 4 - 1 >= 0 && GetColor(B.fin[(p - 8) - 1]) == 1 - B.who)
							flag++;
						if ((p - 8) / 4 + 1 < 8 && GetColor(B.fin[(p - 8) + 4]) == 1 - B.who)
							flag++;
						if ((p - 8) / 4 - 1 >= 0 && GetColor(B.fin[(p - 8) - 4]) == 1 - B.who)
							flag++;
						info[3] = flag;
					}
					if (has_paw && flag == 0)
						break;
					else
						has_paw = 0;
				}
			}
			if (has_paw){
				if (index == 0)
					return MOV(p + 2, p + 2);
				else if (index == 1)
					return MOV(p - 2, p - 2);
				else if (index == 2)
					return MOV(p + 8, p + 8);
				else if (index == 3)
					return MOV(p - 8, p - 8);
			}
			flag = 1;
			for (p = 0; p < 32; p++){
				if (B.fin[p] == FIN_X){
					flag = 0;
					if (p % 4 + 2 < 4 && GetColor(B.fin[p + 2]) == 1 - B.who)
						flag = 1;
					else if (p % 4 - 2 >= 0 && GetColor(B.fin[p - 2]) == 1 - B.who)
						flag = 1;
					else if (p / 4 + 2 < 8 && GetColor(B.fin[p + 8]) == 1 - B.who)
						flag = 1;
					else if (p / 4 - 2 >= 0 && GetColor(B.fin[p - 8]) == 1 - B.who)
						flag = 1;
					else if (p % 4 + 1 < 4 && GetColor(B.fin[p + 1]) == 1 - B.who)
						flag = 1;
					else if (p % 4 - 1 >= 0 && GetColor(B.fin[p - 1]) == 1 - B.who)
						flag = 1;
					else if (p / 4 + 1 < 8 && GetColor(B.fin[p + 4]) == 1 - B.who)
						flag = 1;
					else if (p / 4 - 1 >= 0 && GetColor(B.fin[p - 4]) == 1 - B.who)
						flag = 1;
					if (flag == 0)
						break;
				}
			}
			if (flag == 0){
				return MOV(p, p);
			}
			else{//翻小棋附近(兵除外)
				int max = 5;
				for (int max = 5; max >= 0; max--){
					for (int p = 0; p < 32; p++){
						if (B.fin[p] == FIN_X){
							flag = 0;
							if (p % 4 + 1 < 4 && GetColor(B.fin[p + 1]) == 1 - B.who && B.fin[p + 1] % 7 == max)
								flag = 1;
							else if (p % 4 - 1 >= 0 && GetColor(B.fin[p - 1]) == 1 - B.who && B.fin[p - 1] % 7 == max)
								flag = 1;
							else if (p / 4 + 1 < 8 && GetColor(B.fin[p + 4]) == 1 - B.who && B.fin[p + 4] % 7 == max)
								flag = 1;
							else if (p / 4 - 1 >= 0 && GetColor(B.fin[p - 4]) == 1 - B.who && B.fin[p - 4] % 7 == max)
								flag = 1;
							if (flag)
								return MOV(p, p);
						}
					}
				}
			}
		}
	}
	return MOV();
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

	int dark = B.dark_cnt[0]+B.dark_cnt[1];
	int open_empty  = 32-dark;
	int ours = B.total_cnt[B.who] - B.cnt[B.who];
	int their = B.total_cnt[B.who^1]- B.cnt[B.who^1];
	if(open_empty-ours-their==0 && open_empty !=0 && (ours == 0 || their ==0)){
		return SearchSpecial(B,dark,open_empty,ours,their);
	}
	int ef, of , es,os, esm, osm;
	ef = of = es = os = esm = osm = -1;
	MOVLST lst;
	B.MoveGen(lst);
	// special case winning
	int light_cnt[2][7];
	for(int i =0 ; i< 14 ;i++) light_cnt[B.who!=GetColor(FIN(i))][i%7] = B.total_cnt[i]-B.cnt[i];
	if( open_empty == 32 && ours >= their && their  < 3){
		if(light_cnt[0][6]==0 ){
			for(int i= 5; i >=0 ;i--){
				if(light_cnt[1][6]==0)
					value[i] =0;
				else
					break;
			}
			if(light_cnt[1][0]==0)
				value[6]=0;
			else
				value[6]= 6000;
		}
		for(int i=0;i <7 ;i++){
			if(light_cnt[1][i] !=0 ){
				esm = i;
				if(es==-1){
					if(ef==-1)
						ef = i;
					else
						es = i;
					if(light_cnt[1][i] > 1)
						es = i;
				}
			}
			if(light_cnt[0][i] !=0 ){
				osm = i;
				if(os==-1){
					if(of==-1)
						of = i;
					else
						os = i;
					if(light_cnt[0][i] > 1)
						os = i;
				}
			}
		}
		if(their == 1){
			for(int i=0;i < lst.num;i++)
				if(GetColor(B.fin[lst.mov[i].ed]) == (B.who^1)) // dicect access
					return lst.mov[i];

			if(of <= ef || (ef==0 && light_cnt[0][6])){
				if(ef==0 && light_cnt[0][6]){
					of =6;
				}
				if(ef == 6 &&	light_cnt[0][0]){
					for(int i =0; i< 5 ;i++){
						if(light_cnt[0][i] !=0){
							of = i;
							break;
						}
					}
				}
				int eps ,ops ,ops1,ops2 ,dist,dist2;
				eps = ops = dist = dist2 =0 ;
				ops1= ops2 =-1;
				for (int i =0; i< 32;i ++){
					if(GetColor(FIN(i)) == (B.who^1))
							eps =i;
					else if(GetColor(B.fin[i])==B.who && GetLevel(B.fin[i])== of){
						if(ops1 ==-1)
							ops1 =i;
						else
							ops2 =i;
					}
				}
				dist = abs(ops1 %4 - eps %4 )+ abs(ops1/4 - eps/4);
				if(ops2 !=-1){
					dist2 = abs(ops2 %4 - eps %4) + abs(ops2/4-eps/4);
					if(dist < dist2){
						dist = dist2;
						ops = ops2;
					}else{
						ops = ops1;
					}
				}else{
					ops = ops1;
				}
				int x = check_connect(B,ops,eps);
				if(x!=-1 && (dist%2==1 || dist>3))
					return MOV(ops,ADJ[ops][x]);
				else if(x==-1){
					for(int i=0 ;i < 32;i++){
						if(B.fin[i] != FIN_E && GetColor(B.fin[i])==B.who && i!=ops){
							for(int j=0; j<4 ;j++){
									if(ADJ[i][j] !=-1){
										if(B.fin[ADJ[i][j]]== FIN_E && abs(ADJ[i][j] %4 -ops%4)+abs(ADJ[i][j]/4-ops/4) >abs(i %4 -ops%4)+abs(i/4-ops/4))
											return MOV(i,ADJ[i][j]);
									}
							}
						}
					}
				}else{
					if( ours !=1){
						for (int i=0;i < 32;i++){
							if(B.fin[i] != FIN_E && GetColor(B.fin[i])== B.who && i != ops){
								for (int j =0; j < 4 ;j++){
									if(ADJ[i][j] != -1){
										if(B.fin[ADJ[i][j]] == FIN_E && abs(ADJ[i][j] %4 -eps%4) + abs(ADJ[i][j] /4 -eps/4) < abs(i%4-eps%4) +abs(i/4-eps/4) && abs(ADJ[i][j] %4 -ops %4) + abs(ADJ[i][j/4]-ops/4) > abs(i%4-ops%4) + abs(i/4 -ops/4))
											return MOV(i,ADJ[i][j]);
									}
								}
							}
						}
					}
				}
			}
		}else if (their==2){
			if(of < ef){
				if(os <= ef){
					for(int i=0 ;i <lst.num;i++){
						if(GetColor(B.fin[lst.mov[i].ed])== (B.who^1))
							return lst.mov[i];
					}
					int eps ,eps1,eps2,ops,ops1,ops2,dist,dist2;
					eps=ops=dist=dist2=0;
					eps1=eps2=ops1=ops2=-1;
					for(int i=0;i<32;i++){
						if(GetColor(B.fin[i])==(B.who^1)){
								if(eps1==-1)
									eps1 =i;
								else
									eps2= i;
						}else if(GetColor(B.fin[i])==B.who && GetLevel(B.fin[i])==of){
							if(ops1==-1)
								ops1 =i;
						}else if(GetColor(B.fin[i])==B.who && GetLevel(B.fin[i])==os){
							if(ops2==-1)
								ops2=i;
						}
					}
					eps =eps1;
					dist = abs(ops%4-eps%4)+abs(ops1/4-eps/4);
					dist2 = abs(ops2%4-eps%4)+abs(ops2/4-eps/4);
					int x = check_connect(B,ops1,eps1);
					if(x!=-1 && dist%2 ==1)
						return MOV(ops1,ADJ[ops1][x]);
					else{
						x= check_connect(B,ops2,eps1);
						if(x !=-1 && (dist2%2 ==1 || dist2 >3)){
							return MOV(ops2,ADJ[ops2][x]);
						}
					}
					if(x==-1){
						for(int i=0;i<32;i++){
							if(B.fin[i] != FIN_E && GetColor(B.fin[i])== B.who && i !=ops){
								for(int j=0;j<4 ;j++){
									if (ADJ[i][j] !=-1) {
										if(B.fin[ADJ[i][j]] == FIN_E && abs(ADJ[i][j]%4 - ops %4)+abs(ADJ[i][j]/4 -ops/4)> abs(i %4 - ops %4)+abs(i/4-ops/4) )
											return MOV(i,ADJ[i][j]);
									}
								}
							}
						}
					}else{

					}
				}
			}
			else if(of == ef){

			}
		}
	}
	SCORE alpha = -INF;
	SCORE beta = INF;
	SCORE m = -INF;

	MOV BestFlip = MOV();
	SCORE BestFlipScore = -INF;
/*
	for (p =0 ; p < 32 ; p++){

		SCORE sum_p = 0;

		if(B.fin[p]==FIN_X){
			for(int i =0; i< 14; i ++){
				if(TimesUp()) break;
				if(B.cnt[i]==0) continue;
					double probo = (double ) B.cnt[i] / (B.dark_cnt[0]+B.dark_cnt[1]);
					BOARD N(B);
					N.Flip(p,FIN(i));
					SCORE t = -nega_scout2(N,-INF,INF,6,0);
					sum_p += probo* t;
			}
			cerr << "score of flipping " <<p <<" is "<<sum_p<<endl;
			if(sum_p > BestFlipScore){
				BestFlipScore = sum_p;
				BestFlip = MOV(p,p);
			}
		}

	}

*/
	SCORE origin_nega = B.Eval();
	SCORE nega;
	MOV OldBestMove;
	if(lst.num!=0){
		 nega = Search_Max(B,6);
		OldBestMove = BestMove;
		for (int tt =7 ; tt <= search_depth ; tt++){
			SCORE tmp_nega = Search_Max(B,tt) ;
			if(tmp_nega == nega || tmp_nega < origin_nega ||  nega== WIN )
				break;
			if(TimesUp() )
				break;
			nega = tmp_nega;
			OldBestMove = BestMove;
		}
		BOARD N(B);
		if(nega > origin_nega && OldBestMove.st !=-1) return OldBestMove;
	}
	MOVLST en;
	BOARD N(B);
	N.who^=1;
	N.MoveGen(en);
	if(dark==0)
		return OldBestMove;
	if(lst.num!=0){
		for(int i= 0 ;i < en.num; i++){
			if(en.mov[i].ed == OldBestMove.st){
				for (int j=0;j<4;j++){
					if(ADJ[OldBestMove.st][j]!=-1){
						if (GetColor(B.fin[ADJ[OldBestMove.ed][j]]) == GetColor(B.fin[OldBestMove.st]) && GetLevel(B.fin[ADJ[OldBestMove.ed][j]])>GetLevel(B.fin[en.mov[i].st]))
							break;
					}
					if(j==3)
						return OldBestMove;

				}
			}
		}
	}
	int flip_map[32] ={0};
	int kind[2][7] = {0};
	for(int i=0;i< 32;i++){
		if(B.fin[i] == FIN_X){
			int count,o_n,e_n,ep_n;
			count = o_n = e_n = ep_n =0;
			int our[4] ={0};
			int ene[4] ={0};
			int safe_num = dark;
			for(int j =0; j < 4; j++){
					if(ADJ[i][j] != -1){
						if(ADJ[i][j]!=-1){
							if (ADJ[i][j]!=-1) {
								if (B.fin[ADJ[i][j]]==FIN_X) {
									count++;
								}else if(B.fin[ADJ[i][j]]==FIN_E){
									ep_n++;
								}else if(GetColor(B.fin[ADJ[i][j]]) ==B.who){
									our[o_n++] = GetLevel(B.fin[ADJ[i][j]]);
								}else{
									ene[e_n++] = GetLevel(B.fin[ADJ[i][j]]);
									e_n++;
								}
							}
						}
					}
			}
			if(o_n ==0 && e_n ==0 && ep_n==0){
				flip_map[i] = 800 ;
				for(int j=0;j< 4; j++){
					if(POW[i][j]!=-1){
						if(GetColor(B.fin[POW[i][j]]) ==B.who && GetLevel(B.fin[POW[i][j]]) == LVL_C)
							flip_map[i] = 950;
						if(GetColor(B.fin[POW[i][j]]) ==(B.who^1) && GetLevel(B.fin[POW[i][j]]) == LVL_C)
								flip_map[i] =0;
					}
				}
			}else if(o_n ==0 && e_n ==0){
				flip_map[i] = 800 -50 * ep_n;
			}else{
				int omink,omaxk,emink,emaxk;
				omink=emink =-1;
				omaxk=emaxk = 7;
				for(int j =0;j < o_n;j++){
					if(omink < our[j])
						omink = our[j];
					if(omaxk > our[j])
						omaxk = our[j];
				}
				for(int j=0 ; j< e_n;j++){
					if(emink < ene[j])
						emink = ene[j];
					if(emaxk >ene[j])
						emaxk = ene[j];
				}
				for(int j=0; j< omink && omink !=-1 ;j++){
					if(j==5)
						continue;
					safe_num-=kind[1][j];
				}
				for(int j=emaxk;j<=6 && emaxk !=7 ;j++){
					safe_num -= kind[1][j];
				}
				if(omaxk ==0 && omink <6)
					safe_num = safe_num -kind[1][6];
				if(emaxk ==0 && emink ==0)
					safe_num = safe_num +kind[0][6];
				if(emink ==6 && emaxk !=0)
					safe_num = safe_num -kind[0][0];
				if(omink ==6 && omaxk ==6)
					safe_num = safe_num +kind[1][0];
				flip_map[i] = (int ) ((double) safe_num / dark * 1000);
			}
		}else{
			flip_map[i] = -1;
		}
	}
	int tmp_max =0 ;
	int max_i =0 ;
	for(int i=0;i<32;i++){
		if(flip_map[i] >= tmp_max){
			tmp_max = flip_map[i];
			max_i = i;
		}
	}
	cerr << "flip " << tmp_max <<endl;
	return MOV(max_i,max_i);
	/*if(BestMove.st !=-1){
		if(nega>= B.Eval() || nega >BestFlipScore ){
				cerr << "find best move with value "<< nega << " move : " << (BestMove.st)<<" "<<(BestMove.ed) << endl;
				return BestMove;
		}else{
				cerr << "current best value "<<B.Eval()<< " is no less than nega_scout value "<< nega <<", fliping " << endl;
		}
	}

	if(BestFlipScore==-INF){
		if(BestMove.st!=-1)
			return BestMove;
		else
			cerr <<"GGGGGGGGG"<<endl;
	}


	return BestFlip;*/
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
