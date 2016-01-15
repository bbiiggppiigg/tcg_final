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
#include"board.h"
#ifdef _WINDOWS
#include<windows.h>
#endif
static const unsigned int zobrist_table[32][15] = {{1063537466,1367797081,1900582879,1424681875,185609075,1385468081,396852946,1970739487,1578270328,271394952,79692036,1501736971,317968406,1155685906,1770918674},{1854290145,771781751,534661177,978822791,1349912317,1959064911,784683373,476373784,595151672,1882807225,1169492030,1882210866,1883904552,284914096,1810162309},{2144583961,657101279,1540283279,1773189215,1360567086,645141146,242307119,826754321,1040676957,1550795131,204743078,848109452,1326594625,900639221,1578643291},{97333152,1643230297,1131901259,1454314887,11435655,1073009002,1642112755,1676725688,1448222282,692238476,1533150333,1366378,1489878576,729902812,1049969620},{966275941,928401773,32419709,1564686472,1748277389,1426818669,1738967481,1721501144,222551177,1648602412,1226724890,1722215030,1483414944,1617305785,1357808416},{1524814690,1638135179,1397598913,268799905,1561893694,2054697777,1768494279,1909672673,1725510696,1039098584,792883884,863408753,763908892,1359506078,2140132513},{1003542388,200351578,54612950,904333381,1389364648,1461945105,1550974408,1070367970,203960871,586458285,1801939912,1389710990,840464158,1681157187,766498330},{1930517604,2026431552,1291936691,372810820,1621653441,1414418810,1640451027,1665350603,1393213270,1721225649,2034757653,1642279143,178241510,2112854652,2106033019},{1271480479,162639256,1878776608,2146388815,926511399,473158596,244578131,340947359,815892517,1022447124,104669774,395784725,1197018316,660031916,1403375457},{718410798,1177218552,745363653,1054803020,596851155,381246948,1673736035,593248192,2103273570,2138061370,553580339,1125598769,755064160,882466997,1100752397},{1921401121,1277040708,1271611238,212822122,1341132199,420509681,137526290,711951858,2143479969,1429660558,110472023,1277419553,1196408212,1143432223,1981698605},{1084572912,575736248,1985290401,1322346168,363782773,211122802,697948370,862574676,1777962282,2124609216,2096494643,2021268572,417077711,438464969,1264341126},{430617617,370398529,1880467897,507111980,1815936564,408239984,79158923,1131641368,1381294144,1112454138,1024066584,1553130230,796046325,327463465,1825352641},{1897939892,2101155953,907010803,1291639615,1822305429,75571689,970541646,1755145357,892639907,278159007,2094014777,1144350003,226957889,550283351,1547696275},{1809361461,1569633507,1141232401,1516512250,1685463154,122441701,588334881,1129634179,2006206973,679853664,1687528808,480150127,1787122710,1465100028,888674094},{196732973,1513744478,264675737,966478822,43255446,1144808236,1486028979,454235443,30725416,1005991432,559244793,1838796679,218619976,1416615,186728188},{867047449,1789930448,1410112360,128906228,1863457820,234072892,2020538187,1026398898,2097625982,1708330322,51361464,2091183001,795331005,1189982107,514067838},{611441385,786106800,777591256,1538247597,1919220193,1129405811,315509644,633462265,1523849676,451530410,1811876019,882136873,1994809270,244703926,307700277},{377933563,1820249162,2023114219,1372095782,1134406588,611706450,946086961,914631139,517607947,2127994879,1017274215,1210417738,378334535,2116934625,1958662526},{464249619,835256982,63496035,2025971333,2146970546,2113729728,1781049822,329802821,340719640,1283586578,1766382331,773900989,1792955891,719125333,301506415},{1504393632,2018796893,1825241698,33320891,1678466817,646606327,1245284069,113724021,99175117,388881347,1136061208,503617379,1064236026,223593119,1980652430},{681378863,1551744637,1130704891,674310734,869301119,1012656492,909758569,228702543,1955395718,1393582385,1482490513,1112779497,49924356,1560028962,778918111},{216379465,999853884,484690613,789659620,360294880,1719647267,1276695143,1906151224,522575822,1871207771,1638480529,759445422,1503893433,54403241,1674721512},{2123774602,954039027,1421018287,888711322,822423969,1274894891,1714086918,165706321,1887330535,2030835555,148087467,2119994643,1848777524,462957425,592188894},{1479521260,600668207,109930502,766010694,177270293,821996062,525512883,1842268117,581019973,594543302,247867223,1933625428,572538345,1925225855,1134835636},{1380265245,999617821,812147066,349677930,1521711318,1019369503,2066184802,1557395224,1610840132,63760795,33341712,2028405364,106056623,82235751,1304282036},{1702594123,279828986,96580772,1882881519,270667641,740677941,1758936375,183770023,541292175,756856533,942108950,628193319,1015503781,1519504558,437576182},{1358883546,267171777,2115233809,1290335425,1377620569,1647704876,1174222867,1936493386,1529668217,1606984882,1840567102,2056831926,1129914523,275497640,314092548},{438649910,77677219,1998446004,1237750148,192648947,1592996200,808506251,1435525988,2053989918,608926301,1464762952,1665888703,1847125382,612694242,368037929},{860569343,286585256,1980061018,1478935614,1495134120,1013001293,262378035,1003706954,818728693,1445416922,787193190,1856678810,115885113,2060910009,950778800},{313474273,784720220,1095661313,107414566,1430347282,914824056,1612480319,1860579940,1257667613,2085517917,71544685,2008162122,1327788202,1633735037,458856317},{384343442,29419518,532600216,699989616,810057846,1743379389,710511255,1553585465,2004730029,1626659620,1801407030,1023497804,583579358,660454057,2055848303}};
const int DEFAULTTIME = 15;
static const SCORE INF=1000001;
static const SCORE WIN=1000000;
#ifdef _WINDOWS
DWORD Tick;     // 開始時刻
int   TimeOut;  // 時限
#else
clock_t Tick;     // 開始時刻
clock_t TimeOut;  // 時限
#endif
MOV   BestMove; // 搜出來的最佳著法

