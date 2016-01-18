#ifndef TRANS_TABLE
#define TRANS_TABLE
#define MAX_HASH 999983
//#define MAX_HASH 9999989


typedef struct trans_node{
	U32 key;
	U32 check;
	int search_depth;
	U32 best_value;
	MOV best_move;
	bool is_exact;
	trans_node(){
		key = check = best_value =0 ;
		search_depth = -1;
		is_exact= false;
	}
  void set_node(U32 key, U32 check,int search_depth,U32 best_value, MOV best_move , bool is_exact){
    this->key = key;
    this->check = check;
    this->search_depth = search_depth;
    this->best_value = best_value;
    this->best_move = best_move;
    this->is_exact = is_exact;
  }

}trans_node;

trans_node *trans_table [MAX_HASH];


#endif
