//#include "Map.hpp"


// copy ctor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ConstIterator::
ConstIterator(const ConstIterator &it) : Iterator(it) {
	// XXX this->_print_trace("ConstIterator(const ConstIterator &)", false);
	// TODO
}

// copy ctor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ConstIterator::
ConstIterator(const Iterator &it) : Iterator(it) {
	// XXX this->_print_trace("ConstIterator(const Iterator &)", false);
}

// destructor
template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ConstIterator::
~ConstIterator(){
	// XXX this->_print_trace("~ConstIterator()", false);
}


// operator=
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ConstIterator& Map<Key_T, Mapped_T>::ConstIterator::
operator=(const ConstIterator& it){
	// XXX this->_print_trace("operator=", false);
	this->loc = it.loc;
	return *this;
}

// Node* constructor

template<typename Key_T, typename Mapped_T>	
Map<Key_T, Mapped_T>::ConstIterator::
ConstIterator(typename Map<Key_T, Mapped_T>::SkipList::Node* at) : Iterator(at) {
	// XXX this->_print_trace("ConstIterator(Node...)", false);
}


// operator++()  prefix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ConstIterator& Map<Key_T, Mapped_T>::ConstIterator::
operator++(){
	// XXX this->_print_trace("operator++()", false);
	this->loc = this->loc->forward[0];
	return *this;
}


// operator++(int)  postfix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::ConstIterator::
operator++(int){
	// XXX this->_print_trace("operator++(int)", false);
	typename Map<Key_T, Mapped_T>::ConstIterator it(*this);
	this->loc = this->loc->forward[0];
	return it;
}


// operator--()  prefix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ConstIterator& Map<Key_T, Mapped_T>::ConstIterator::
operator--(){
	// XXX this->_print_trace("operator--()", false);
	this->loc = this->loc->backward;
	return *this;
}


// operator--(int)  postfix
template<typename Key_T, typename Mapped_T>	
typename Map<Key_T, Mapped_T>::ConstIterator Map<Key_T, Mapped_T>::ConstIterator::
operator--(int){
	// XXX this->_print_trace("operator--(int)", false);	
	typename Map<Key_T, Mapped_T>::ConstIterator it(*this);
	this->loc = this->loc->backward;
	return it;
}

//operator*() const
template<typename Key_T, typename Mapped_T>	
const typename Map<Key_T, Mapped_T>::ValueType &Map<Key_T, Mapped_T>::ConstIterator::
operator*() const {
	// XXX this->_print_trace("operator*() const", false);
	return static_cast<typename SkipList::DataNode *>(this->loc)->pair;
}

// operator->() const
template<typename Key_T, typename Mapped_T>	
const typename Map<Key_T, Mapped_T>::ValueType *Map<Key_T, Mapped_T>::ConstIterator::
operator->() const {
	// XXX this->_print_trace("operator->() const", false);
	return &static_cast<typename SkipList::DataNode *>(this->loc)->pair;
}




//debug 
template<typename Key_T, typename Mapped_T>
void Map<Key_T, Mapped_T>::ConstIterator::
_print_trace(std::string fn, bool focus) const {
	if(this->__print_trace){	
		if(focus){
			std::cout << "CnstIt\t";
		} else{		
			std::cout << "-----CnstIt\t";
		}
		std::cout << fn << std::endl;
	}
}

