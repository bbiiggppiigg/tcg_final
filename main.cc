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
#include"anqi.hh"
#include"Protocol.h"
#include"ClientSocket.h"
#define MAX_HASH 999989
//#define MAX_HASH 99999989
trans_node trans_table [MAX_HASH];
#ifdef _WINDOWS
#include<windows.h>
#else
#include<ctime>
#endif
SCORE SearchMax(const BOARD&,int,int);
SCORE SearchMin(const BOARD&,int,int);

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


SCORE nega_scout(const BOARD & B , int alpha, int beta, int depth,int is_max,int cut){
	trans_node *tn = &trans_table[B.Key%MAX_HASH];
	SCORE m = -INF;
	SCORE n  = beta;

	if(tn->position == B.Key && tn->check == B.Check){
		if(cut <=tn->search_depth ){
			if(tn->is_exact){
				return tn->best_value;
			}else{
				m = tn->best_value;
			}
		}
	}
	if(B.ChkLose()){
		if(cut >= tn->search_depth){
			tn->search_depth = cut;
			tn->best_value = -is_max * WIN;
			tn->is_exact = true;
			tn->position = B.Key;
			tn->check = B.Check;
		}
		return -is_max * WIN;
	}
	MOVLST lst;

	if(cut==depth || TimesUp() || B.MoveGen(lst)==0 ){
		if(cut >= tn->search_depth){
			tn->search_depth = cut;
			tn->best_value = -is_max * WIN;
			tn->is_exact = true;
			tn->position = B.Key;
			tn->check = B.Check;
		}
		return is_max* B.Eval();
	}



	for (int i = 0 ; i< lst.num;i++){
		BOARD N (B);
		N.Move(lst.mov[i]);

		SCORE t=  -nega_scout(N,-n,-get_max(alpha,m),depth,-is_max,cut+1); // null window search
		if(t>m){ // if failed high
			if(n==beta || depth-cut < 3 || t>= beta){
				m = t;
			}else{
				m = -nega_scout(N,-beta,-t,depth,-is_max,cut+1); //research
			}
			BestMove = lst.mov[i];
		}
		if(m>=beta){ //cut off
			tn->search_depth = cut;
			tn->best_value = m;
			tn->is_exact = false;
			tn->position = B.Key;
			tn->check = B.Check;
			return m;
		}
		//if(get_max(alpha,m)!=alpha)
		n=get_max(alpha,m)+1;
	}
	tn->search_depth = cut;
	tn->best_value = m;
	tn->is_exact = true;
	tn->position = B.Key;
	tn->check = B.Check;

	return m;
}


// �@�ӭ��q�����誺�f������
SCORE Eval(const BOARD &B) {
	int cnt[2]={0,0};
	for(POS p=0;p<32;p++){const CLR c=GetColor(B.fin[p]);if(c!=-1)cnt[c]++;}
	for(int i=0;i<14;i++)cnt[GetColor(FIN(i))]+=B.cnt[i];
	return cnt[B.who]-cnt[B.who^1];
}


// dep=�{�b�b�ĴX�h
// cut=�٭n�A���X�h
SCORE SearchMax(const BOARD &B,int dep,int cut) {
	if(B.ChkLose())return -WIN;

	MOVLST lst;
	if(cut==0||TimesUp()||B.MoveGen(lst)==0)return +Eval(B);

	SCORE ret=-INF;
	for(int i=0;i<lst.num;i++) {
		BOARD N(B);
		N.Move(lst.mov[i]);
		const SCORE tmp=SearchMin(N,dep+1,cut-1);
		if(tmp>ret){ret=tmp;if(dep==0)BestMove=lst.mov[i];}
	}
	return ret;
}

SCORE SearchMin(const BOARD &B,int dep,int cut) {
	if(B.ChkLose())return +WIN;

	MOVLST lst;
	if(cut==0||TimesUp()||B.MoveGen(lst)==0)return -Eval(B);

	SCORE ret=+INF;
	for(int i=0;i<lst.num;i++) {
		BOARD N(B);
		N.Move(lst.mov[i]);
		const SCORE tmp=SearchMax(N,dep+1,cut-1);
		if(tmp<ret){ret=tmp;}
	}
	return ret;
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

	// �s�C���H�H��½�l
	if(B.who==-1){p=rand()%32;printf("%d\n",p);return MOV(p,p);}

	// �Y�j�X�Ӫ����G�|���{�b�n�N�ηj�X�Ӫ����k
	//if(SearchMax(B,0,5)>Eval(B))return BestMove;
	if(nega_scout(B,-INF,INF,6,1,0) > B.Eval()) return BestMove;
	// �_�h�H�K½�@�Ӧa�� ���p�ߥi���w�g�S�a��½�F
	for(p=0;p<32;p++)if(B.fin[p]==FIN_X)c++;
	if(c==0)return BestMove;
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
	    if(!B.ChkLose())Output(Play(B));
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
