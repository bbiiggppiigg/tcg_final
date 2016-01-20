#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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
}
