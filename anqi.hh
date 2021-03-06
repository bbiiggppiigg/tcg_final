
#ifndef ANQI
#define ANQI
#define get_max(a,b) ((a) > (b) ? (a) : (b))
#define get_min(a,b) ((a) < (b) ? (a) : (b))
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// (color)
//  0 =   red
//  1 =   black
// -1 = non
typedef int CLR;
typedef  int SCORE;
typedef unsigned int U32;
typedef int POS;

static const SCORE INF=1000001;
static const SCORE WIN=1000000;
static const U32 zobrist_table[32][15] = {{1063537466,1367797081,1900582879,1424681875,185609075,1385468081,396852946,1970739487,1578270328,271394952,79692036,1501736971,317968406,1155685906,1770918674},{1854290145,771781751,534661177,978822791,1349912317,1959064911,784683373,476373784,595151672,1882807225,1169492030,1882210866,1883904552,284914096,1810162309},{2144583961,657101279,1540283279,1773189215,1360567086,645141146,242307119,826754321,1040676957,1550795131,204743078,848109452,1326594625,900639221,1578643291},{97333152,1643230297,1131901259,1454314887,11435655,1073009002,1642112755,1676725688,1448222282,692238476,1533150333,1366378,1489878576,729902812,1049969620},{966275941,928401773,32419709,1564686472,1748277389,1426818669,1738967481,1721501144,222551177,1648602412,1226724890,1722215030,1483414944,1617305785,1357808416},{1524814690,1638135179,1397598913,268799905,1561893694,2054697777,1768494279,1909672673,1725510696,1039098584,792883884,863408753,763908892,1359506078,2140132513},{1003542388,200351578,54612950,904333381,1389364648,1461945105,1550974408,1070367970,203960871,586458285,1801939912,1389710990,840464158,1681157187,766498330},{1930517604,2026431552,1291936691,372810820,1621653441,1414418810,1640451027,1665350603,1393213270,1721225649,2034757653,1642279143,178241510,2112854652,2106033019},{1271480479,162639256,1878776608,2146388815,926511399,473158596,244578131,340947359,815892517,1022447124,104669774,395784725,1197018316,660031916,1403375457},{718410798,1177218552,745363653,1054803020,596851155,381246948,1673736035,593248192,2103273570,2138061370,553580339,1125598769,755064160,882466997,1100752397},{1921401121,1277040708,1271611238,212822122,1341132199,420509681,137526290,711951858,2143479969,1429660558,110472023,1277419553,1196408212,1143432223,1981698605},{1084572912,575736248,1985290401,1322346168,363782773,211122802,697948370,862574676,1777962282,2124609216,2096494643,2021268572,417077711,438464969,1264341126},{430617617,370398529,1880467897,507111980,1815936564,408239984,79158923,1131641368,1381294144,1112454138,1024066584,1553130230,796046325,327463465,1825352641},{1897939892,2101155953,907010803,1291639615,1822305429,75571689,970541646,1755145357,892639907,278159007,2094014777,1144350003,226957889,550283351,1547696275},{1809361461,1569633507,1141232401,1516512250,1685463154,122441701,588334881,1129634179,2006206973,679853664,1687528808,480150127,1787122710,1465100028,888674094},{196732973,1513744478,264675737,966478822,43255446,1144808236,1486028979,454235443,30725416,1005991432,559244793,1838796679,218619976,1416615,186728188},{867047449,1789930448,1410112360,128906228,1863457820,234072892,2020538187,1026398898,2097625982,1708330322,51361464,2091183001,795331005,1189982107,514067838},{611441385,786106800,777591256,1538247597,1919220193,1129405811,315509644,633462265,1523849676,451530410,1811876019,882136873,1994809270,244703926,307700277},{377933563,1820249162,2023114219,1372095782,1134406588,611706450,946086961,914631139,517607947,2127994879,1017274215,1210417738,378334535,2116934625,1958662526},{464249619,835256982,63496035,2025971333,2146970546,2113729728,1781049822,329802821,340719640,1283586578,1766382331,773900989,1792955891,719125333,301506415},{1504393632,2018796893,1825241698,33320891,1678466817,646606327,1245284069,113724021,99175117,388881347,1136061208,503617379,1064236026,223593119,1980652430},{681378863,1551744637,1130704891,674310734,869301119,1012656492,909758569,228702543,1955395718,1393582385,1482490513,1112779497,49924356,1560028962,778918111},{216379465,999853884,484690613,789659620,360294880,1719647267,1276695143,1906151224,522575822,1871207771,1638480529,759445422,1503893433,54403241,1674721512},{2123774602,954039027,1421018287,888711322,822423969,1274894891,1714086918,165706321,1887330535,2030835555,148087467,2119994643,1848777524,462957425,592188894},{1479521260,600668207,109930502,766010694,177270293,821996062,525512883,1842268117,581019973,594543302,247867223,1933625428,572538345,1925225855,1134835636},{1380265245,999617821,812147066,349677930,1521711318,1019369503,2066184802,1557395224,1610840132,63760795,33341712,2028405364,106056623,82235751,1304282036},{1702594123,279828986,96580772,1882881519,270667641,740677941,1758936375,183770023,541292175,756856533,942108950,628193319,1015503781,1519504558,437576182},{1358883546,267171777,2115233809,1290335425,1377620569,1647704876,1174222867,1936493386,1529668217,1606984882,1840567102,2056831926,1129914523,275497640,314092548},{438649910,77677219,1998446004,1237750148,192648947,1592996200,808506251,1435525988,2053989918,608926301,1464762952,1665888703,1847125382,612694242,368037929},{860569343,286585256,1980061018,1478935614,1495134120,1013001293,262378035,1003706954,818728693,1445416922,787193190,1856678810,115885113,2060910009,950778800},{313474273,784720220,1095661313,107414566,1430347282,914824056,1612480319,1860579940,1257667613,2085517917,71544685,2008162122,1327788202,1633735037,458856317},{384343442,29419518,532600216,699989616,810057846,1743379389,710511255,1553585465,2004730029,1626659620,1801407030,1023497804,583579358,660454057,2055848303}};
static const U32 check_table[32][15] = {{92224486,1681226715,1935055426,984194614,1439828304,1348570932,889243686,1179931129,1238488705,1868158211,1924133137,2096877033,2005646361,1995066015,264849847},{1745261945,138375242,2095386240,572208527,676942023,2143702402,873124695,850988914,329588578,1034904833,1175471178,1442019893,1675385256,394418128,1850942654},{345075336,1475325252,945322102,944547808,815890432,987404529,1701778534,1604610192,583857918,1047244683,265416369,529378964,247498427,30238350,1409807758},{1451911355,417462624,465246819,415328206,1099305492,1225588903,1957034344,1016682156,1997100360,96347910,116653532,2090826260,1242035959,1357314073,1806326477},{2100264947,964258490,1380841168,2089221094,33814761,1389005319,1865153543,816801942,1274767570,1721686518,1190648348,974162090,326921902,1313237888,1899743397},{200409783,1032864385,1241399994,1406068553,884118683,943351588,38373715,701933905,1275468364,615029394,960231947,278726024,886451261,1492284388,362195803},{1456205423,1720903149,909467447,1778266030,787250911,679312010,1173884618,546509737,401591540,7910259,1951220546,2088426932,1718719556,717041895,1792385948},{1877511567,212197551,1581385637,1090785787,1916311317,1632050760,68500189,231441731,748288200,815540568,1549691222,978697338,1388907393,219311261,882425375},{401211443,62070921,1696400452,1409499192,560809887,226044126,225054139,766211806,1409876030,451875212,1164512292,1939616533,333308671,1281482121,756511884},{1592044148,1987237463,1834362497,855250747,1091255458,1220137226,525012179,2016870577,1661903391,1437979655,343098247,458645134,1129958055,1007139964,535269294},{462026975,2133984920,760161893,660719648,77185299,173197505,1090124850,1545361393,1221705333,1110382764,570222118,1651104312,326485250,416878665,1388066141},{1112774426,2112179706,1499633832,1477733232,614052669,1724284048,1897662118,1727575629,1384689163,190480002,1646759584,319085752,607567305,98953650,961652772},{536211682,1268356562,1346057412,1594185986,1477886730,1046409908,1267738473,1695253824,1465475219,752058290,1902417435,45809862,1127205008,1981319269,1151523701},{536215943,1339971189,234767434,808803699,2139767230,1306681948,1235725814,529405761,697875606,1787113775,1314929483,265609504,1627915262,1430145654,1821029554},{106777034,1452765193,1883016008,383540617,1568725272,908912285,1037592884,1256387748,2049663332,906439397,277953561,788567502,1332420477,31486023,904611399},{1767045880,1188750797,1294277138,1053997903,2097635265,1864349703,191564944,554026955,41939293,499061235,1798535110,2147262245,573863880,581172483,1010295225},{2026133393,575745672,2143679569,489370464,2134504085,895833460,265113103,1874838243,378797670,1310909982,1429332901,1045991765,682460013,395279864,1301754077},{17376503,2136593576,1654170345,317694353,844644429,1072011533,2057520448,1964485542,1694915416,72819257,1955057256,18845,316727915,1781590139,820976052},{562073989,2144453617,613893318,1193555438,441499839,731793688,617667847,207554931,862282589,1163823367,1098272293,1040482486,429804681,1739768706,155304190},{1004890225,1378611567,1123539086,497710331,568728052,162657167,32323138,2089101322,168290404,220856929,1090663687,2011660264,2139002327,1335859109,1989999225},{1006656197,994531913,1232637190,158509721,1193158567,213739883,1733555797,961641330,343905988,1149446239,2127534108,1863030606,1643821782,335571419,656782111},{470993997,371384737,1275796577,1836337991,1845123700,1330163220,748473270,1778528411,902121084,714510968,57285152,718875808,402707034,1576148741,1121104242},{377476516,581111174,2127358509,1059221860,1849851037,1325621240,1720826702,1772106365,340976312,1302505588,1910603645,192487924,1034166286,1647613631,1788151799},{1581129675,1083799747,466053975,1096297216,57618052,2018958814,251680651,1601400414,324210247,835608890,1683046497,307876795,1197187942,1363452451,1894830467},{1380657506,1149897507,1122060796,1421894065,575526639,609875585,239509964,1059610470,1938768366,1110551431,1257524740,1831735053,1792956026,721394278,1928443031},{1518821493,1842204609,1661124664,1234816848,284799728,2035462980,611808150,507875214,1758708520,649178332,1523299164,1936493461,1530928742,1317792087,1132754698},{765678631,1038738393,1181604688,1448707407,255799763,2111839094,69935242,728057385,98649089,137863339,2081767107,1456190425,1468831763,1330918476,557158980},{1142275940,1875403047,1281523910,1458859607,1232617050,1967500388,825824610,447409709,1274731016,1107323440,691771178,121723788,1407272972,1799435993,108533650},{911439247,558570278,1239641309,1912620816,1882826216,1488673767,1955514419,1241106445,767357804,1343311593,542362640,1568292612,79646606,738194961,829680808},{834020085,749804626,532308586,93531500,25890896,1356592378,414216847,1748047602,1859755854,292156093,1119838009,570734955,1680421183,1281380984,1204185972},{867742076,579624555,764073093,1971748638,1359202009,1324612124,1940483266,2015588320,1585846462,901943917,2031832493,1871238904,14249213,1115838074,2063304114},{386312242,906786413,1815284179,181023524,1625523716,2037621325,379890566,351860231,1692422226,1119447867,456069302,787622571,483350689,1891877069,1135021201}};
const int DEFAULTTIME = 15;
const int initial_pieces[14] = {1,2,2,2,2,2,5,1,2,2,2,2,2,5};
static const U32 turn[2]= {2957199342U,1642319895U};
static const int pos_score[32] = {2,3,3,2,3,4,4,3,3,4,4,3,3,5,5,3,3,5,5,3,3,4,4,3,3,4,4,3,2,3,3,2};
extern int value[7];
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
static const POS POW[32][4] = {
	{ 2, -1, -1, 8 }, { 3, -1, -1, 9 }, { -1, -1, 0, 10 }, { -1, -1, 1, 11 },
	{ 6, -1, -1, 12 }, { 7, -1, -1, 13 }, { -1, -1, 4, 14 }, { -1, -1, 5, 15 },
	{ 10, 0, -1, 16 }, { 11, 1, 7, 17 }, { -1, 2, 8, 18 }, { -1, 3, 9, 19 },
	{ 14, 4, -1, 20 }, { 15, 5, 11, 21 }, { -1, 6, 12, 22 }, { -1, 7, 13, 23 },
	{ 18, 8, -1, 24 }, { 19, 9, 15, 25 }, { -1, 10, 16, 26 }, { -1, 11, 17, 27 },
	{ 22, 12, -1, 28 }, { 23, 13, 19, 29 }, { -1, 14, 20, 30 }, { -1, 15, 21, 31 },
	{ 26, 16, -1, -1 }, { 27, 17, 23, -1 }, { -1, 18, 24, -1 }, { -1, 19, 25, -1 },
	{ 30, 20, -1, -1 }, { 31, 21, 27, -1 }, { -1, 22, 28, -1 }, { -1, 23, 29, -1 }
};
// (level)
enum LVL {
	LVL_K=0, // King
	LVL_G=1, // Guard
	LVL_M=2, // Minister
	LVL_R=3, // Rook
	LVL_N=4, // kNight
	LVL_C=5, // Cannon
	LVL_P=6  // Pawn
};

