#include "skip_test.hpp"
#include <stdlib.h>
#include <map>
#include <limits.h>

typedef std::pair<int, std::string> mPair; // XXX
typedef SkipList<int, std::string> sk;

void _check_coin(sk &l);




int 
main(int argc, char** argv){
	sk list;
	std::cout << "limit ULLONG_MAX " << ULLONG_MAX << std::endl;
	std::cout << "limit ULONG_MAX " << ULONG_MAX << std::endl;

{	
	char *ptr;
	unsigned long low = strtoul(argv[1], &ptr, 10);
	unsigned long high = strtoul(argv[2], &ptr, 10);;	
	bool toBrk = false;
	unsigned long brkPt = 100;
	unsigned long interval = ((high - low) / 2) + ((high - low) / 6);
	std::cout << "interval " << interval << " " << 
		((double)interval)/((double)(high-low)) << std::endl;


	int print = atoi(argv[3]);
	int pretty = 0;

	std::uniform_int_distribution<> unif(low,high);
	//std::mt19937_64 re(std::random_device{}());
	std::mt19937_64 re;	
	//re.seed(0);
	
	if(argc == 4){
		re.seed(std::random_device{}());
	}
	else{
			std::cout << "yes\n";	
		int sd = atoi(argv[4]);	
		re.seed(sd);	
	}
	
	int in;
	char l = 'a';
	
	unsigned long brk = 2;
	std::vector<int> placed;
	for(unsigned long i = 0; i < interval; i++){
		if(i == brk){
			std::cout << i << std::endl;
			brk*=2;
		}		
		if(toBrk && (i == brkPt)) exit(0);
		
		in = unif(re);		
		std::string ss(1, l);
		mPair pp(in, ss);		

		while(!(list.insert(pp)).first){
			in = unif(re);
			pp = std::make_pair(in, ss);
		}
		placed.push_back(in);
		//list.print_list(*(list.sentinel), true);
		//list.pretty_print();
		//list.verify();
		if(l == 'z') l = 'a';
		else l = l+1;
	

	}
	
	//list.verify();	
	if(pretty) list.pretty_print();

	
	int idx;
	brk = 2;
	unsigned long erase_end = (interval/4);
	std::cout << "erase_end " << erase_end << std::endl;
	for(unsigned long i = 0; i < erase_end; i++){	
				
		//while(!list.erase(in = unif(re)));
		idx = unif(re)%placed.size();
		in = placed[idx];		
		list.erase(in);
		placed.erase(placed.begin()+idx);
		if(i == brk){
			std::cout << i << std::endl;
			brk*=2;
			if(print) std::cout << "erased " << in << std::endl;
			if(print) std::cout << "count erase " << i << std::endl;
		}		
		
		//list.verify();	
		//if(print) list.pretty_print();
	}
	std::cout << "erase over----count " << erase_end << std::endl;
	list.verify();
	if(pretty) list.pretty_print();
	

	std::cout << "\n\nfind\n\n";	
	
	brk=2;	

	for(unsigned long i = 0; i < interval; i ++){
		
		//std::cout << "i " << i << std::endl;		
		//in = unif(re);
		idx = unif(re)%placed.size();
		in = placed[idx];	
				
		SkipList<int, std::string>::SearchResult res(list.find(in));
		
		if(i == brk){
			if(print) {
				std::cout << "\t\tbrk " << i << std::endl;
				brk*=2;
			
				if(res.first){
					std::cout << "found id: " << (res.second)->id << std::endl;
				}
				std::cout << "in " << in << std::endl;
			}
		}	
		
	}	
	
	list.verify();
	if(pretty) list.pretty_print();
	
	std::cout << "\nthis->curr_max_ht " << list.curr_max_ht << std::endl;	
	std::cout << "\nsame_insert try " << list.same << std::endl;		
	std::cout << "this->size " << list.size << std::endl;	
	
/*
	std::cout << "insert sum " << list.insert_op_sum << std::endl;
	std::cout << "lg(this->size) " << std::log2(list.size) << std::endl;
	std::cout << "insert_op/size " << 
		((double)list.insert_op_sum)/((double)list.size) << std::endl;
	
	std::cout << "\nfound " << list.found_total << std::endl;	
	std::cout << "find_op_sum: " << list.find_op_sum << std::endl;	
	std::cout << "lg(found) " << std::log2(list.found_total) << std::endl;
	std::cout << "find_op/found " << 
				((double)list.find_op_sum)/((double)list.found_total) 
				<< std::endl;
	std::cout << "end\n";
*/
	list.show_time_complexity();	
	
}



	//_check_coin(list);

	std::cout << "end" << std::endl;

}

void _check_coin(sk &l){
	int end = 1000000000;
	std::map<int, int> hist;
    for(int n=0; n<end; ++n) {
        ++hist[l.get_rand_height()];
    }
	std::cout.precision(10);
	double d;    
	for(auto p : hist) {
		d = (double)p.second/(double)end;       
		std::cout << p.first
                  << ' ' << p.second << " " << std::fixed << d << '\n';
	}

}




