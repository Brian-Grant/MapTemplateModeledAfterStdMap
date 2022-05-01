//#include "Map.hpp"

#define HT_FOR_BILLION_ITEMS 31

// SkipList--------------------------------------------------------------------

// constructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::                  
SkipList() : sentinel(new Node(1, 0, HT_FOR_BILLION_ITEMS)), 
			 node_id(1), 
			 curr_max_ht(1), 
			 size(0), 
			 flip(std::random_device{}()), 
			 //flip(6627998), // XXX flip(debug_seed)
			 coin(0.5), 
			 capacity(HT_FOR_BILLION_ITEMS) { 
		
	// XXX this->_print_trace("SkipList()", false);
	this->sentinel->forward[0] = this->sentinel;
	this->sentinel->backward = this->sentinel;
	
	//this->init_stats();
	
	//this->_check_sz_capacity(this->sentinel);
}	


// copy ctor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::                 
SkipList(const SkipList& s) : 
				sentinel(new Node(s.sentinel->height, 0)),
				node_id(s.node_id),
				curr_max_ht(s.curr_max_ht),
				size(s.size),	
				flip(s.flip),
				coin(s.coin),
				capacity(s.capacity) {
	// XXX this->_print_trace("SkipList(SkipList)", false);
	this->init_stats();
	this->copy_list(s);
}

// destructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::                  
~SkipList(){
	// XXX this->_print_trace("~SkipList()", false);
	this->clear_list();
	delete this->sentinel;
}

// clear_list
template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                  
init_stats(){
	// XXX this->_print_trace("init_stats()", false);
	this->same = 0; // XXX DEBUG
	
	this->insert_success_op_sum = 0;
	this->insert_success_total = 0;
	this->insert_all_op_sum = 0;
	this->insert_all_total = 0;		
	
	this->find_success_op_sum = 0;
	this->find_success_total = 0;
	this->find_all_op_sum = 0;
	this->find_all_total = 0;
		
	this->erase_success_op_sum = 0;
	this->erase_success_total = 0;
	this->erase_all_op_sum = 0;
	this->erase_all_total = 0;
}


// copy_list
template<typename Key_T, typename Mapped_T>	inline
void Map<Key_T, Mapped_T>::SkipList::                  
copy_list(const SkipList& s){
	// XXX this->_print_trace("copy_list()", false);
	// maybe make list circular first? NAH
	using NodePtrVect = std::vector<Map<Key_T, Mapped_T>::SkipList::Node*>;
	using Node = Map<Key_T, Mapped_T>::SkipList::Node;	
	using DataNode = Map<Key_T, Mapped_T>::SkipList::DataNode;
	NodePtrVect update(this->curr_max_ht, this->sentinel);
	auto update_begin = update.begin();
	unsigned int i;	
	// XXX for(unsigned int i = 0; i < this->curr_max_ht; i++) {
	// XXX 	assert(update[i]->id == 0 && update[i]->height == this->curr_max_ht);
	// XXX }
	Node* loc = s.sentinel->backward;
	Node* nFront = this->sentinel;
	DataNode* dN;
	DataNode* dLoc;	
	//unsigned int mx_ht = 1;
	//unsigned int mx_ht_idx = 0;
	while(loc != s.sentinel){
					//std::cout << ")))))" << std::endl;
					//s.pretty_print_dnode(loc);		
		dLoc = static_cast<DataNode*>(loc);
		dN = new DataNode(*dLoc);		
					//std::cout << "dN.id " << dN->id << std::endl;
					//std::cout << "(" << dN->pair.first << ", ";
					//std::cout <<  dN->pair.second << ")" << std::endl;		
		// XXX assert(dN->id == loc->id);
		
		//21dN->forward.assign(update.begin(), update.begin()+ dN->height);
		dN->forward.assign(update_begin, update_begin+ dN->height);
		for(i = 0; i < dN->height; i++) update[i] = dN;
		nFront->backward = dN;
		nFront = nFront->backward;
		loc = loc->backward;
	}
	this->sentinel->forward.assign(update_begin, update.end()); //21
	this->sentinel->forward[0]->backward = this->sentinel;
	this->sentinel->forward.resize(this->capacity);

}