enum FIN {
	FIN_K= 0 /* 'K' «Ó */ , FIN_k= 7 /* 'k' ±N */ , FIN_X=14 /* 'X' ¥¼Â½ */ ,
	FIN_G= 1 /* 'G' ¥K */ , FIN_g= 8 /* 'g' ¤h */ , FIN_E=15 /* '-' ªÅ®æ */ ,
	FIN_M= 2 /* 'M' ¬Û */ , FIN_m= 9 /* 'm' ¶H */ ,
	FIN_R= 3 /* 'R' ÚÏ */ , FIN_r=10 /* 'r' ¨® */ ,
	FIN_N= 4 /* 'N' ØX */ , FIN_n=11 /* 'n' °¨ */ ,
	FIN_C= 5 /* 'C' ¬¶ */ , FIN_c=12 /* 'c' ¯¥ */ ,
	FIN_P= 6 /* 'P' §L */ , FIN_p=13 /* 'p' ¨ò */
};

// (position)
//  0  1  2  3
//  4  5  6  7
//  8  9 10 11
// 12 13 14 15
// 16 17 18 19
// 20 21 22 23
// 24 25 26 27
// 28 29 30 31

typedef struct EAT_COUNTER{
	int num_cs;
	int num_ucs;
	EAT_COUNTER(){
		num_ucs = num_cs =0;
	};
	EAT_COUNTER(int cs, int ucs): num_cs(cs) , num_ucs(ucs){};
	int score() const{
		return num_cs+4*num_ucs+1;
	}
}EAT_COUNTER;

