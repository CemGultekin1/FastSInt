#pragma once
#include "dag.h"
#include "midpoint.h"
#include "databases.h"

typedef GraphEdgeType<Midpoint> Simplex;


class Path{
    public:
        Simplex** _path;
        int_type _crs;
        int_type _depth;
        int_type _lst;
        Path(int_type depth);//:_depth(depth),_path(new Simplex*[_depth]){}
        void add(Simplex*);
        bool down();
        bool up();
        Simplex* current();
        ~Path();
        
};
class SimplexDAG;

class Walker{
    public:
        IncondenseMidpoint* _imidptr;
        Path* _path;
        Walker(SimplexDAG*,IncondenseMidpoint*);
        bool move_down();
        ~Walker();
        void move_down_all_the_way();
        // bool move_up();
};
class SimplexDAG:public DAG<Midpoint>{    
    typedef std::vector<Midpoint*> MidpointVec;
    public:
        MidpointVec _midpoints;
        SimplexDAG(int_type dim):DAG<Midpoint>(dim+1){ _midpoints = MidpointVec();}
        SimplexDAG():DAG<Midpoint>(){ 
            _midpoints = MidpointVec();
        }
        void to_file(std::string _dagfile,std::string _midpointfile);
        void from_file(std::string _dagfile,std::string _midpointfile);
        int_type add_midpoint(Midpoint*);
        void midpoint_split_on_simplex(Simplex* sm);
        void add_node_to_dag(DataPoint* dp);
        ~SimplexDAG();
};