// clear_list
template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                
clear_list(){
	// XXX this->_print_trace("clear_list()", false);
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel->forward[0];
	while(tmp != this->sentinel){
		Map<Key_T, Mapped_T>::SkipList::Node* del = tmp;
		tmp = tmp->forward[0];
		delete static_cast<DataNode *>(del);
	}
}

// reset
template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                  
reset(){
	// XXX this->_print_trace("reset()", false);
	this->clear_list();
	this->size = 0;
	this->curr_max_ht = 1;
	this->sentinel->reset();
}


// operator= 
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::SkipList& Map<Key_T, Mapped_T>::SkipList::   
operator=(const SkipList & other){
	SkipList second(other);	
	//assert(0); // should never use
	using std::swap;
	swap(this->sentinel, second.sentinel);
	swap(this->node_id, second.node_id);
	swap(this->curr_max_ht, second.curr_max_ht);
	swap(this->size, second.size);
	swap(this->capacity, second.capacity);	
	return *this;
}

// insert(ValueType)
template<typename Key_T, typename Mapped_T>	inline
typename Map<Key_T, Mapped_T>::SkipList::InsertResult Map<Key_T, Mapped_T>::SkipList::                 
insert(const ValueType &pair){
	// XXX this->_print_trace("insert(ValueType&)", false);
	std::vector<Map<Key_T, Mapped_T>::SkipList::Node*> update(this->curr_max_ht);
	// gen num operations and position, and populate update
	typename Map<Key_T, Mapped_T>::SkipList::SearchHelpResult info = 
											this->find_help(pair.first, update);
	//STAT this->insert_all_op_sum += info.second;
	//STAT this->insert_all_total++;	

	Map<Key_T, Mapped_T>::SkipList::Node* pos = info.first;	
	if((pos != this->sentinel) && 
		(static_cast<DataNode *>(pos)->pair.first == pair.first))
	{	
		this->same++;
		return InsertResult(false, pos);
	
	} 
	// XXX if(pos == sentinel); // std::cout << "landed on sentinel...\n";
	// XXX else assert(pair.first < static_cast<DataNode *>(pos)->pair.first);
	
	//STAT this->insert_success_op_sum += info.second;
	//STAT this->insert_success_total++;

	Map<Key_T, Mapped_T>::SkipList::Node* newNode = 
											this->insert_before(pair, update);
	if(pos == sentinel) this->sentinel->backward = newNode;
	return InsertResult(true, newNode);
}



// find(Key)
// XXX return a position and a bool indicating if found
template<typename Key_T, typename Mapped_T>	inline
typename Map<Key_T, Mapped_T>::SkipList::SearchResult Map<Key_T, Mapped_T>::SkipList::
find(const Key_T & key) {                   
	// XXX this->_print_trace("find(Key_T)", false);
	if(!this->size){
		return SearchResult(false, this->sentinel); // TODO think through
	}
	//get num operations, and position
	typename Map<Key_T, Mapped_T>::SkipList::SearchHelpResult info = 
												this->find_help(key);
	//21this->find_all_op_sum += info.second;
	//21this->find_all_total++;		

	Map<Key_T, Mapped_T>::SkipList::Node* pos = info.first;	
	if(pos != this->sentinel && 
		static_cast<DataNode *>(pos)->pair.first == key)
	{
		// XXX assert(static_cast<DataNode *>(pos)->pair.first == key);		
		//21this->find_success_op_sum += info.second;
		//21this->find_success_total++;		
		return SearchResult(true, pos); // found return position unchanged

	}
	else{
		return SearchResult(false, pos);
	}
	/*
	if(pos == this->sentinel || 
		static_cast<DataNode *>(pos)->pair.first != key) 
	{	// did not find, but return position
		return SearchResult(false, pos);
	}
	else {
		assert(static_cast<DataNode *>(pos)->pair.first == key);		
		this->find_success_op_sum += info.second;
		this->find_success_total++;		
		return SearchResult(true, pos); // found return position unchanged
	}
	*/
}