typedef struct MOV {
	POS st; //
	POS ed; // // ­ed == st implies flip
	bool is_eat_move;
	MOV() {st= ed=-1; is_eat_move = false;}
	MOV(POS s,POS e):st(s),ed(e) {is_eat_move = (s!=e);};
	MOV(POS s, POS e, bool eat): st(s), ed(e), is_eat_move(eat){};
	bool operator==(const MOV &x) const {return st==x.st&&ed==x.ed;}
	MOV operator=(const MOV &x) {st=x.st;ed=x.ed; is_eat_move = x.is_eat_move; return MOV(x.st, x.ed,x.is_eat_move);}

}MOV;
//bool operator<(const MOV &x) const {return this->is_eat_move || ! x.is_eat_move; };


typedef struct MOVLST {
		int num;     // ¨«ªk¼Æ(²¾°Ê+¦Y¤l,¤£¥]¬AÂ½¤l)
		MOV mov[68];
		SCORE scores[68];
		MOVLST(){
			num =0;
		}
		void sort(){
			for (int i =0 ;i  < num; i ++){
					for (int j =i+1 ; j <num; j ++){
							if(scores[i] > scores[j]){
								MOV tmp = mov[j];
								mov[j] = mov[i];
								mov[i] = tmp;
							}
					}
			}
		}
}MOVLST;

