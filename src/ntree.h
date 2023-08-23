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
        void from_binary(BinarySegmentReader* nd,std::vector<NodeType*> * nodes);
        void to_binary(BinarySegmentWriter* bb) const;
        void add_children(std::vector<NodeType*> * nodes,int_type first_child_node_id);
        void set_num_children(int_type num_children);
        bool is_leaf();
        bool is_first();
        std::string to_string() const;
        void print() const;
        void delete_pointers();
        int_type get_num_children();
        int_type get_node_id();
        NodeType* get_mother();
        NodeType* get_children();
        ~NodeType();
        int_type get_depth();
};



class NTree{    
    int_type _nbranch;
    int_type _depth;
    public:
        std::vector<NodeType*> _nodes;
        NTree(int_type nbranch = NLLC);
        int_type get_nbranch();
        int_type get_max_depth();
        NodeType* get_head();
        void to_file(std::string _filename);
        void from_file(std::string _filename);
        ~NTree();
        NodeType* branch_from_node(NodeType* ,int_type ,int_type  = NLLC);
        void print();
};