//---------------21
// find(Key) const 
// XXX return a position and a bool indicating if found
template<typename Key_T, typename Mapped_T>	inline
typename Map<Key_T, Mapped_T>::SkipList::SearchResult Map<Key_T, Mapped_T>::SkipList::
find(const Key_T & key) const {                  
	// XXX this->_print_trace("find(Key_T) const ", false);
	using SearchHelpResult = typename SkipList::SearchHelpResult;	
	using Node = Map<Key_T, Mapped_T>::SkipList::Node;	
	if(!this->size){
		return SearchResult(false, this->sentinel); // TODO think through
	}
	// get num operations, and position
	SearchHelpResult info = this->find_help(key);	
	Node* pos = info.first;	
	
	if(pos != this->sentinel && 
		static_cast<DataNode *>(pos)->pair.first == key)
	{
		// XXX assert(static_cast<DataNode *>(pos)->pair.first == key);		
		return SearchResult(true, pos); // found return position unchanged

	}
	else{
		return SearchResult(false, pos);
	}

//	if(pos == this->sentinel || 
//		static_cast<DataNode *>(pos)->pair.first != key) 
//	{	// did not find, but return position
//		return SearchResult(false, pos);
//	}
//	else {
//		assert(static_cast<DataNode *>(pos)->pair.first == key);		
//		return SearchResult(true, pos); // found return position unchanged
//	}

}


// erase(Key)
template<typename Key_T, typename Mapped_T>	inline
bool Map<Key_T, Mapped_T>::SkipList::                  
erase(const Key_T & key){	
	// XXX this->_print_trace("erase(Key_T)", false); 	
	// XXX bool print = false;	
	using SkipList = Map<Key_T, Mapped_T>::SkipList;
	using SearchHelpResult = typename SkipList::SearchHelpResult;	
	using NodePtrVect = std::vector<Map<Key_T, Mapped_T>::SkipList::Node*>;
	NodePtrVect update(this->curr_max_ht);
	// gen num operations and position, and populate update
	SearchHelpResult info = this->find_help(key, update);
	//STAT this->erase_all_op_sum += info.second;
	//STAT this->erase_all_total++;			

	Map<Key_T, Mapped_T>::SkipList::Node* pos = info.first;	
	if((pos != this->sentinel) && 
							(static_cast<DataNode *>(pos)->pair.first == key)) {
		for(unsigned int i = 0; i < this->curr_max_ht; i++){
			if(update[i]->forward[i] != pos) break;
			update[i]->forward[i] = pos->forward[i];
		}
		pos->forward[0]->backward = pos->backward;
	
		unsigned int top = this->curr_max_ht - 1;
		unsigned int tmpHt = this->curr_max_ht;		
		// XXX if(print) std::cout << "beforeHt " << this->curr_max_ht << std::endl;		
		// XXX if(print) std::cout << "beforeSize " << this->size << std::endl;		
		while((tmpHt > 1) && (this->sentinel->forward[top] == pos)){
			this->sentinel->forward[top] = nullptr;			
			tmpHt--;
			top--;
		}	
		// XXX assert(this->curr_max_ht == this->sentinel->height);		
		// XXX if(print) {
		// XXX 	std::cout << "this->sentinel->forward.size() ";
		// XXX 	std::cout << this->sentinel->forward.size() << std::endl;
		// XXX }	
		if(this->curr_max_ht - tmpHt){
			this->curr_max_ht = tmpHt;
			this->sentinel->height = tmpHt;
			// XXX assert(this->curr_max_ht == this->sentinel->height);
			//assert(this->curr_max_ht == this->sentinel->forward.size());	
		}		
		// XXX if(print) {
		// XXX 	std::cout << "newHt: " << tmpHt << std::endl;
		// XXX 	std::cout << "newSize " << this->size << std::endl;
		// XXX 	std::cout << "found/deleted " << pos->id << std::endl;
		// XXX }		
		delete pos;
		pos = nullptr;		
		this->size--;		
		
		//STAT this->erase_success_op_sum += info.second;
		//STAT this->erase_success_total++;					
		
		return true;

	} else {
		//std::cout << "not found\n";
		return false;
	}
}