static const char *tbl="KGMRNCPkgmrncpX-";
static const int tbl2[]={1,2,2,2,2,2,5};
static const char *nam[16]={
	"帥","仕","相","硨","傌","炮","兵",
	"將","士","象","車","馬","砲","卒",
	"Ｏ","　"
};

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

CLR GetColor(const FIN f) {
	return f<FIN_X?f/7:-1;
}

LVL GetLevel(const FIN f) {
	assert(f<FIN_X);
	return LVL(f%7);
}

bool ChkEats(FIN fa,FIN fb) {
	if(fa>=FIN_X)return false;
	if(fb==FIN_X)return false;
	if(fb==FIN_E)return true ;
	if(GetColor(fb)==GetColor(fa))return false;

	const LVL la=GetLevel(fa);
	if(la==LVL_C)return true ;

	const LVL lb=GetLevel(fb);
	if(la==LVL_K)return lb!=LVL_P;
	if(la==LVL_P)return lb==LVL_P||lb==LVL_K;

	return la<=lb;
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
Board::Board(){
	position =0 ;

}
void Board::NewGame() {
	
	who=-1;
	for(POS p=0;p<32;p++)fin[p]=FIN_X;
	for(int i=0;i<14;i++)cnt[i]=tbl2[GetLevel(FIN(i))];
}

static inline FIN find(char c) {
	return FIN(strchr(tbl,c)-tbl);
}

static POS LoadGameConv(const char *s) {
	return (8-(s[1]-'0'))*4+(s[0]-'a');
}

static void LoadGameReplay(Board &brd,const char *cmd) {
	if(cmd[2]!='-')brd.Flip(LoadGameConv(cmd),find(cmd[3]));
	else brd.Move(MOV(LoadGameConv(cmd),LoadGameConv(cmd+3)));
}

static inline POS mkpos(int x,int y) {
	return x*4+y;
}

void Board::Init(int Board[32], int Piece[14], int Color) {
    for (int i = 0 ; i < 14; ++i) {
	cnt[i] = Piece[i];
    }
    for (int i = 0 ; i < 32; ++i) {
	switch(Board[i]) {
	    case  0: fin[i] = FIN_E;break;
	    case  1: fin[i] = FIN_K;cnt[FIN_K]--;break;
	    case  2: fin[i] = FIN_G;cnt[FIN_G]--;break;
	    case  3: fin[i] = FIN_M;cnt[FIN_M]--;break;
	    case  4: fin[i] = FIN_R;cnt[FIN_R]--;break;
	    case  5: fin[i] = FIN_N;cnt[FIN_N]--;break;
	    case  6: fin[i] = FIN_C;cnt[FIN_C]--;break;
	    case  7: fin[i] = FIN_P;cnt[FIN_P]--;break;
	    case  8: fin[i] = FIN_X;break;
	    case  9: fin[i] = FIN_k;cnt[FIN_k]--;break;
	    case 10: fin[i] = FIN_g;cnt[FIN_g]--;break;
	    case 11: fin[i] = FIN_m;cnt[FIN_m]--;break;
	    case 12: fin[i] = FIN_r;cnt[FIN_r]--;break;
	    case 13: fin[i] = FIN_n;cnt[FIN_n]--;break;
	    case 14: fin[i] = FIN_c;cnt[FIN_c]--;break;
	    case 15: fin[i] = FIN_p;cnt[FIN_p]--;break;
	}
    }
    who = Color;
}

void Board::Init(char Board[32], int Piece[14], int Color) {
    for (int i = 0 ; i < 14; ++i) {
	cnt[i] = Piece[i];
    }
    for (int i = 0 ; i < 32; ++i) {
	switch(Board[i]) {
	    case '-': fin[i] = FIN_E;break;
	    case 'K': fin[i] = FIN_K;cnt[FIN_K]--;break;
	    case 'G': fin[i] = FIN_G;cnt[FIN_G]--;break;
	    case 'M': fin[i] = FIN_M;cnt[FIN_M]--;break;
	    case 'R': fin[i] = FIN_R;cnt[FIN_R]--;break;
	    case 'N': fin[i] = FIN_N;cnt[FIN_N]--;break;
	    case 'C': fin[i] = FIN_C;cnt[FIN_C]--;break;
	    case 'P': fin[i] = FIN_P;cnt[FIN_P]--;break;
	    case 'X': fin[i] = FIN_X;break;
	    case 'k': fin[i] = FIN_k;cnt[FIN_k]--;break;
	    case 'g': fin[i] = FIN_g;cnt[FIN_g]--;break;
	    case 'm': fin[i] = FIN_m;cnt[FIN_m]--;break;
	    case 'r': fin[i] = FIN_r;cnt[FIN_r]--;break;
	    case 'n': fin[i] = FIN_n;cnt[FIN_n]--;break;
	    case 'c': fin[i] = FIN_c;cnt[FIN_c]--;break;
	    case 'p': fin[i] = FIN_p;cnt[FIN_p]--;break;
	}
    }
    who = Color;
}

int Board::LoadGame(const char *fn) {
	FILE *fp=fopen(fn,"r");
	assert(fp!=NULL);

	while(fgetc(fp)!='\n');

	while(fgetc(fp)!='\n');

	fscanf(fp," %*c");
	for(int i=0;i<14;i++)fscanf(fp,"%d",cnt+i);

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

#ifdef _WINDOWS
void Board::Display() const {
	HANDLE hErr=GetStdHandle(STD_ERROR_HANDLE);
	for(int i=0;i<8;i++) {
		SetConsoleTextAttribute(hErr,8);
		for(int j=0;j<4;j++)fprintf(stderr,"[%02d]",mkpos(i,j));
		if(i==2) {
			SetConsoleTextAttribute(hErr,12);
			fputs("  ",stderr);
			for(int j=0;j<7;j++)for(int k=0;k<cnt[j];k++)fputs(nam[j],stderr);
		}
		fputc('\n',stderr);
		for(int j=0;j<4;j++) {
			const FIN f=fin[mkpos(i,j)];
			const CLR c=GetColor(f);
			SetConsoleTextAttribute(hErr,(c!=-1?12-c*2:7));
			fprintf(stderr," %s ",nam[fin[mkpos(i,j)]]);
		}
		if(i==0) {
			SetConsoleTextAttribute(hErr,7);
			fputs("  輪到 ",stderr);
			if(who==0) {
				SetConsoleTextAttribute(hErr,12);
				fputs("紅方",stderr);
			} else if(who==1) {
				SetConsoleTextAttribute(hErr,10);
				fputs("黑方",stderr);
			} else {
				fputs("？？",stderr);
			}
		} else if(i==1) {
			SetConsoleTextAttribute(hErr,7);
			fputs("  尚未翻出：",stderr);
		} else if(i==2) {
			SetConsoleTextAttribute(hErr,10);
			fputs("  ",stderr);
			for(int j=7;j<14;j++)for(int k=0;k<cnt[j];k++)fputs(nam[j],stderr);
		}
		fputc('\n',stderr);
	}
	SetConsoleTextAttribute(hErr,7);
}
#else
void Board::Display() const {
	for(int i=0;i<8;i++) {
		for(int j=0;j<4;j++)fprintf(stderr,"[%02d]",mkpos(i,j));
		if(i==2) {
			fputs("  ",stderr);
			for(int j=0;j<7;j++)for(int k=0;k<cnt[j];k++)fputs(nam[j],stderr);
		}
		fputc('\n',stderr);
		for(int j=0;j<4;j++) {
			const FIN f=fin[mkpos(i,j)];
			const CLR c=GetColor(f);
			fprintf(stderr," %s ",nam[fin[mkpos(i,j)]]);
		}
		if(i==0) {
			fputs("  輪到 ",stderr);
			if(who==0) {
				fputs("紅方",stderr);
			} else if(who==1) {
				fputs("黑方",stderr);
			} else {
				fputs("？？",stderr);
			}
		} else if(i==1) {
			fputs("  尚未翻出：",stderr);
		} else if(i==2) {
			fputs("  ",stderr);
			for(int j=7;j<14;j++)for(int k=0;k<cnt[j];k++)fputs(nam[j],stderr);
		}
		fputc('\n',stderr);
	}
}
#endif


int Board::MoveGen(MOVLST &lst) const {
	if(who==-1)return false;
	lst.num=0;
	for(POS p=0;p<32;p++) {
		const FIN pf=fin[p];
		if(GetColor(pf)!=who)continue;
		const LVL pl=GetLevel(pf);
		for(int z=0;z<4;z++) {
			const POS q=ADJ[p][z];
			if(q==-1)continue;
			const FIN qf=fin[q];
			if(pl!=LVL_C){if(!ChkEats(pf,qf))continue;}
			else if(qf!=FIN_E)continue;
			lst.mov[lst.num++]=MOV(p,q);
		}
		if(pl!=LVL_C)continue;
		for(int z=0;z<4;z++) {
			int c=0;
			for(POS q=p;(q=ADJ[q][z])!=-1;) {
				const FIN qf=fin[q];
				if(qf==FIN_E||++c!=2)continue;
				if(qf!=FIN_X&&GetColor(qf)!=who)lst.mov[lst.num++]=MOV(p,q);
				break;
			}
		}
	}
	return lst.num;
}

bool Board::ChkLose() const {
	if(who==-1)return false;

	bool fDark=false;
	for(int i=0;i<14;i++) {
		if(cnt[i]==0)continue;
		if(GetColor(FIN(i))==who)return false;
		fDark=true;
	}

	bool fLive=false;
	for(POS p=0;p<32;p++)if(GetColor(fin[p])==who){fLive=true;break;}
	if(!fLive)return true;

	MOVLST lst;
	return !fDark&&MoveGen(lst)==0;
}

bool Board::ChkValid(MOV m) const {
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

void Board::Flip(POS p,FIN f) {
	if(f==FIN_X) {
		int i,sum=0;
		for(i=0;i<14;i++)    sum+=cnt[i];
		sum=rand()%sum;
		for(i=0;i<14;i++)if((sum-=cnt[i])<0)break;
		f=FIN(i);
	}
	fin[p]=f;
	cnt[f]--;
	if(who==-1)who=GetColor(f);
	who^=1;
}

void Board::Move(MOV m) {
	if(m.ed!=m.st) {
		fin[m.ed]=fin[m.st];
		fin[m.st]=FIN_E;
		who^=1;
	} else {
		Flip(m.st);
	}
}

void Board::DoMove(MOV m, FIN f) {
    if (m.ed!=m.st) {
	fin[m.ed]=fin[m.st];
	fin[m.st]=FIN_E;
	who^=1;
    }
    else {
	Flip(m.st, f);
    }
}

MOV Board::Play() {
#ifdef _WINDOWS
	Tick=GetTickCount();
	TimeOut = (DEFAULTTIME-3)*1000;
#else
	Tick=clock();
	TimeOut = (DEFAULTTIME-3)*CLOCKS_PER_SEC;
#endif
	POS p; int c=0;

	// 新遊戲？隨機翻子
	if(who==-1){p=rand()%32;printf("%d\n",p);return MOV(p,p);}

	// 若搜出來的結果會比現在好就用搜出來的走法
	if(SearchMax(0,5)>Eval())return BestMove;

	// 否則隨便翻一個地方 但小心可能已經沒地方翻了
	for(p=0;p<32;p++)if(fin[p]==FIN_X)c++;
	if(c==0)return BestMove;
	c=rand()%c;
	for(p=0;p<32;p++)if(fin[p]==FIN_X&&--c<0)break;
	return MOV(p,p);
}

// 一個重量不重質的審局函數
SCORE Board::Eval() {
	int cnt[2]={0,0};
	for(POS p=0;p<32;p++){const CLR c=GetColor(fin[p]);if(c!=-1)cnt[c]++;}
	for(int i=0;i<14;i++)cnt[GetColor(FIN(i))]+=cnt[i];
	return cnt[who]-cnt[who^1];
}

// dep=現在在第幾層
// cut=還要再走幾層
SCORE Board::SearchMax(int dep,int cut) {
	if(ChkLose())return -WIN;

	MOVLST lst;
	if(cut==0||TimesUp()||MoveGen(lst)==0)return +Eval();

	SCORE ret=-INF;
	for(int i=0;i<lst.num;i++) {
		Board N(*this);
		N.Move(lst.mov[i]);
		const SCORE tmp= N.SearchMin(dep+1,cut-1);
		if(tmp>ret){ret=tmp;if(dep==0)BestMove=lst.mov[i];}
	}
	return ret;
}

SCORE Board::SearchMin(int dep,int cut) {
	if(ChkLose())return +WIN;

	MOVLST lst;
	if(cut==0||TimesUp()||MoveGen(lst)==0)return -Eval();

	SCORE ret=+INF;
	for(int i=0;i<lst.num;i++) {
		Board N(*this);
		N.Move(lst.mov[i]);
		const SCORE tmp= N.SearchMax(dep+1,cut-1);
		if(tmp<ret){ret=tmp;}
	}
	return ret;
}
bool Board::TimesUp() {
#ifdef _WINDOWS
	return GetTickCount()-Tick>=TimeOut;
#else
	return clock() - Tick > TimeOut;
#endif
}

