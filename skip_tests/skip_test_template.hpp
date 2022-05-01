#include "skip_test.hpp"



// SkipList--------------------------------------------------------------------

// constructor
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::                  // XXX
SkipList() : sentinel(new Node(1, 0, 31)), node_id(1), curr_max_ht(1), size(0), 
									//flip(std::random_device{}()), 
									flip(6627998), 
									coin(0.5), ht_for_one_billion(31),
									capacity(31) { // XXX flip(debug_seed)
	this->_print_trace("SkipList()", false);
	//this->sentinel->forward.push_back(this->sentinel);
	//this->sentinel->backward.push_back(this->sentinel);
	this->sentinel->forward[0] = (this->sentinel);
	this->sentinel->backward[0] = (this->sentinel);
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
	
	//this->_check_sz_capacity(this->sentinel);
}	

// destructor
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::                  // XXX
~SkipList(){
	this->_print_trace("~SkipList()", false);
	// TODO delete all nodes (HEIGHT)
	
	SkipList<Key_T, Mapped_T>::Node* tmp = this->sentinel->forward[0];
	while(tmp != this->sentinel){
		SkipList<Key_T, Mapped_T>::Node* del = tmp;
		tmp = tmp->forward[0];
		delete static_cast<DataNode *>(del);
	}
	
	delete this->sentinel;
}


// operator= 
// TODO


// print_list
template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
print_list(const typename SkipList<Key_T, Mapped_T>::Node &sent, bool frwd){     
	this->_print_trace("print_list(Node)", false);
	std::string out;		
	if(frwd){
		std::cout << "Printing list in " << "forward" << " order: " << std::endl;
		std::cout << sent.id << " max_h: " << this->curr_max_ht << " sz: " << 
			this->size << " height: " << 
			static_cast<const DataNode *>(&sent)->height << " " <<
			static_cast<const DataNode *>(&sent)->backward.size() << " " <<
			static_cast<const DataNode *>(&sent)->forward.size() << " " <<
			static_cast<const DataNode *>(&sent)->backward.capacity() << " " <<
			static_cast<const DataNode *>(&sent)->forward.capacity() <<  std::endl;    	
		for (Node *n = sent.forward[0]; n != &sent; n = n->forward[0]) {
        	std::cout << static_cast<DataNode *>(n)->id << "(" << 
			static_cast<DataNode *>(n)->pair.first << ", " <<
			static_cast<DataNode *>(n)->pair.second << ") " <<
			"\theight: " << static_cast<DataNode *>(n)->height << " " <<
			static_cast<DataNode *>(n)->backward.size() << " " <<
			static_cast<DataNode *>(n)->forward.size() << " " <<
			static_cast<DataNode *>(n)->backward.capacity() << " " <<
			static_cast<DataNode *>(n)->forward.capacity() <<  std::endl;
		}
	} else {
		
	}
}
	

template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
pretty_print_sentinel(){ 
	for(unsigned int i = 0; i < this->curr_max_ht; i++){
		std::cout << this->sentinel->id << "\t";
	}
	std::cout << "height: " << this->sentinel->height;
	std::cout << "\tsz: " << this->sentinel->forward.size();		
	std::cout << std::endl;
	for(unsigned int i = 0; i < this->curr_max_ht; i++){
		std::cout << this->sentinel->forward[i]->id << "\t";
	}
	std::cout << std::endl;
}