//insert_before(pair, update)
// XXX place new node in correct position and return ptr to that node 
template<typename Key_T, typename Mapped_T>	inline
typename Map<Key_T, Mapped_T>::SkipList::Node* Map<Key_T, Mapped_T>::SkipList::                
insert_before(const ValueType & pair, 
					std::vector<Map<Key_T, Mapped_T>::SkipList::Node*> &update){
	unsigned int rand_ht = this->get_rand_height();
	unsigned int old_max = this->curr_max_ht;	
	if(rand_ht > this->curr_max_ht){ // TODO check if past 31
		if(rand_ht > this->capacity){
			std::cout << "AT CAPACITY" << std::endl;
			this->capacity*=2;
			this->sentinel->forward.resize(this->capacity);
		}		
		this->curr_max_ht = this->sentinel->height = rand_ht;
	}
	Map<Key_T, Mapped_T>::SkipList::DataNode* newNode = 
								new DataNode(pair, rand_ht, this->node_id++);
	int end = rand_ht;
	if(rand_ht > old_max){
		end = old_max;		
		for(unsigned int i = old_max; i < rand_ht; i++){
			this->sentinel->forward[i] = newNode;
			newNode->forward[i] = this->sentinel;   //keep list of max heights
		}											// for erase
	}
	//newNode->backward = update[0]->forward[0];	
	for(int i = 0; i < end; i++){
		newNode->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = newNode;
	}
	newNode->backward = update[0];
	newNode->forward[0]->backward = newNode;
	++this->size;
	return newNode;

}





// find(Key, update)
// return num operations and position where key should be and UPDATE vector
template<typename Key_T, typename Mapped_T>	inline// pass in new vect already init
typename Map<Key_T, Mapped_T>::SkipList::SearchHelpResult Map<Key_T, Mapped_T>::SkipList::
find_help(const Key_T & key, 
			std::vector<Map<Key_T, Mapped_T>::SkipList::Node*> &update){
	// XXX this->_print_trace("find_help(Key_T, update)", false);
	unsigned long local_op_count = 0;	
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel;
	for(int i = (this->curr_max_ht-1); i >= 0; i--){
		while(tmp->forward[i] != this->sentinel && 
			  static_cast<DataNode *>(tmp->forward[i])->pair.first < key)
		{	
			tmp = tmp->forward[i];
			//local_op_count++;
		}
		update[i] = tmp;
	}
	tmp = tmp->forward[0];
	return SearchHelpResult(tmp, local_op_count);

}
//-------------21
// find_help(const Key_T & key)
// XXX return num ops and position of where key should be
template<typename Key_T, typename Mapped_T>	inline// pass in new vect
typename Map<Key_T, Mapped_T>::SkipList::SearchHelpResult Map<Key_T, Mapped_T>::SkipList::
find_help(const Key_T & key){
	// XXX this->_print_trace("find_help(Key_T)", false);
	unsigned long local_op_count = 0;	
	//update.resize(this->curr_max_ht);
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel;
	for(int i = (this->curr_max_ht-1); i >= 0; i--){
		while(tmp->forward[i] != this->sentinel && 
			  static_cast<DataNode *>(tmp->forward[i])->pair.first < key)
		{	
			tmp = tmp->forward[i];
			//local_op_count++;
		}
		//update[i] = tmp;
	}
	tmp = tmp->forward[0];
	return SearchHelpResult(tmp, local_op_count);

}




// find_help(const Key_T & key)
// XXX return num ops and position of where key should be
template<typename Key_T, typename Mapped_T>	inline// pass in new vect
typename Map<Key_T, Mapped_T>::SkipList::SearchHelpResult Map<Key_T, Mapped_T>::SkipList::
find_help(const Key_T & key) const {
	// XXX this->_print_trace("find_help(Key_T) const", false);
	unsigned long local_op_count = 0;	
	//update.resize(this->curr_max_ht);
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel;
	for(int i = (this->curr_max_ht-1); i >= 0; i--){
		while(tmp->forward[i] != this->sentinel && 
			  static_cast<DataNode *>(tmp->forward[i])->pair.first < key)
		{	
			tmp = tmp->forward[i];
			//local_op_count++;
		}
	}
	tmp = tmp->forward[0];
	return SearchHelpResult(tmp, local_op_count);
}



