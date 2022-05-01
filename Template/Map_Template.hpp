#include "Map.hpp"



// Map implementation
// constructors and assignment

//default ctor	
template<typename Key_T, typename Mapped_T>
Map<Key_T, Mapped_T>::
Map() : data(SkipList()), length(0) {
	// XXX this->_print_trace("Map()", false);
}

// copy ctor
template<typename Key_T, typename Mapped_T> 
Map<Key_T, Mapped_T>::
Map(const Map &other) : data(other.data), length(other.length) {
	// XXX this->_print_trace("Map(const Map &other", false);	
}

// assignment
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>& Map<Key_T, Mapped_T>::
operator=(const Map &other){
	// XXX this->_print_trace("operator=()", false);
	Map second(other);
	using std::swap;
	swap(this->length, second.length);
	swap(this->data, second.data);
	
	return *this;
}	

// initializer list ctor
template<typename Key_T, typename Mapped_T>
Map<Key_T, Mapped_T>::	
Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> list){
	// XXX this->_print_trace("Map(std::initializer_list...)", false);
	this->insert(list.begin(), list.end());
}	

// destructor
template<typename Key_T, typename Mapped_T>
Map<Key_T, Mapped_T>::
~Map(){
	// XXX this->_print_trace("~Map()", false);	
}
	
// size 
template<typename Key_T, typename Mapped_T>
size_t Map<Key_T, Mapped_T>::
size() const{
	// XXX this->_print_trace("size()", false);
	return this->data.size;
}

// empty
template<typename Key_T, typename Mapped_T>
bool Map<Key_T, Mapped_T>::
empty() const{
	// XXX this->_print_trace("empty()", false);
	if(!this->data.size) return true;
	else return false;
}


// Iterator begin()
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::
begin(){
	// XXX this->_print_trace("begin()->it", false);
	if(this->empty()){ 
		return 
		typename Map<Key_T, Mapped_T>::Iterator::Iterator(this->data.sentinel);
	}
	else {
		// XXX assert(this->data.sentinel->forward[0] != this->data.sentinel);
		return 
		typename Map<Key_T, Mapped_T>::Iterator::Iterator
											(this->data.sentinel->forward[0]);
	}

}

// Iterator end()
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::
end(){
	// XXX this->_print_trace("end()->it", false);
	return 
	typename Map<Key_T, Mapped_T>::Iterator::Iterator(this->data.sentinel);
}

// ConstIterator begin()
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::
begin() const {
	// XXX this->_print_trace("begin()->cnstIt", false);
	if(this->empty()){ 
		return 
		typename 
		Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(this->data.sentinel);
	}
	else {
		// XXX assert(this->data.sentinel->forward[0] != this->data.sentinel);
		return 
		typename Map<Key_T, Mapped_T>::ConstIterator::ConstIterator
											(this->data.sentinel->forward[0]);
	}

}

// ConstIterator end()
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::
end() const {
	// XXX this->_print_trace("end()->cnstIt", false);
	return 
	typename 	
	Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(this->data.sentinel);
}



// RevIterator begin()
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::
rbegin(){
	// XXX this->_print_trace("rbegin()->revIt", false);
	if(this->empty()){ 
		return typename 
		Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator
														(this->data.sentinel);
	}
	else {
		// XXX assert(this->data.sentinel->backward != this->data.sentinel);
		return typename 
		Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator
												(this->data.sentinel->backward);
	}

}

// RevIterator end()
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::
rend(){
	// XXX this->_print_trace("rend()->revIt", false);
	return typename 
	Map<Key_T, Mapped_T>::ReverseIterator::ReverseIterator(this->data.sentinel);
}


//.......

// find -> it
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::
find(const Key_T & key){
	// XXX this->_print_trace("find(key)", false);	
	auto find_res(this->data.find(key));
	if(find_res.first){
		return typename 
		Map<Key_T, Mapped_T>::Iterator::Iterator(find_res.second);
	} else{
		return typename 
		Map<Key_T, Mapped_T>::Iterator::Iterator(this->data.sentinel);
	}

}

// find -> cnstIt
template<typename Key_T, typename Mapped_T>
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::
find(const Key_T & key) const {
	// XXX this->_print_trace("find(key) const", false);	
	auto find_res(this->data.find(key));
	if(find_res.first){
		return typename 
		Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(find_res.second);
	} else{
		return typename 
		Map<Key_T, Mapped_T>::ConstIterator::ConstIterator(this->data.sentinel);
	}
}

// at 
template<typename Key_T, typename Mapped_T>
Mapped_T& Map<Key_T, Mapped_T>::
at(const Key_T &key){
	// XXX this->_print_trace("at(key)", false);	
	auto find_res(this->data.find(key));
	if(find_res.first){
		return 
		static_cast<typename SkipList::DataNode *>(find_res.second)->pair.second;
	}
	else{
		std::string msg = "out of range exception in at()";		
		throw std::out_of_range(msg);
	}

}