template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
pretty_print_dnode(typename SkipList<Key_T, Mapped_T>::Node* n){ 
	for(unsigned int i = 0; i < n->height; i++){		
		std::cout << static_cast<DataNode *>(n)->id << "\t";
	}
	for(unsigned int i = static_cast<DataNode *>(n)->height; 
							i < this->curr_max_ht; i++){
		std::cout << "\t";
	}
	std::cout << "height: " << static_cast<DataNode *>(n)->height;
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
void SkipList<Key_T, Mapped_T>::                  // XXX
pretty_print(){  
	this->_print_trace("print_list(Node)", false);		

	this->pretty_print_sentinel();
	for (Node *n = this->sentinel->forward[0]; 
									n != this->sentinel; n = n->forward[0]) {
	
		this->pretty_print_dnode(n);		
	}
	this->pretty_print_sentinel();
	std::cout << "this->size " << this->size << std::endl;
	std::cout << "this->curr_max_ht " << this->curr_max_ht << std::endl;
}



template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
verify(){ 
	//for (Node *n = sent.forward[0]; n != &sent; n = n->forward[0]) {
	SkipList<Key_T, Mapped_T>::Node* tmp = this->sentinel->forward[0];
	while(tmp != this->sentinel){
		SkipList<Key_T, Mapped_T>::Node* del = tmp;
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
void SkipList<Key_T, Mapped_T>::                  // XXX
show_time_complexity(){
	
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

	

	

	

	

}

// insert(Node*)
template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
insert(typename SkipList<Key_T, Mapped_T>::Node* n){                   // XXX
	this->_print_trace("insert(Node*)", false);
	// TODO 
	
}

// insert(ValueType)
// XXX full insert
template<typename Key_T, typename Mapped_T>	
typename SkipList<Key_T, Mapped_T>::InsertResult SkipList<Key_T, Mapped_T>::                 
insert(const ValueType &pair){
	this->_print_trace("insert(ValueType&)", false);
	std::vector<SkipList<Key_T, Mapped_T>::Node*> update(this->curr_max_ht);
	// gen num operations and position, and populate update
	typename SkipList<Key_T, Mapped_T>::SearchHelpResult info = 
											this->find_help(pair.first, update);
	this->insert_all_op_sum += info.second;
	this->insert_all_total++;	

	SkipList<Key_T, Mapped_T>::Node* pos = info.first;	
	if((pos != this->sentinel) && 
		(static_cast<DataNode *>(pos)->pair.first == pair.first))
	{	
		this->same++;
		return InsertResult(false, pos);
	
	} 
	if(pos == sentinel) std::cout << "in weird\n";
	else assert(static_cast<DataNode *>(pos)->pair.first > pair.first);
	
	this->insert_success_op_sum += info.second;
	this->insert_success_total++;

	SkipList<Key_T, Mapped_T>::Node* newNode = 
										
										this->insert_before(pair, update);
	return InsertResult(true, newNode);
}


//insert_before(pair, update)
// XXX place new node in correct position and return ptr to that node 
template<typename Key_T, typename Mapped_T>	
typename SkipList<Key_T, Mapped_T>::Node* SkipList<Key_T, Mapped_T>::                
insert_before(const ValueType & pair, 
					std::vector<SkipList<Key_T, Mapped_T>::Node*> &update){
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
	SkipList<Key_T, Mapped_T>::DataNode* newNode = 
								new DataNode(pair, rand_ht, this->node_id++);
	int end = rand_ht;
	if(rand_ht > old_max){
		end = old_max;		
		for(unsigned int i = old_max; i < rand_ht; i++){
			this->sentinel->forward[i] = newNode;
			newNode->forward[i] = this->sentinel;   //keep list of max heights
		}											// for erase
	}
	for(int i = 0; i < end; i++){
		newNode->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = newNode;
	}
	++this->size;
	return newNode;

}

// XXX XXX NOT IN USE
// insert_before(ValueType)
template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
insert_before(Node *pos, Node *n){
	this->_print_trace("insert_before(Node*, Node*)", false);
	// for the case of prior empty list
	if(this->curr_max_ht < n->height){
		std::cout << "new max ht____ old: "<< this->curr_max_ht << " new: "
													 << n->height << std::endl;		
		this->curr_max_ht = n->height;
	}	
	// for now ht 1	
    n->forward.push_back(pos);
    pos->backward[0]->forward[0] = n;
    pos->backward[0] = n;
	this->size++;	
}


// erase(Key)
template<typename Key_T, typename Mapped_T>	
bool SkipList<Key_T, Mapped_T>::                  // XXX
erase(const Key_T & key){
	//std::cout << "\nerase\n";	
	bool print = false;	
	this->_print_trace("erase(Key_T)", false);
	// TODO 
	std::vector<SkipList<Key_T, Mapped_T>::Node*> update(this->curr_max_ht);
	// gen num operations and position, and populate update
	typename SkipList<Key_T, Mapped_T>::SearchHelpResult info = 
											this->find_help(key, update);
	this->erase_all_op_sum += info.second;
	this->erase_all_total++;			

	SkipList<Key_T, Mapped_T>::Node* pos = info.first;	
	//if(pos->pair.first == key){
	if((pos != this->sentinel) && 
		(static_cast<DataNode *>(pos)->pair.first == key))
	{
		//for(unsigned int i = 0; i < (this->curr_max_ht-1); i++){
		for(unsigned int i = 0; i < this->curr_max_ht; i++){
			if(update[i]->forward[i] != pos) break;
			update[i]->forward[i] = pos->forward[i];
		}
	
		unsigned int top = this->curr_max_ht - 1;
		unsigned int tmpHt = this->curr_max_ht;		
		if(print) std::cout << "beforeHt " << this->curr_max_ht << std::endl;		
		if(print) std::cout << "beforeSize " << this->size << std::endl;		
		while(tmpHt > 1 && this->sentinel->forward[top] == pos){
			this->sentinel->forward[top] = nullptr;			
			tmpHt--;
			top--;
		}
		int diff;		
		assert(this->curr_max_ht == this->sentinel->height);		
		if(print) std::cout << "this->sentinel->forward.size() " << 
									this->sentinel->forward.size() << std::endl;
		//assert(this->curr_max_ht == this->sentinel->forward.size());		
		if((diff = this->curr_max_ht - tmpHt)){
			/*			
			for(int i = 0; i < diff; i++){
				this->sentinel->forward.pop_back();
			}
			*/	
			this->curr_max_ht = tmpHt;
			this->sentinel->height = tmpHt;
			assert(this->curr_max_ht == this->sentinel->height);
			//assert(this->curr_max_ht == this->sentinel->forward.size());	
		}		
		if(print) {
			std::cout << "newHt: " << tmpHt << std::endl;
			std::cout << "newSize " << this->size << std::endl;
			std::cout << "found/deleted " << pos->id << std::endl;
		}		
		delete pos;
		pos = nullptr;		
		this->size--;		
		
		this->erase_success_op_sum += info.second;
		this->erase_success_total++;					
		
		return true;

	} else {
		//std::cout << "not found\n";
		return false;
	}
}

// erase(Node)
template<typename Key_T, typename Mapped_T>	
void SkipList<Key_T, Mapped_T>::                  // XXX
erase(Node *n){
	this->_print_trace("erase(Node*)", false);
	// TODO 
	
}

//test

// find(Key, update)
// return num operations and position where key should be and UPDATE vector
template<typename Key_T, typename Mapped_T>	// pass in new vect already init
typename SkipList<Key_T, Mapped_T>::SearchHelpResult SkipList<Key_T, Mapped_T>::
find_help(const Key_T & key, 
			std::vector<SkipList<Key_T, Mapped_T>::Node*> &update){
	this->_print_trace("find(Key_T)", false);
	unsigned long local_op_count = 0;	
	SkipList<Key_T, Mapped_T>::Node* tmp = this->sentinel;
	for(int i = (this->curr_max_ht-1); i >= 0; i--){
		while(tmp->forward[i] != this->sentinel && 
			  static_cast<DataNode *>(tmp->forward[i])->pair.first < key)
		{	
			tmp = tmp->forward[i];
			local_op_count++;
		}
		update[i] = tmp;
	}
	tmp = tmp->forward[0];
	return SearchHelpResult(tmp, local_op_count);

}

// find_help(const Key_T & key)
// XXX return num ops and position of where key should be
template<typename Key_T, typename Mapped_T>	// pass in new vect
typename SkipList<Key_T, Mapped_T>::SearchHelpResult SkipList<Key_T, Mapped_T>::
find_help(const Key_T & key){
	this->_print_trace("find(Key_T)", false);
	unsigned long local_op_count = 0;	
	//update.resize(this->curr_max_ht);
	SkipList<Key_T, Mapped_T>::Node* tmp = this->sentinel;
	for(int i = (this->curr_max_ht-1); i >= 0; i--){
		while(tmp->forward[i] != this->sentinel && 
			  static_cast<DataNode *>(tmp->forward[i])->pair.first < key)
		{	
			tmp = tmp->forward[i];
			local_op_count++;
		}
		//update[i] = tmp;
	}
	tmp = tmp->forward[0];
	return SearchHelpResult(tmp, local_op_count);

}


// find(Key)
// XXX return a position and a bool indicating if found
template<typename Key_T, typename Mapped_T>	
typename SkipList<Key_T, Mapped_T>::SearchResult SkipList<Key_T, Mapped_T>::
find(const Key_T & key){                   // XXX ABOVE
	this->_print_trace("find(Key_T)", false);
	std::vector<SkipList<Key_T, Mapped_T>::Node*> update(this->curr_max_ht);
	if(!this->size){
		return SearchResult(false, this->sentinel); // TODO think through
	}
	// get num operations, and position
	typename SkipList<Key_T, Mapped_T>::SearchHelpResult info = 
												this->find_help(key);
	this->find_all_op_sum += info.second;
	this->find_all_total++;		

	SkipList<Key_T, Mapped_T>::Node* pos = info.first;	
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
	
}


// get_rand_height()
template<typename Key_T, typename Mapped_T>	
unsigned int SkipList<Key_T, Mapped_T>::                  // XXX
get_rand_height(){
	this->_print_trace("get_rand_height()", false);
	unsigned int height = 1;
	while(this->coin(flip)){
		++height;
	}
	//std::cout << "Height: " << height << std::endl;
	return height;
}



// XXX XXX Node XXX XXX-------------------------------------------------------

// XXX Node(height) XXX
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::Node::                  // XXX
Node(int aHeight, unsigned long aId) : height(aHeight), id(aId)  {
	this->_print_trace("Node(height)", false);
	this->forward.resize(aHeight);
	this->backward.resize(aHeight);
/*	
	std::cout << "backward-size: " << backward.size() << std::endl;
	std::cout << "forward-size: " << forward.size() << std::endl;
	std::cout << "backward-capacity: " << backward.capacity() << std::endl;
	std::cout << "forward-capacity: " << forward.capacity() << std::endl;
*/
}

// XXX Node(height, capacity) XXX USED FOR HEAD NODE CONSTRUCTOR IN SKIPLIST
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::Node::                  // XXX
Node(int aHeight, unsigned long aId, int capacity) : height(aHeight), id(aId) { 
	this->_print_trace("Node(height, capacity)", false);
	this->forward.resize(capacity);
	this->backward.resize(capacity);	
/*	
	std::cout << "backward-size: " << backward.size() << std::endl;
	std::cout << "forward-size: " << forward.size() << std::endl;
	std::cout << "backward-capacity: " << backward.capacity() << std::endl;
	std::cout << "forward-capacity: " << forward.capacity() << std::endl;
*/

}

// XXX destructor XXX
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::Node::                  // XXX
~Node() {
	this->_print_trace("~Node()", false);
	//delete this->sentinel;  // XXX wrong place
}

// operator=
//TODO



// DataNode--------------------------------------------------------------------
// constructor
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::DataNode::                  // XXX
DataNode(const ValueType &aPair, const int aHeight, unsigned long id) : 
															Node(aHeight, id),
		 pair(this->copy_value(aPair.first, aPair.second)) {
	this->_print_trace2("DataNode()", false);
}

// destructor
template<typename Key_T, typename Mapped_T>	
SkipList<Key_T, Mapped_T>::DataNode::                  // XXX
~DataNode() {
	this->_print_trace2("~DataNode()", false);
}


// operator=
// TODO


// copy_value
template<typename Key_T, typename Mapped_T>	
typename SkipList<Key_T, Mapped_T>::ValueType                   // XXX
SkipList<Key_T, Mapped_T>::DataNode::                  // XXX
copy_value(Key_T key, Mapped_T mapped) {
	//this->_print_trace2("copy_value()", false);  // BUG  MEM ERROR
	return std::make_pair(key, mapped);
}




// debug---------------------------------------------------------------------
// Skiplist debug
template<typename Key_T, typename Mapped_T>
void SkipList<Key_T, Mapped_T>::                  // XXX
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

template<typename Key_T, typename Mapped_T>
void SkipList<Key_T, Mapped_T>::                  // XXX
_check_sz_capacity(Node* n) const {
	assert(n->forward.size() == n->backward.size());
	assert(n->forward.capacity() == n->backward.capacity());
	assert(n->forward.size() <= this->curr_max_ht);

}

// Node debug
template<typename Key_T, typename Mapped_T>
void SkipList<Key_T, Mapped_T>::Node::                  // XXX
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
void SkipList<Key_T, Mapped_T>::DataNode::                  // XXX
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

