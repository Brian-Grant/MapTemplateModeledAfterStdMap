#include "Map.hpp"

#include <algorithm> 

template class cs540::Map<int, std::string>;
typedef cs540::Map<int, std::string> Mmap;
//typedef std::pair<const int, std::string> MapPair;
typedef std::pair<int, std::string> MapPair;
typedef std::vector<MapPair> Pairs;

void 
populate_vector(Pairs &in, unsigned long amount, bool shuffle, bool print);
void print_pairs(Pairs &in);
void print_pair(MapPair &pair);
void pretty_print_wrapper(Mmap& map, std::string var);


int main(){
	


	unsigned long low = 0;
	unsigned long high = 100;
	unsigned long end = 200;	
	
	std::uniform_int_distribution<> unif(low,high);
	//std::mt19937_64 re(std::random_device{}());
	std::mt19937_64 re;	
	re.seed(0);
	Pairs pairs;
	Pairs inserted;
	bool print = true;
	bool shuffle = true;	
	
	populate_vector(pairs, end, shuffle, !print);
	

	

	printf("XXX map1 default construct\n");	
	Mmap map1;
	
	printf("XXX map1 size on empty\n");
	assert(map1.size() == 0);
	
	printf("XXX map1 empty on empty\n");	
	assert(map1.empty());
	
	printf("XXX map1 find on empty\n");	
	Mmap::Iterator it1 = map1.find(1);
	assert(it1.loc == map1.data.sentinel);
	
	printf("XXX map1 insert(it, it)\n");
	auto it_beg = pairs.begin();
	auto it_end = pairs.end();	
	map1.insert(it_beg, it_end);
	
	for(auto it = map1.begin(); it != map1.end(); ++it){
		
	}
	
	
	//add to inserted
	//inserted.insert(inserted.end(), it_beg, it_end);
	//pairs.erase(it_beg, it_end);	
	//std::shuffle(std::begin(inserted), std::end(inserted), re);

	//print
	pretty_print_wrapper(map1, "map1");
	

	map1.data.verify_both();
	
	
/*		
	std::cout << "going to erase: " << inserted.back().first << std::endl;
	map1.erase(inserted.back().first);
	
	pairs.push_back(inserted.back());
	inserted.pop_back();
	map1.data.verify_both();

	//print
	pretty_print_wrapper(map1, "map1");
	
	
	const Mmap& map1c = map1;
	auto findRes1 = map1c.find(1);
	//std::cout << "find res " << findRes1->first << std::endl;
	
	std::cout << "XXX map2 copy constructor---------------------" << std::endl;
	Mmap map2(map1);
	std::cout << "XXX map2 copy constructor end-----------------" << std::endl;
	//print
	pretty_print_wrapper(map2, "map2");

	map2.data.verify_both();
	

	it_beg = pairs.begin();
	it_end = it_beg+5;
	
	map2.insert(it_beg, it_end);
	
	
	//print
	
	pretty_print_wrapper(map2, "map2");

	map2.data.verify_both();
	
	Mmap map3({{44, "yo"}, {99, "hi"}});
	
	//print
	pretty_print_wrapper(map3, "map3");
	
	map3 = map1;
*/
	std::cout << "@@@@@@ Done @@@@@@" << std::endl;
	return 0;

	
}


void 
populate_vector(Pairs &in, unsigned long amount, bool shuffle, bool print){
	int key_in;
	char l = 'a';
	int ctr = 0;	
	if(print){	// looks redundant but saves conditional checks in the loop 
		for(unsigned long i = 0; i < amount; i++){
			if((ctr%2)==0) key_in = ctr+1;
			else if((i%3)==0) key_in = ctr-1;
			else key_in = ctr;
			std::cout << i << "\t\t(" << key_in << ", " << l << ")" << std::endl;
			std::string ss(1, l);
			in.push_back(MapPair(key_in, ss));
			if(l == 'z') l = 'a';
			else l = l+1;
			ctr+=3;
		}
		if(shuffle){
			std::mt19937_64 re;	
			re.seed(0);
			//re.seed(std::random_device{}());
			std::shuffle(std::begin(in), std::end(in), re);
			print_pairs(in);
		}
		std::cout << "Size: " << in.size() << std::endl;
	} else{
		for(unsigned long i = 0; i < amount; i++){
			if((ctr%2)==0) key_in = ctr+1;
			else if((i%3)==0) key_in = ctr-1;
			else key_in = ctr;
			std::string ss(1, l);
			in.push_back(MapPair(key_in, ss));
			if(l == 'z') l = 'a';
			else l = l+1;
			ctr+=3;
		}
	}

}


void print_pairs(Pairs &in){
	std::cout << "_______print_pairs_________" << std::endl;	
	for(unsigned long i = 0; i < in.size(); i++){
		std::cout << i << "\t\t";
		print_pair(in[i]);
	}
	std::cout << "_______end_print_pairs______" << std::endl;
}

void print_pair(MapPair &pair){
	std::cout << "(" << pair.first << ", " 
	<< pair.second << ")" << std::endl;
}

void pretty_print_wrapper(Mmap& map, std::string var){
	map.data.__print_trace = false;
	std::cout << "XXX pretty_print()\t\t" << var << std::endl;	
	map.data.pretty_print();
	map.data.__print_trace = true;

}