typedef struct BOARD {
	CLR who;     // Whose turn is it
	FIN fin[32]; // What is  currently on the board
	int cnt[14]; // The count of dark piecess
	int total_cnt[14]; // count of each living piec (dark or not)

	int dark_cnt[2];
	int total_sum[2];
	int empty;
	int total_pos_score[2];
	EAT_COUNTER eat_cnt[14];
	U32 key;
	U32 check;
	void NewGame();              // Start a new game
	int  LoadGame(const char*);  // Load from board.txt
	void Display() const;        // Åã¥Ü¨ì stderr ¤W
	int  MoveGen(MOVLST&) const; // ¦C¥X©Ò¦³¨«ªk(¨«¤l+¦Y¤l,¤£¥]¬AÂ½¤l)
	                             // ¦^¶Ç¨«ªk¼Æ¶q
	bool ChkLose() const;        // ÀË¬d·í«eª±®a(who)¬O§_¿é¤F
	bool ChkWin() const;
	bool ChkValid(MOV) const;    // ÀË¬d¬O§_¬°¦Xªk¨«ªk
	void Flip(POS,FIN=FIN_X);    // Â½¤l
	void Move(MOV);              // ²¾°Ê or ¦Y¤l
	void DoMove(MOV m, FIN f) ;

	void Init(int Board[32], int Piece[14], int Color);
	void Init(char Board[32], int Piece[14], int Color);
	SCORE Eval() const;
	SCORE Eval2() const;
	SCORE Eval3() const;
	int GetValue(int) const;
	int ChkEat(int x,MOVLST ) const;
	int ChkDead(int x,MOVLST ,MOVLST) const;
}BOARD;

CLR  GetColor(FIN);    // ºâ¥X´Ñ¤lªºÃC¦â
LVL  GetLevel(FIN);    // ºâ¥X´Ñ¤lªº¶¥¯Å
bool ChkEats(FIN,FIN); // §PÂ_²Ä¤@­Ó´Ñ¤l¯à§_¦Y²Ä¤G­Ó´Ñ¤l
void Output (MOV);     // ±Nµª®×¶Çµ¹ GUI

#endif
