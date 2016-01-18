
/*	pair<SCORE,MOV> tmp_pair = nega_scout(B,-INF,INF,search_depth,1,0);
	BestMove = tmp_pair.second;
	if( tmp_pair.first> B.Eval()){
		cerr << "find best move with value "<< (tmp_pair.first) << " move : " << (tmp_pair.second.st)<<" "<<(tmp_pair.second.ed) << endl;
		return tmp_pair.second;
	}else{
		cerr << "current best value "<<B.Eval()<< " is greater than nega_scout value "<< (tmp_pair.first) <<", fliping " << endl;
	}
*/

std::pair<SCORE,MOV> nega_scout(const BOARD & B , int alpha, int beta, int depth,int is_max,int cut){

	trans_node *tn = trans_table[B.Key%MAX_HASH];
	SCORE m = -INF;
	SCORE n  = beta;
	SCORE t;
	pair<SCORE,MOV> tmp_pair ;
	MOV RET_MOVE ;
	MOVLST lst;

	if(cut > max_length)
	{
		max_length = cut;
		cerr<<"New Depth : "<<cut<<endl;
	}
	if(tn!=NULL){ //hash hit
		if(tn->position == B.Key && tn->check == B.Check){
			if(cut <=tn->search_depth ){
				if(tn->is_exact){
					return make_pair(tn->best_value,tn->best_move);
				}else{
					m = tn->best_value;
					RET_MOVE  = tn->best_move;
				}
			}
		}
	}else{
		tn = (trans_node *) malloc (sizeof(trans_node));
	}

	if(B.ChkLose()){ // update value
		tn->set_node(B.Key,B.Check,cut,-WIN,MOV(),true);
		return make_pair(-WIN,MOV());
	}
	if(cut==depth || TimesUp() || B.MoveGen(lst)==0 ){
		if(cut >= tn->search_depth){
			tn->set_node(B.Key,B.Check,cut,B.Eval(),MOV(),true);
		}
		return make_pair( B.Eval(),MOV());
	}

	for (int i = 0 ; i< lst.num;i++){
		BOARD N (B);
		N.Move(lst.mov[i]);
		tmp_pair =  nega_scout(N,-n,-get_max(alpha,m),depth,-is_max,cut+1); // null window search
		t = -tmp_pair.first;
		if(t>m){ // if failed high
			if(n==beta || depth-cut < 3 || t>= beta){
				m = t;
			}else{
				tmp_pair  = nega_scout(N,-beta,-t,depth,-is_max,cut+1); //research
				m = -tmp_pair.first;
			}
			RET_MOVE = lst.mov[i];
		}
		if(m>=beta){ //cut off
			tn->search_depth = cut;
			tn->best_value = m;

			tn->set_node(B.Key,B.Check,cut,m,lst.mov[i],false);
			return make_pair(m,RET_MOVE);
		}
		//if(get_max(alpha,m)!=alpha)
		n=get_max(alpha,m)+1;
	}
	//cerr << "At End , Updating Hash Entry , No cut"<<endl;
	tn->set_node(B.Key,B.Check,cut,m,RET_MOVE,true);
	//cerr << "At End , End Updating Hash Entry , No cut"<<endl;
	return make_pair(m,RET_MOVE);
}
