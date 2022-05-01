#ifndef CS540_MAP_H
#define CS540_MAP_H

#include <iostream>
#include <vector>
#include <string>
#include <assert.h>
#include <random>
#include <cmath>
//#include <iterator>

namespace cs540 {
	template<typename Key_T, typename Mapped_T>
	class Map {
				
	
	public:
		// forward decl		
		class Iterator;
		class ConstIterator;
		class ReverseIterator;
	private:
		class SkipList;
	public:	
		// type member
		typedef std::pair<const Key_T, Mapped_T> ValueType;
		// constructors and assignment
		Map();
		Map(const Map &);
		Map &operator=(const Map &);
		Map(std::initializer_list<std::pair<const Key_T, Mapped_T>>);
		~Map();
		// size
		size_t size() const;
		bool empty() const; 
		// iterators
		Iterator begin();
		Iterator end(); 
		ConstIterator begin() const;
		ConstIterator end() const;
		ReverseIterator rbegin();
		ReverseIterator rend();
		// element access
		Iterator find(const Key_T &); // XXX 11
		ConstIterator find(const Key_T &) const; // XXX 11
		Mapped_T &at(const Key_T &); // XXX 11
		const Mapped_T &at(const Key_T &) const; // XXX 11 --is returning const?
		Mapped_T &operator[](const Key_T &); // XXX 11
		// modifiers 
		std::pair<Iterator, bool> insert(const ValueType &); // XXX 11
		template <typename IT_T>
		void insert(IT_T range_beg, IT_T range_end); // XXX 11
		void erase(Iterator pos); // XXX 11
		void erase(const Key_T &); // XXX 11
		void clear();
		
		/*
		template<typename K, typename M>
		friend bool operator==(const typename Map<K, M>::Iterator &, 
								const typename Map<K, M>::Iterator &);
		template<typename K, typename M>		
		friend bool operator!=(const typename Map<K, M>::Iterator &, 
								const typename Map<K, M>::Iterator &);	
		*/
		
		friend bool operator==(const Iterator &first, 
								const Iterator &second){
			if(first.loc == second.loc) return true;
			return false;
		}
				
		friend bool operator!=(const Iterator &first, 
								const Iterator &second){
			return !(first == second);
		}

	//private: // XXX old
	public: // XXX new
		SkipList data;
		unsigned int length;
	public:
		void _see_insert(std::pair<Iterator, bool> &) const;
	private:		
		bool __print_trace = false;
		void _print_trace(std::string fn, bool focus) const;	
};


template<typename Key_T, typename Mapped_T>
class Map<Key_T, Mapped_T>::Iterator {		
	friend class Map<Key_T, Mapped_T>;	
	public:
		Iterator(const Iterator &);
		~Iterator();
		Iterator& operator=(const Iterator &); 
			
	protected:			
		Iterator(typename Map<Key_T, Mapped_T>::SkipList::Node* at); 
		Iterator() = delete;
	public:
		Iterator &operator++(); 
		Iterator operator++(int);
		Iterator &operator--(); 
		Iterator operator--(int);
		ValueType &operator*() const; 
		ValueType *operator->() const;
		template<typename K, typename M>
		friend bool operator==(const typename Map<K, M>::Iterator &, 
								const typename Map<K, M>::Iterator &);
		template<typename K, typename M>		
		friend bool operator!=(const typename Map<K, M>::Iterator &, 
								const typename Map<K, M>::Iterator &);
	// data members		
	//protected: // XXX old
	public:			
		typename Map<Key_T, Mapped_T>::SkipList::Node* loc;
	 	
	//debug
	private:		
		bool __print_trace = false;
		void _print_trace(std::string fn, bool focus) const;
};


template<typename Key_T, typename Mapped_T>
class Map<Key_T, Mapped_T>::ConstIterator : public Iterator {
	friend class Map<Key_T, Mapped_T>;		
	public:	
		ConstIterator(const ConstIterator &);
		ConstIterator(const Iterator &); 
		~ConstIterator();
		ConstIterator& operator=(const ConstIterator &);
		ConstIterator() = delete;
	protected:			
		ConstIterator(typename Map<Key_T, Mapped_T>::SkipList::Node* at); 
	public:
		ConstIterator &operator++(); 	
		ConstIterator operator++(int);
		ConstIterator &operator--();
		ConstIterator operator--(int);
		const ValueType &operator*() const;
		const ValueType *operator->() const;