// get_rand_height()
template<typename Key_T, typename Mapped_T>	inline
unsigned int Map<Key_T, Mapped_T>::SkipList::                  
get_rand_height(){
	// XXX this->_print_trace("get_rand_height()", false);
	unsigned int height = 1;
	while(this->coin(flip)){
		++height;
	}
	//std::cout << "Height: " << height << std::endl;
	return height;
}

/*
template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                
pretty_print_sentinel() const { 
	this->_print_trace("pretty_print_sentinel()", false);	
	for(unsigned int i = 0; i < this->curr_max_ht; i++){
		std::cout << this->sentinel->id << "\t";
	}
	std::cout << "back: " << this->sentinel->backward->id;
	std::cout << "\theight: " << this->sentinel->height;
	std::cout << "\tsz: " << this->sentinel->forward.size();		
	std::cout << std::endl;
	for(unsigned int i = 0; i < this->curr_max_ht; i++){
		std::cout << this->sentinel->forward[i]->id << "\t";
	}
	std::cout << std::endl;
}


template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                  
pretty_print_dnode(typename Map<Key_T, Mapped_T>::SkipList::Node* n) const { 
	this->_print_trace("pretty_print_dnode(node)", false);
	unsigned int i;	
	for(i = 0; i < n->height; i++){		
		std::cout << static_cast<DataNode *>(n)->id << "\t";
	}
	for(i = static_cast<DataNode *>(n)->height; 
					i < this->curr_max_ht; i++){
		std::cout << "\t";
	}
	std::cout << "back: " << static_cast<DataNode *>(n)->backward->id;
	std::cout << "\theight: " << static_cast<DataNode *>(n)->height;
	std::cout << "\tsz: " << static_cast<DataNode *>(n)->forward.size();
	std::cout << "\t(" << static_cast<DataNode *>(n)->pair.first << ", " <<
	static_cast<DataNode *>(n)->pair.second << ") ";	
	std::cout << std::endl;
	for(unsigned int i = 0; i < n->height; i++){		
		std::cout << static_cast<DataNode *>(n)->forward[i]->id << "\t";
	}
	std::cout << std::endl;


}


template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                  
pretty_print()const {  
	this->_print_trace("pretty_print()", false);		
	this->pretty_print_sentinel();
	for (Node *n = this->sentinel->forward[0]; 
									n != this->sentinel; n = n->forward[0]) {
	
		this->pretty_print_dnode(n);		
	}
	this->pretty_print_sentinel();
	std::cout << "this->size " << this->size << std::endl;
	std::cout << "this->curr_max_ht " << this->curr_max_ht << std::endl;
}

*/

template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                  
verify_forward(){ 
	// XXX this->_print_trace("verify_forward()", false);	
	//for (Node *n = sent.forward[0]; n != &sent; n = n->forward[0]) {
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel->forward[0];
	while(tmp != this->sentinel){
		Map<Key_T, Mapped_T>::SkipList::Node* del = tmp;
		tmp = tmp->forward[0];
		if(tmp != this->sentinel){	
			assert(static_cast<DataNode *>(del)->pair.first < 
						static_cast<DataNode *>(tmp)->pair.first);
			//std::cout << "X";		
		}	
	}
	//std::cout << std::endl;
}

template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                  
verify_backward(){ 
	// XXX this->_print_trace("verify_backward()", false);	
	//for (Node *n = sent.forward[0]; n != &sent; n = n->forward[0]) {
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel->backward;
	while(tmp != this->sentinel){
		Map<Key_T, Mapped_T>::SkipList::Node* del = tmp;
		tmp = tmp->backward;
		if(tmp != this->sentinel){	
			assert(static_cast<DataNode *>(tmp)->pair.first < 
					static_cast<DataNode *>(del)->pair.first );
			//std::cout << "X";		
		}	
	}
	//std::cout << std::endl;
}

