#include "simplex.h"
#include "midptransform.h"




void SimplexDAG::to_file(std::string _dagfile,std::string _midpointfile){
    DAG<Midpoint>::to_file(_dagfile);
    
    BinaryBuffer* buff = new BinaryBuffer();
    BinarySegmentWriter* bsw;
    int_type midp_id;
    _edge_features.clear();
    // _edge_features.resize(_edges.size(),nullptr);
    for(auto edgeptr: _edges){
        bsw = buff->new_segment();  
        midp_id = edgeptr->_features->_midpoint_id;
        bsw->write((char*) &midp_id,sizeof(midp_id));
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
    _dim = _width;
    BinaryBuffer* buff = new BinaryBuffer();
    buff->from_file(_midpointfile);  
    BinarySegmentReader* bsr;
    int_type nsegs = buff->num_segments();
    int_type* midp_id;
    int_type nedges = _edges.size();
    std::vector<int_type> midpids(nedges,0);
    for(int_type edge_id  = 0; edge_id < nedges; edge_id ++ ){
        bsr = buff->read_segment(edge_id);
        midp_id = (int_type*) bsr->next(sizeof(int_type));
        midpids[edge_id] = *midp_id;
    }
    _midpoints = SimplexDAG::MidpointVec(nsegs,nullptr);
    Midpoint* midptr;
    for(int_type i = nedges; i < nsegs; i ++){        
        bsr = buff->read_segment(i);
        midptr = new Midpoint();
        midptr->from_binary(bsr);
        _midpoints[i]  = midptr;
        
    }
    delete buff;
    for(int_type edge_id  = 0; edge_id < nedges; edge_id ++ ){
        _edges[edge_id]->add_feature(&_midpoints);
        
    }
    midpids.clear();
}   
SimplexDAG::~SimplexDAG(){
    DAG<Midpoint>::~DAG();
    _midpoints.clear();
}

Path::Path(int_type depth){
    _depth = depth;
    _path = new Simplex*[_depth];
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
    Simplex* newsmptr = &(smptr->_children[exit_index]);
    midt.run_transform();
    _path->add(newsmptr);
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

// Walker* SimplexDAG::find_simplex(IncondenseMidpoint* imidp){
//     // Midpoint* midptr;

//     Walker* walker = new Walker(this,imidp);
//     while(walker->move_down()){
//         continue;
//     }
//     return walker;
// }
// Simplex* SimplexDAG::add_midpoint(Midpoint*midp,int_type nsimplex){

// }