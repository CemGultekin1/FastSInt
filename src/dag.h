#pragma once
#include <fstream>
#include <iostream>
#include "constants.h"
#include <cstring>
#include "ioopers.h"
#include "gedge.h"

template <class Feature>
class DAG{    
    protected:
        int_type _width;        
        typedef GraphEdgeType<Feature> Edge;
        typedef std::vector<Edge*> EdgeVec;
        typedef std::vector<Feature*> FeatureVec;
    public:
    int_type _depth;
        EdgeVec _edges;
        FeatureVec _edge_features;
        DAG(int_type width = NLLC);
        int_type get_width();
        int_type get_max_depth();
        Edge* get_head();
        // void edge_to_file(BinarySegmentWriter*bsw,Edge* edge) const;
        // void edge_from_file(BinarySegmentReader*bsw,Edge* edge);
        void to_file(std::string _filename);
        void from_file(std::string _filename);
        ~DAG();
        Edge* branch_from_edge(Edge* ,int_type ,int_type  = NLLC);
        void print(bool keep_short = false);
        void add_feature(Feature*feat,Edge*edge);
};

#include "dag.tpp"

