#include "simplex.h"
#include "midptransform.h"
#include "stdexcept"



void SimplexDAG::to_file(std::string _dagfile,std::string _midpointfile){
    DAG<Midpoint>::to_file(_dagfile);
    
    BinaryBuffer* buff = new BinaryBuffer();
    BinarySegmentWriter* bsw;
    int_type midp_id;
    // _edge_features.clear();
    // _edge_features.resize(_edges.size(),nullptr);
    for(auto edgeptr: _edges){
        bsw = buff->new_segment();  
        if(edgeptr->_features != nullptr){
            midp_id = edgeptr->_features->_midpoint_id;
            bsw->write((char*) &midp_id,sizeof(midp_id));
        }        
        delete bsw;
    }
    // writing the midpoints
    for(auto midptr: _midpoints){
        bsw = buff->new_segment();  
        midptr->to_binary(bsw);
        delete bsw;
    }
    buff->to_file(_midpointfile);
    delete buff;

}
void SimplexDAG::from_file(std::string _dagfile,std::string _midpointfile){
    DAG<Midpoint>::from_file(_dagfile);

    BinaryBuffer* buff = new BinaryBuffer();
    buff->from_file(_midpointfile);  
    BinarySegmentReader* bsr;
    int_type nsegs = buff->num_segments();
    int_type* midp_id;
    int_type nedges = _edges.size();

    // std::cout << "nsegs, negdes = " << nsegs << " "<< nedges << std::endl;

    std::vector<int_type> midpids(nedges,NLLC);
    for(int_type edge_id  = 0; edge_id < nedges; edge_id ++ ){
        bsr = buff->read_segment(edge_id);
        if(bsr->get_size() > 0){
            midp_id = (int_type*) bsr->next(sizeof(int_type));
            midpids[edge_id] = *midp_id;
        }        
    }

    std::cout << "edge_id midpoint_id mapping is read!" << std::endl;

    _midpoints.resize(nsegs - nedges,nullptr);
    Midpoint* midptr;
    for(int_type i = nedges; i < nsegs; i ++){      
        std::cout << "bsr = buff->read_segment("   << i << ");" << std::endl;
        bsr = buff->read_segment(i);
        midptr = new Midpoint();
        std::cout << "midptr->from_binary(bsr);" << std::endl;
        midptr->from_binary(bsr);
        std::cout << "_midpoints[i - nedges]  = midptr;" << std::endl;
        _midpoints[i - nedges]  = midptr;
        
    }
    delete buff;
    std::cout << "midpoints are read!" << std::endl;
    _edge_features.resize(_edges.size(),nullptr);
    int_type midpointloc;
    for(int_type edge_id  = 0; edge_id < nedges; edge_id ++ ){
        midpointloc = midpids[edge_id];
        if(midpointloc != NLLC){
            _edge_features[edge_id] = _midpoints[midpointloc];
        }
    }
    for(int_type edge_id  = 0; edge_id < nedges; edge_id ++ ){
        _edges[edge_id]->add_feature(&_edge_features);
    }
    midpids.clear();
}   

int_type  SimplexDAG::add_midpoint(Midpoint* midp){
    midp->_midpoint_id = _midpoints.size();
    _midpoints.push_back(midp);
    return midp->_midpoint_id;
}
// int_type SimplexDAG::add_midpoint_connection(int_type edge_id, int_type midpoint_id){
    // _edge_features
// }

void SimplexDAG::midpoint_split_on_simplex(Simplex* sm){
    if(!sm->is_leaf()){
        throw std::runtime_error("This simplex not a leaf, can't add midpoint to it");
    }
    branch_from_edge(sm,sm->_features->get_length());
}
void SimplexDAG::add_node_to_dag(DataPoint* dp){
    IncondenseMidpoint imidp(dp->_weights,dp->_dim, _depth,dp->_data_id,dp->_node_id,_midpoints.size());
    Walker walker(this,&imidp);
    walker.move_down_all_the_way();
    Midpoint* midp = imidp.condensate();
    int_type midpoint_id = add_midpoint(midp);
    Simplex* sm = walker._path->current();
    add_feature(midp,sm);
    midpoint_split_on_simplex(sm);
}




SimplexDAG::~SimplexDAG(){
    _midpoints.clear();
}

Path::Path(int_type depth){
    _depth = depth;
    _path = new Simplex*[_depth + 1];
    _crs = 0;
    _lst = 0;
}

void Path::add(Simplex* sm){
    _path[_lst++] = sm;
}

bool Path::down(){
    if( _crs < _depth){
        _crs++;
        return true;
    }
    return false;    
}

bool Path::up(){
    if( _crs > 0){
        _crs--;
        return true;
    }
    return false;    
}

Simplex* Path::current(){
    return _path[_crs];
}

Path::~Path(){
    delete _path;
}

Walker::Walker(SimplexDAG* sd,IncondenseMidpoint*imipdtr){    
    _path = new Path(sd->get_max_depth());
    _path->add(sd->_edges[0]);
    _imidptr = imipdtr;
}

bool Walker::move_down(){
    
    Simplex* smptr = _path->current();
    Midpoint* midptr = smptr->_features;
    if(midptr == nullptr){
        return false;
    }
    MidpointTransform midt (midptr,_imidptr);
    int_type exit_index = midt.exit_index();
    if(smptr->get_num_children() <= exit_index){
        throw std::runtime_error("No children but a midpoint?");
    }
    Simplex* newsmptr = &(smptr->_children[exit_index]);
    midt.run_transform();
    _path->add(newsmptr);
    _path->down();
    return true;
}

void Walker::move_down_all_the_way(){
    while(move_down()){
        continue;
    }
}
Walker::~Walker(){
    delete _path;
}
