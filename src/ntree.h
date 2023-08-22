#pragma once
#include <fstream>
#include <iostream>
#include "constants.h"
#include <cstring>
#include "ioopers.h"

class NodeType{
    NodeType* _mother;
    NodeType* _children;
    bool _leaf;
    int_type _node_id;
    int_type _num_children;
    public:
        NodeType(NodeType* mother ,NodeType* children , bool leaf, int_type & num_children,int_type & node_id);
        NodeType();
        void from_binary(BinarySegmentReader* nd);
        int_type binary_size();
        void to_buffer(BinarySegmentWriter* bb);
        bool is_leaf();
        bool is_first();
};

class NTreeIndex{    
    public:
        int_type _depth;
        int_type _child_ind;
        NTreeIndex(int_type depth,int_type child_ind): _depth(depth),_child_ind(child_ind){};
        void print();
};

class NTreeIndexer{
    int_type _current_depth;
    int_type* _cursor_arr;
    int_type _max_width;
    int_type _max_depth;
    NTreeIndex* _cur_coordinate;
    bool _empty;
    public:
        NTreeIndexer(int_type max_depth,int_type max_width);
        NTreeIndex* next();
        ~NTreeIndexer();
        void print();
};



class NTree{
    NodeType* _head;
    int_type _nbranch;
    int_type _depth;
    BinaryBuffer* _buff;
    public:
        NTree(int_type nbranch,BinaryBuffer* buff);
        int_type get_nbranch();
        int_type get_max_depth();
        NodeType* get_head();
};




class NTreeIterator{
    int_type _nbranch;
    int_type _cur_depth;
    NodeType* _current;
    NTreeIndexer* _counter;
    bool _head_counted;
    public:
        NTreeIterator(NTree* NTree);
        NodeType* next();
        ~NTreeIterator();
};