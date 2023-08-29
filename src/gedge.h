#pragma once
#include <fstream>
#include <iostream>
#include "constants.h"
#include <cstring>
#include "ioopers.h"
template<class Feature>
class GraphEdgeType{// in case of multiple parents, finding depth would require
    protected:             
        
        int_type _num_children;
        int_type _num_parents;
        
    public:
        int_type _edge_id;
        int_type _depth;
        GraphEdgeType<Feature>** _parents;
        GraphEdgeType<Feature>* _children;   
        Feature* _features;
        GraphEdgeType();        
        void to_binary(BinarySegmentWriter* bb) const;
        void set_num_children(int_type num_children);
        bool is_leaf();
        bool is_first();
        std::string to_string() const;
        void print() const;
        void delete_pointers();
        int_type get_num_children();
        int_type get_edge_id();
        ~GraphEdgeType();
        GraphEdgeType(GraphEdgeType<Feature>** parent ,GraphEdgeType<Feature>* children ,\
                     int_type& num_parents, int_type & num_children, \
                     int_type & edge_id, int_type & depth);
        void from_binary(BinarySegmentReader* nd,std::vector<GraphEdgeType<Feature>*> * edges,std::vector<Feature*>* features);
        void add_children(std::vector<GraphEdgeType<Feature>*> * edges,int_type first_child_edge_id);
        GraphEdgeType<Feature>** get_parents() const;
        GraphEdgeType<Feature>* get_children();
        void add_parent(GraphEdgeType<Feature>* parent);
        bool add_feature(std::vector<Feature*>* features);
        int_type get_exit_index(int_type = 0) const;
};

#include "gedge.tpp"