template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::                 
verify_both(){ 
	// XXX this->_print_trace("verify_both()", false);	
	std::vector<Key_T> forward;
	std::vector<Key_T> backward;		
	// backward
	Map<Key_T, Mapped_T>::SkipList::Node* tmp = this->sentinel->backward;
	while(tmp != this->sentinel){
		Map<Key_T, Mapped_T>::SkipList::Node* del = tmp;
		backward.push_back(static_cast<DataNode *>(del)->pair.first);
		//std::cout << static_cast<DataNode *>(del)->pair.first << std::endl;
		tmp = tmp->backward;
		if(tmp != this->sentinel){	
			assert(static_cast<DataNode *>(tmp)->pair.first <
					static_cast<DataNode *>(del)->pair.first);		
		}	
	}
	// forward	
	//std::cout << "forward" << std::endl;
	tmp = this->sentinel->forward[0];	
	while(tmp != this->sentinel){
		Map<Key_T, Mapped_T>::SkipList::Node* del = tmp;
		forward.push_back(static_cast<DataNode *>(del)->pair.first);		
		//std::cout << static_cast<DataNode *>(del)->pair.first << std::endl;		
		tmp = tmp->forward[0];
		if(tmp != this->sentinel){	
			assert(static_cast<DataNode *>(del)->pair.first < 
						static_cast<DataNode *>(tmp)->pair.first);	
		}	
	}
	// compare forward and backward
	assert(forward.size() == backward.size());
	size_t back_idx = backward.size()-1;
	for(size_t i = 0; i < backward.size(); i++){
		assert(forward[i] == backward[back_idx]);
		back_idx--;
	}
}


template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::              
show_time_complexity(){
	/*	
	// XXX this->_print_trace("show_time_complexity()", false);
	if(this->insert_success_total){
		double lg_insert_success = std::log2(this->insert_success_total);
		double ave_insert_success = ((double)this->insert_success_op_sum) /
								((double)this->insert_success_total);
		std::cout << "lg(successful_inserts): " << lg_insert_success;
		std::cout << std::endl;
		std::cout << "ave(successful_inserts): " << ave_insert_success;
		std::cout << std::endl; std::cout << std::endl;
		
	}
	if(this->insert_all_total) {
		double lg_insert_all = std::log2(this->insert_all_total);
		double ave_insert_all = 	((double)this->insert_all_op_sum) /
								((double)this->insert_all_total);
		std::cout << "lg(all_inserts): " << lg_insert_all;
		std::cout << std::endl;
		std::cout << "ave(all_inserts): " << ave_insert_all;
		std::cout << std::endl; std::cout << std::endl;
	}
	if(this->find_success_total){
		double lg_find_success = std::log2(this->find_success_total);
		double ave_find_success = ((double)this->find_success_op_sum) /
								((double)this->find_success_total);
		std::cout << "lg(successful_find): " << lg_find_success;
		std::cout << std::endl;
		std::cout << "ave(successful_find): " << ave_find_success;
		std::cout << std::endl; std::cout << std::endl;
	}
	if(this->find_all_total){
		double lg_find_all = std::log2(this->find_all_total);
		double ave_find_all = 	((double)this->find_all_op_sum) /
								((double)this->find_all_total);
		std::cout << "lg(all_find): " << lg_find_all;
		std::cout << std::endl;
		std::cout << "ave(all_find): " << ave_find_all;
		std::cout << std::endl; std::cout << std::endl;	
	}
	if(this->erase_success_total){
		double lg_erase_success = std::log2(this->erase_success_total);
		double ave_erase_success = ((double)this->erase_success_op_sum) /
								((double)this->erase_success_total);
		std::cout << "lg(successful_erase): " << lg_erase_success;
		std::cout << std::endl;
		std::cout << "ave(successful_erase): " << ave_erase_success;
		std::cout << std::endl; std::cout << std::endl;
	}
	if(this->erase_all_total) {
		double lg_erase_all = std::log2(this->erase_all_total);
		double ave_erase_all = 		((double)this->erase_all_op_sum) /
								((double)this->erase_all_total);
		std::cout << "lg(all_erase): " << lg_erase_all;
		std::cout << std::endl;
		std::cout << "ave(all_erase): " << ave_erase_all;
		std::cout << std::endl; std::cout << std::endl;
	}
*/
}


// XXX XXX Node XXX XXX-------------------------------------------------------

// XXX Node(height) XXX
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::Node::                 
Node(int aHeight, unsigned long aId) : height(aHeight), id(aId)  {
	// XXX this->_print_trace("Node(height)", false);
	this->forward.resize(aHeight);
}

