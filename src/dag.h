#pragma once
#include <fstream>
#include <iostream>
#include "constants.h"
#include <cstring>
#include "ioopers.h"


class GraphEdgeType{
    GraphEdgeType* _parent;
    GraphEdgeType* _children;
    bool _leaf;
    int_type _edge_id;
    int_type _num_children;
    public:
        GraphEdgeType(GraphEdgeType* parent ,GraphEdgeType* children , bool leaf, int_type & num_children,int_type & edge_id);
        GraphEdgeType();
        void from_binary(BinarySegmentReader* nd,std::vector<GraphEdgeType*> * edges);
        void to_binary(BinarySegmentWriter* bb) const;
        void add_children(std::vector<GraphEdgeType*> * edges,int_type first_child_edge_id);
        void set_num_children(int_type num_children);
        bool is_leaf();
        bool is_first();
        std::string to_string() const;
        void print() const;
        void delete_pointers();
        int_type get_num_children();
        int_type get_edge_id();
        GraphEdgeType* get_parent();
        GraphEdgeType* get_children();
        ~GraphEdgeType();
        int_type get_depth();
};



class DAG{    
    int_type _width;
    int_type _depth;
    public:
        std::vector<GraphEdgeType*> _edges;
        DAG(int_type width = NLLC);
        int_type get_width();
        int_type get_max_depth();
        GraphEdgeType* get_head();
        void to_file(std::string _filename);
        void from_file(std::string _filename);
        ~DAG();
        GraphEdgeType* branch_from_edge(GraphEdgeType* ,int_type ,int_type  = NLLC);
        void print();
};



