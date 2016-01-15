#include <stdio.h>

#include <iostream>
#include <time.h>
using namespace std;

int main(){
	srand(time(NULL));
	printf("unsigned int zobrist_table[32][15] = {");
	for (int i =0 ; i < 32 ; i++){
		cout<<"{"<<(unsigned int) rand();
		for(int j =0;  j  < 15 ;j ++){
			cout<<","<<(unsigned int) rand();
		}
		cout<<"}";
	}
	printf("}\n");
	return 0;
}