	//debug
	private:		
		bool __print_trace = false;
		void _print_trace(std::string fn, bool focus) const;
};

template<typename Key_T, typename Mapped_T>
class Map<Key_T, Mapped_T>::ReverseIterator : public Iterator {
	friend class Map<Key_T, Mapped_T>;		
	public:
		ReverseIterator(const ReverseIterator &);
		~ReverseIterator();
		ReverseIterator& operator=(const ReverseIterator &);
	protected:			
		ReverseIterator(typename Map<Key_T, Mapped_T>::SkipList::Node* at); 
		ReverseIterator() = delete;
	public:
		
		ReverseIterator &operator++();
		ReverseIterator operator++(int);
		ReverseIterator &operator--();
		ReverseIterator operator--(int);
		ValueType &operator*() const; 
		ValueType *operator->() const;

	//debug
	private:		
		bool __print_trace = false;
		void _print_trace(std::string fn, bool focus) const;
};



// SkipList-------------------------------------------------------------------
template<typename Key_T, typename Mapped_T>
class Map<Key_T, Mapped_T>::SkipList {
	typedef std::pair<const Key_T, Mapped_T> ValueType; // XXX
	public:	
		class Node;	
		class DataNode;
				
		typedef std::pair<bool, Node*>				SearchResult;
		typedef std::pair<Node*, unsigned long> 	SearchHelpResult;
		typedef std::pair<bool, Node*>				InsertResult;
		
		SkipList();
		SkipList(const SkipList &);		
		~SkipList();
		void init_stats();
		void copy_list(const SkipList& s);
// member fns
		
		void 				clear_list();
		void				reset();
		SkipList&			operator=(const SkipList &);		
		InsertResult		insert(const ValueType &);
		SearchResult		find(const Key_T &);
		SearchResult		find(const Key_T &) const;		
		bool				erase(const Key_T &);
			
	public: //XXX make private...or protected?
		Node*				insert_before(const ValueType		&pair, 
										  std::vector<Node*>	&update);
		
		SearchHelpResult	find_help(const Key_T			&key, 
									  std::vector<Node*>	&update);
		
		SearchHelpResult	find_help(const Key_T &key);		
		SearchHelpResult	find_help(const Key_T &key) const;
		unsigned int		get_rand_height();
// debug + verification
		//private: // XXX make private
		void 				pretty_print_sentinel() const;
		void 				pretty_print_dnode(Node *) const;
		void 				pretty_print() const;

		void 				verify_forward();
		void 				verify_backward();
		void 				verify_both();
		void 				show_time_complexity();

		bool 				__print_trace = false;
		void 				_print_trace(std::string fn, bool focus) const;
		void				_check_sz_capacity(Node*) const;	
	
	public:
	// data members			
		Node* sentinel;
		unsigned long node_id;		
		unsigned int curr_max_ht;
   		int size; // XXX
		std::mt19937 flip;
		std::bernoulli_distribution coin;
		unsigned int capacity;
		
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
	};

// Node ----------------------------------------------------------------------
	template<typename Key_T, typename Mapped_T>
	class Map<Key_T, Mapped_T>::SkipList::Node { // XXX
	public:				
		Node(int aHeight, unsigned long aId);				
		Node(int aHeight, unsigned long aId, int capacity);		
		Node(const Node &);		
		~Node();
		Node& operator=(const Node &);
		//friend void swap(Node &, Node &);
	

		unsigned int height;		
		const unsigned long id;		
		Node* backward;
		std::vector<Node*> forward;

		void reset();
	protected:		
		bool __print_trace = false;
		void _print_trace(std::string fn, bool focus) const; // XXX virtual

		
	};

// DataNode-------------------------------------------------------------------
	template<typename Key_T, typename Mapped_T>
	class Map<Key_T, Mapped_T>::SkipList::DataNode : public Node { // XXX
	public:
		DataNode() = delete;
		DataNode(const ValueType &aPair, const int aHeight, 
									unsigned long id); // TODO pass in vect?
		DataNode(const DataNode &);
		~DataNode();
		DataNode& operator=(const DataNode &);
			
		ValueType pair;
	private:
		ValueType copy_value(Key_T, Mapped_T);
	protected:		
		bool __print_trace2 = false;
		void _print_trace2(std::string fn, bool focus) const;
	};


	//#define TEMPLATE_T template<typename Key_T, typename Mapped_T>
	#include "Map_Template.hpp"
	#include "Iterator_Template.hpp"
	#include "ConstIterator_Template.hpp"
	#include "ReverseIterator_Template.hpp"
	#include "SkipList_Template.hpp"
	#include "FreeFunctions_Template.hpp"
}


#endif
