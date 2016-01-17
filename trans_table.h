#ifndef TRANS_TABLE
#define TRANS_TABLE
#define MAX_HASH 99999989


typedef struct trans_node{
	U32 position;
	U32 check;
	int search_depth;
	U32 best_value;
	MOV best_move;
	bool is_exact;
	trans_node(){
		position = check = best_value =0 ;
		search_depth = -1;
		is_exact= false;
	}
  void set_node(U32 position, U32 check,int search_depth,U32 best_value, MOV best_move , bool is_exact){
    this->position = position;
    this->check = check;
    this->search_depth = search_depth;
    this->best_value = best_value;
    this->best_move = best_move;
    this->is_exact = is_exact;
  }
}trans_node;

trans_node *trans_table [MAX_HASH];

#endif
