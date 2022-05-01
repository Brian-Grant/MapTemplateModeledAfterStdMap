//#include "Map.hpp"

// Map comparison ---implemented as free functions
template<typename Key_T, typename Mapped_T>	
bool operator==(const Map<Key_T, Mapped_T> & first , 
				const Map<Key_T, Mapped_T> & second){
	if(first.size() != second.size()) return false;
	auto second_pos = second.begin();
	for(auto first_pos = first.begin(); first_pos != first.end(); ++first_pos){
		if((*first_pos) != (*second_pos)) return false;
		++second_pos;
	}
	return true;
}

template<typename Key_T, typename Mapped_T>
bool operator!=(const Map<Key_T, Mapped_T> & first, 
				const Map<Key_T, Mapped_T> & second){
	return !(first == second);

}

template<typename Key_T, typename Mapped_T>	
bool operator<(const Map<Key_T, Mapped_T> & first, 
			   const Map<Key_T, Mapped_T> & second){	
	if(first.size() > second.size()) return false;
	auto second_pos = second.begin();
	for(auto first_pos = first.begin(); first_pos != first.end(); ++first){
		if((*first_pos) < (*second_pos)) return true;
		else if((*first_pos) > (*second_pos)) return false;
		
		++second_pos;
	}
	if(first.size() < second.size()) return true;
	else return false;
}

	

