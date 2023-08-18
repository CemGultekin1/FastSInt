#pragma once
#include <fstream>
#include <iostream>
#include "constants.h"
#include <cstring>
class NodeData{
    char* _binary;
    int_type _sizeof;
    public:
        NodeData(char* binary,int_type len, int_type typesize);
        NodeData();
        void write(std::ofstream& file);
        void read(std::ifstream& file);
        ~NodeData();
};

class NodeType{
    NodeType* _mother;
    NodeType* _children;
    bool* _leaf;
    int_type _num_children;
    NodeData* _node_data;
    public:
        NodeType(NodeType* mother ,NodeType* children , bool* leaf, int_type & num_children);
        void assimilate_data(NodeData* nd);
        void write(std::ofstream& file,int_type& num_div);
        void read(std::ifstream& file,int_type& num_div);
        bool is_leaf();
        bool is_first();
};

class TreeCoordinate{    
    public:
        int_type _depth;
        int_type _child_ind;
        TreeCoordinate(int_type depth,int_type child_ind): _depth(depth),_child_ind(child_ind){};
        void print();
};

class TreeCounter{
    int_type _current_depth;
    int_type* _cursor_arr;
    int_type _max_width;
    int_type _max_depth;
    TreeCoordinate* _cur_coordinate;
    bool _empty;
    public:
        TreeCounter(int_type max_depth,int_type max_width);
        TreeCoordinate* next();
        ~TreeCounter();
        void print();
};



class IOTree{
    NodeType* _head;
    int_type _nbranch;
    int_type _depth;
    public:
        IOTree(int_type nbranch);
        void to_file(std::string filename);
        void from_file(std::string filename);
        int_type get_nbranch();
        int_type get_max_depth();
        NodeType* get_head();
};




class TreeWalker{
    int_type _nbranch;
    int_type _cur_depth;
    NodeType* _current;
    TreeCounter* _counter;
    bool _head_counted;
    public:
        TreeWalker(IOTree* iotree);
        NodeType* next();
        ~TreeWalker();
};