// at -> const 
template<typename Key_T, typename Mapped_T>
const Mapped_T& Map<Key_T, Mapped_T>::
at(const Key_T &key) const {
	// XXX this->_print_trace("at(key) const", false);	
	auto find_res(this->data.find(key));
	if(find_res.first){
		return static_cast<typename SkipList::DataNode *>(find_res.second)->pair.second;
	}
	else{
		std::string msg = "out of range exception in at()";		
		throw std::out_of_range(msg);
	}

}

// operator[]
template<typename Key_T, typename Mapped_T>
Mapped_T& Map<Key_T, Mapped_T>::
operator[](const Key_T &key){
	// XXX this->_print_trace("operator[]", false);	
	Mapped_T m {};	
	ValueType v(key, m);
	//std::cout << "m " << m << std::endl;	
	//std::cout << "[] " << v.first << " " << key << std::endl; 
	//this->data.pretty_print(); // XXX debug
	typename SkipList::InsertResult res = this->data.insert(v);
	if(!res.first){ // found
		
	} else{ // no found, inserted

	}	
	return static_cast<typename SkipList::DataNode *>(res.second)->pair.second;
	
}

// insert(valtype)
template<typename Key_T, typename Mapped_T>
std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> Map<Key_T, Mapped_T>::
insert(const ValueType &pair){
	// XXX this->_print_trace("insert(ValueType)", false);
	typename SkipList::InsertResult res = this->data.insert(pair);
	// XXX typename SkipList::DataNode* n;
	// XXX n = static_cast<typename SkipList::DataNode *>(res.second);
	bool bool_ret;	
	if(res.first){
		bool_ret = true;				
		// XXX assert(n->pair.first == pair.first);
		// XXX assert(n->pair.second == pair.second);
	} else{ 
		bool_ret = false; 
		// XXX assert(n->pair.first == pair.first);
	}
	//if(res.second == this->data.sentinel) this->data.pretty_print_sentinel();
	//else this->data.pretty_print_dnode(res.second);
	typename Map<Key_T, Mapped_T>::Iterator it(res.second);
	//std::pair<typename Map<Key_T, Mapped_T>::Iterator, bool> 
				//ret = std::make_pair(it, bool_ret);
				
	return {it, bool_ret};
}

// insert(it_beg, it_end)
template<typename Key_T, typename Mapped_T>
template<typename IT_T>
void Map<Key_T, Mapped_T>::
insert(IT_T range_beg, IT_T range_end){
	// XXX this->_print_trace("insert(begin, end)", false);
	// TODO what does only one constructor call mean
	for(auto it = range_beg; it != range_end; ++it){
		//std::cout << "**************************" << std::endl;		
		auto pairRet = this->insert(ValueType(it->first, it->second));
		//if(pairRet.second) std::cout << "inserted: ";
		//else std::cout << "not inserted: ";
		//std::cout << (pairRet.first)->first << std::endl;
		// XXX assert((pairRet.first)->first == it->first);
		//std::cout << "**************************" << std::endl;	
		
	}
	//this->_print_trace("LEAVING insert(begin, end)", false); 
}

// erase(it)
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::
erase(typename Map<Key_T, Mapped_T>::Iterator pos) {
	//assert( &(*pos) != this->data.sentinel);
	// XXX this->_print_trace("erase(Iterator)", false);	
	this->data.erase((*pos).first);

}

// erase(key)
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::
erase(const Key_T &key) {
	//assert((*pos).loc != this->data.sentinel);
	// XXX this->_print_trace("erase(key)", false);	
	this->data.erase(key);
}


// clear
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::
clear() {
	// XXX this->_print_trace("clear()", false);
	this->data.reset();
	this->length = 0;
}

// tests ---------------------------------------------------------------------
/*
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::
_see_insert(std::pair<typename Map<Key_T, Mapped_T>::Iterator, 
															bool> &out) const {
	this->_print_trace("_see_insert(out)", false);	
	if(out.first.loc == this->data.sentinel) {
		std::cout << "problem in insert" << std::endl;		
	}
	else {
		if(out.second){		
			std::cout << "inserted" << std::endl;
		} else {		
			std::cout << "already in map" << std::endl;	
		}	
		this->data.pretty_print_dnode(out.first.loc);
	}
}
*/
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::
_print_trace(std::string fn, bool focus) const {
	if(this->__print_trace){	
		if(focus){
			std::cout << "Map\t";
		} else{		
			std::cout << "-----Map\t";
		}
		std::cout << fn << std::endl;
	}
}


