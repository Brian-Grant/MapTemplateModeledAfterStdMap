//#include "Map.hpp"


// copy ctor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::Iterator::
Iterator(const Iterator &it) : loc(it.loc) {
	// XXX this->_print_trace("Iterator(const Iterator &)", false);
	
}


// destructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::Iterator::
~Iterator(){
	// XXX this->_print_trace("~Iterator()", false);
}


// operator=
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::
operator=(const Iterator& it){
	// XXX this->_print_trace("operator=", false);
	this->loc = it.loc;
	return *this;
}

// Node* constructor

template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::Iterator::
Iterator(typename Map<Key_T, Mapped_T>::SkipList::Node* at) : loc(at) {
	// XXX this->_print_trace("Iterator(Node...)", false);
}


// operator++()  prefix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::
operator++(){
	// XXX this->_print_trace("operator++()", false);
	this->loc = this->loc->forward[0];
	return *this;
}


// operator++(int)  postfix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::
operator++(int){
	// XXX this->_print_trace("operator++(int)", false);
	typename Map<Key_T, Mapped_T>::Iterator it(*this);
	this->loc = this->loc->forward[0];
	return it;
}


// operator--()  prefix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::Iterator& Map<Key_T, Mapped_T>::Iterator::
operator--(){
	// XXX this->_print_trace("operator--()", false);
	this->loc = this->loc->backward;
	return *this;
}


// operator--(int)  postfix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::Iterator Map<Key_T, Mapped_T>::Iterator::
operator--(int){
	// XXX this->_print_trace("operator--(int)", false);	
	typename Map<Key_T, Mapped_T>::Iterator it(*this);
	this->loc = this->loc->backward;
	return it;
}

//operator*() const
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::Iterator::
operator*() const {
	// XXX this->_print_trace("operator*() const", false);
	return static_cast<typename SkipList::DataNode *>(this->loc)->pair;
}

// operator->() const
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ValueType *Map<Key_T, Mapped_T>::Iterator::
operator->() const {
	// XXX this->_print_trace("operator->() const", false);
	return &static_cast<typename SkipList::DataNode *>(this->loc)->pair;
}


//debug 
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::Iterator::
_print_trace(std::string fn, bool focus) const {
	if(this->__print_trace){	
		if(focus){
			std::cout << "It\t\t";
		} else{		
			std::cout << "-----It\t\t";
		}
		std::cout << fn << std::endl;
	}
}