// XXX Node(height, capacity) XXX USED FOR HEAD NODE CONSTRUCTOR IN SKIPLIST
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::Node::                 
Node(int aHeight, unsigned long aId, int capacity) : height(aHeight), id(aId) { 
	// XXX this->_print_trace("Node(height, capacity)", false);
	this->forward.resize(capacity);
}


// XXX copy ctor XXX
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::Node::                 
Node(const Node& n) : Node(n.height, n.id) {
	// XXX this->_print_trace("Node(Node)", false);
}

// XXX destructor XXX
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::Node::                  
~Node() {
	// XXX this->_print_trace("~Node()", false);
}


// XXX operator=() XXX
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::SkipList::Node& 
Map<Key_T, Mapped_T>::SkipList::Node::
operator=(const Node& other) {
	// XXX this->_print_trace("operator=()", false);
	assert(0); // should never use	
	Node second = Node(other);
	using std::swap;
	swap(this->height, second.height);
	//swap(this->id, second.id);
	swap(this->backward, second.backward);
	//swap(first.forward, second.forward);
	this->forward.swap(second.forward);

	//this->swap(*this, otherCpy);
	return *this;
}

// XXX reset XXX
template<typename Key_T, typename Mapped_T>	
void Map<Key_T, Mapped_T>::SkipList::Node::                  
reset() {
	// XXX this->_print_trace("reset()", false);
	for(unsigned int i = 0; i < this->height; i++){
		this->forward[i] = nullptr; // just to be safe	
	}		
	this->height = 1;
	
	this->forward[0] = this;
	this->backward = this;
}


// DataNode--------------------------------------------------------------------
// constructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::DataNode::                  
DataNode(const ValueType &aPair, const int aHeight, unsigned long id) : 
				Node(aHeight, id),
		 		//21pair(this->copy_value(aPair.first, aPair.second)) {
				pair(aPair.first, aPair.second) {
	// XXX this->_print_trace2("DataNode()", false);
}


// copy ctor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::DataNode::                  
DataNode(const DataNode& n) : Node(n), 
						//21pair(this->copy_value(n.pair.first, n.pair.second)) {
						pair(n.pair.first, n.pair.second) {
	// XXX this->_print_trace2("DataNode(DataNode)", false);
}

// destructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::SkipList::DataNode::                  
~DataNode() {
	// XXX this->_print_trace2("~DataNode()", false);
}


// operator=
// TODO
// XXX operator=() XXX
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::SkipList::DataNode& 
Map<Key_T, Mapped_T>::SkipList::DataNode::
operator=(const DataNode& other) {
	// XXX this->_print_trace("operator=()", false);
	assert(0); // should never use
	DataNode second = DataNode(other);
	using std::swap;
	swap(this->height, second.height);
	//swap(this->id, second.id);
	swap(this->backward, second.backward);
	this->forward.swap(second.forward);

	return *this;
}

// copy_value
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::SkipList::ValueType                   
Map<Key_T, Mapped_T>::SkipList::DataNode::                  
copy_value(Key_T key, Mapped_T mapped) {
	//this->_print_trace2("copy_value()", false);  // XXX BUG  MEM ERROR
	return std::make_pair(key, mapped);
}




// debug---------------------------------------------------------------------
// Skiplist debug
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::SkipList::                  
_print_trace(std::string fn, bool focus) const {
	if(this->__print_trace){	
		if(focus){
			std::cout << "Skip\t";
		} else{		
			std::cout << "-----Skip\t";
		}
		std::cout << fn << std::endl;
	}
}

// XXX not really used
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::SkipList::                  
_check_sz_capacity(Node* n) const {
	assert(n->forward.size() <= this->curr_max_ht);
}

// Node debug
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::SkipList::Node::                  
_print_trace(std::string fn, bool focus) const {
	if(this->__print_trace){	
		if(focus){
			std::cout << "Node\t";
		} else{		
			std::cout << "-----Node\t";
		}
		std::cout << fn << std::endl;
	}
}


// DataNode debug
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::SkipList::DataNode::                 
_print_trace2(std::string fn, bool focus) const {
	if(this->__print_trace2){	
		if(focus){
			std::cout << "Dnode\t";
		} else{		
			std::cout << "-----Dnode\t";
		}
		std::cout << fn << std::endl;
	}
}

