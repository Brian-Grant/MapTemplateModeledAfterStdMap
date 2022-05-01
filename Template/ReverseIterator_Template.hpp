//#include "Map.hpp"


// copy ctor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ReverseIterator::
ReverseIterator(const ReverseIterator &it) : Iterator(it) {
	// XXX this->_print_trace("ReverseIterator(const ReverseIterator &)", false);
	
}


// destructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ReverseIterator::
~ReverseIterator(){
	// XXX this->_print_trace("~ReverseIterator()", false);
}


// operator=
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ReverseIterator& Map<Key_T, Mapped_T>::ReverseIterator::
operator=(const ReverseIterator& it){
	// XXX this->_print_trace("operator=", false);
	this->loc = it.loc;
	return *this;
}

// Node* constructor

template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ReverseIterator::
ReverseIterator(typename Map<Key_T, Mapped_T>::SkipList::Node* at) : Iterator(at) {
	// XXX this->_print_trace("ReverseIterator(Node...)", false);
}


// operator++()  prefix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ReverseIterator& Map<Key_T, Mapped_T>::ReverseIterator::
operator++(){
	// XXX this->_print_trace("operator++()", false);
	this->loc = this->loc->backward;
	return *this;
}


// operator++(int)  postfix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::ReverseIterator::
operator++(int){
	// XXX this->_print_trace("operator++(int)", false);
	typename Map<Key_T, Mapped_T>::ReverseIterator it(*this);
	this->loc = this->loc->backward;
	return it;
}


// operator--()  prefix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ReverseIterator& Map<Key_T, Mapped_T>::ReverseIterator::
operator--(){
	// XXX this->_print_trace("operator--()", false);
	this->loc = this->loc->forward[0];
	return *this;
}


// operator--(int)  postfix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ReverseIterator Map<Key_T, Mapped_T>::ReverseIterator::
operator--(int){
	// XXX this->_print_trace("operator--(int)", false);	
	typename Map<Key_T, Mapped_T>::ReverseIterator it(*this);
	this->loc = this->loc->forward[0];
	return it;
}

//operator*() const
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::ReverseIterator::
operator*() const {
	// XXX this->_print_trace("operator*() const", false);
	return static_cast<typename SkipList::DataNode *>(this->loc)->pair;
}

// operator->() const
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ValueType *Map<Key_T, Mapped_T>::ReverseIterator::
operator->() const {
	// XXX this->_print_trace("operator->() const", false);
	return &static_cast<typename SkipList::DataNode *>(this->loc)->pair;
}



//debug 
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::ReverseIterator::
_print_trace(std::string fn, bool focus) const {
	if(this->__print_trace){	
		if(focus){
			std::cout << "RevIt\t";
		} else{		
			std::cout << "-----RevIt\t";
		}
		std::cout << fn << std::endl;
	}
}
