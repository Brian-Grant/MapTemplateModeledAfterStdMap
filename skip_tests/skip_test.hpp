#ifndef SKIP_TEST
#define SKIP_TEST

#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <random>
#include <cmath>


// SkipList-------------------------------------------------------------------
template<typename Key_T, typename Mapped_T>
	class SkipList {
	typedef std::pair<const Key_T, Mapped_T> ValueType; // XXX
	public:	
		class Node;	
		class DataNode;
		
		//typedef std::pair<bool, Node*> SearchResult;		
		typedef std::pair<bool, Node*> SearchResult;
		typedef std::pair<Node*, unsigned long> SearchHelpResult;
		typedef std::pair<bool, Node*> InsertResult;
		SkipList();
		~SkipList();
		SkipList& operator=(const SkipList &);
		// Template specializations.
		
	
		void pretty_print_sentinel();
		void pretty_print_dnode(Node *);
		void pretty_print();
		void print_list(const Node &sent, bool frwd);
		void verify();
		void show_time_complexity();
			
		void insert(Node*);
		InsertResult insert(const ValueType &);
	//protected:	// XXX OLD
	public: // XXX NEW			
		void insert_before(Node *, Node *);
		//Node *pos, 		
		Node* insert_before(const ValueType & pair, 
												std::vector<Node*> &update);
		
	public:
		bool erase(const Key_T &);
	private:			
		void erase(Node*);
	public:
		SearchHelpResult find_help(const Key_T & key, std::vector<Node*> &update);
		SearchHelpResult find_help(const Key_T & key);	
			
		SearchResult find(const Key_T &);
	//private: //XXX old
	public: //XXX	new
		unsigned int get_rand_height();
	public:
	// data members			
		Node* sentinel;
		unsigned long node_id;		
		unsigned int curr_max_ht;
   		int size; // XXX
		std::mt19937 flip;
		std::bernoulli_distribution coin;
		int same; 
		unsigned long insert_success_op_sum;
		unsigned long insert_success_total;
		unsigned long insert_all_op_sum;
		unsigned long insert_all_total;		
		
		unsigned long find_success_op_sum;
		unsigned long find_success_total;
		unsigned long find_all_op_sum;
		unsigned long find_all_total;
		
		unsigned long erase_success_op_sum;
		unsigned long erase_success_total;
		unsigned long erase_all_op_sum;
		unsigned long erase_all_total;


	private:
		const int ht_for_one_billion; // rough height for a list 
	public:							  // with one billion objects
		unsigned int capacity;	
	
	private:		
		bool __print_trace = false;
		void _print_trace(std::string fn, bool focus) const;
	public:
		void _check_sz_capacity(Node*) const; 
		
	};

// Node ----------------------------------------------------------------------
	template<typename Key_T, typename Mapped_T>
	//class SkipList<Key_T, Mapped_T>::Node { // XXX
	class SkipList<Key_T, Mapped_T>::Node { // XXX
	public:				
		Node(int aHeight, unsigned long aId);				
		Node(int aHeight, unsigned long aId, int capacity);		
		~Node();
		Node& operator=(const Node &);

		unsigned int height;		
		const unsigned long id;		
				
		std::vector<Node*> backward;
		std::vector<Node*> forward;
	protected:		
		bool __print_trace = false;
		//virtual void _print_trace(std::string fn, bool focus) const; // XXX
		void _print_trace(std::string fn, bool focus) const; // XXX virtual

		
	};

// DataNode-------------------------------------------------------------------
	template<typename Key_T, typename Mapped_T>
	//class SkipList<Key_T, Mapped_T>::DataNode : Node { // XXX
	class SkipList<Key_T, Mapped_T>::DataNode : public Node { // XXX
	public:
			DataNode() = delete;
			DataNode(const ValueType &aPair, const int aHeight, 
										unsigned long id); // TODO pass in vect?
			~DataNode();
			DataNode& operator=(const DataNode &);
				
			ValueType pair;
		private:
			ValueType copy_value(Key_T, Mapped_T);
		protected:		
			bool __print_trace2 = false;
			void _print_trace2(std::string fn, bool focus) const;
	};

/*
template<typename Key_T, typename Mapped_T>
template <SkipList<Key_T, Mapped_T>::Node *SkipList<Key_T, Mapped_T>::Node::*ptr> struct Traits {
    static const char *const order_type;
	};	

template<typename Key_T, typename Mapped_T>
template <> const char *const Traits<&SkipList<Key_T, Mapped_T>::Node::forward[0]>::order_type = "reverse";
template<typename Key_T, typename Mapped_T>
template <> const char *const Traits<&SkipList<Key_T, Mapped_T>::Node::backward[0]>::order_type = "forward";
*/
#include "skip_test_template.hpp"

#endif
