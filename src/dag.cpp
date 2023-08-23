#include  "dag.h"
#include <stdexcept>
#include <string>



/*
GraphEdgeType
*/


GraphEdgeType::GraphEdgeType(GraphEdgeType* parent ,GraphEdgeType* children , bool leaf,int_type& num_children,int_type& edge_id){
    _parent = parent;
    _children = children;
    _leaf = leaf;
    _num_children = num_children;
    _edge_id = edge_id;
}
GraphEdgeType::GraphEdgeType(){
    _parent = nullptr;
    _children = nullptr;
    _leaf = true;
    _num_children = 0;
    _edge_id = 0;
}
int_type GraphEdgeType::get_edge_id(){
    return _edge_id;
}
void GraphEdgeType::delete_pointers(){
    if(_leaf){
        return;
    }
    if(_children == nullptr){
        return;
    }
    for(int_type i = 0; i < _num_children; i ++){
        _children[i].delete_pointers();
    }
    delete _children;
}
GraphEdgeType::~GraphEdgeType(){
    delete_pointers();
}

int_type GraphEdgeType:: get_num_children(){
    return _num_children;
}

void GraphEdgeType::add_children(std::vector<GraphEdgeType*> * edges,int_type first_child_edge_id){
    int_type edge_id = first_child_edge_id;
    
    _children =  new GraphEdgeType[_num_children];
    GraphEdgeType* child;
    for(child=_children;child - _children  < _num_children; child++, edge_id++){
        child->_parent = this;
        child->_edge_id = edge_id;
        
        while(edges->size() <= edge_id){
            edges->push_back(nullptr);
        }        
        if( (*edges)[edge_id] != nullptr){
            throw std::range_error("The edge id " + std::to_string(edge_id) + " has already been initiated!");
        }
        (*edges)[edge_id] = child;
    }
}

void GraphEdgeType::set_num_children(int_type num_children){
    _num_children = num_children;
}

void GraphEdgeType::from_binary(BinarySegmentReader* nd,std::vector<GraphEdgeType*> * edges){
    int_type* cursor;
    cursor = (int_type*) nd->next(sizeof(int_type));
    if(cursor == nullptr){
        throw std::range_error("No number of children is written for this!");
    }
    _num_children = *cursor;

    if(_num_children>0){
        cursor = (int_type*) nd->next(sizeof(int_type));
        if(cursor == nullptr){
            throw std::range_error("No number of children is written for this!");
        }
        add_children(edges,*cursor);
    }    
}

void GraphEdgeType::to_binary(BinarySegmentWriter* bb) const{
    // bb->write((char*) &_edge_id, sizeof(_edge_id));
    bb->write((char*) &_num_children, sizeof(_num_children));    
    if(_num_children > 0){
        bb->write((char*) &_children[0]._edge_id, sizeof(_edge_id));
    }
    
}

std::string GraphEdgeType::to_string() const{
    std::string printstring =  "edgeid = " + std::to_string(_edge_id) + ", number of children = " + std::to_string(_num_children)  + ": (";
    if(_num_children > 0){
        for(GraphEdgeType* x = _children; x - _children < _num_children; x ++){
            printstring +=  std::to_string(x->_edge_id) + ", ";
        }
    }
    printstring += ")";
    return printstring;
}

void GraphEdgeType::print() const{
    std::cout << to_string() << std::endl;
}

bool GraphEdgeType::is_leaf(){
    return _leaf;
}

bool GraphEdgeType::is_first(){
    return _parent == nullptr;
}


GraphEdgeType* GraphEdgeType::get_parent(){
    return _parent;
}
GraphEdgeType* GraphEdgeType::get_children(){
    return _children;
}




int_type GraphEdgeType::get_depth(){
    int_type depth = 0;
    GraphEdgeType* parent = _parent;
    while(parent != nullptr){
        depth+=1;
        parent = parent->_parent;
    }
    return depth;
}



DAG::DAG(int_type width){//BinaryBuffer* buff){
    _width = width;
    _depth = 0;
    _edges  = std::vector<GraphEdgeType*>();
    GraphEdgeType* head = new GraphEdgeType();
    _edges.push_back(head);
}

void DAG::to_file(std::string _filename){
    BinaryBuffer* buff = new BinaryBuffer();
    BinarySegmentWriter* bsw;
    // writing the headers
    bsw = buff->new_segment();
    bsw->write((char*) &_width, sizeof(_width));
    bsw->write((char*) &_depth, sizeof(_depth));
    delete bsw;
    
    // writing the edges
    for(auto edgeptr: _edges){
        bsw = buff->new_segment();
        edgeptr->to_binary(bsw);
        delete bsw;
    }
    buff->to_file(_filename);
    delete buff;
}

void DAG::from_file(std::string _filename){
    BinaryBuffer* buff = new BinaryBuffer();
    buff->from_file(_filename);  
    int_type nsegs = buff->num_segments() - 1;
    BinarySegmentReader* bsr;    
    _edges.resize(nsegs,nullptr);
    GraphEdgeType * nt = _edges[0];
    
    
    bsr = buff->read_segment(0);
    _width = *((int_type*) bsr->next(sizeof(_width)));
    _depth = *((int_type*) bsr->next(sizeof(_depth)));
    delete bsr;

    bsr = buff->read_segment(1);
    nt->from_binary(bsr,&_edges);
    delete bsr;


    std::vector<bool> tobechecked(_edges.size(),false);
    int_type num_children = nt->get_num_children() + 1;
    if(num_children == 1){
        return;
    }
    for(int_type segi = 1; segi < num_children + 1; segi ++){
        tobechecked[segi] = true;
    }
    int_type i = 1;
    int_type i1;
    int_type i2;
    while(i < tobechecked.size()){
        bsr = buff->read_segment(i + 1);     
        nt = _edges[i]; //new GraphEdgeType();
        nt->from_binary(bsr,&_edges);
        delete bsr;

        tobechecked[i] = false;
        num_children = nt->get_num_children();
        if(num_children > 0 ){
            i1 = nt->get_children()->get_edge_id();
            for(i2 = i1; i2 - i1 < num_children; i2 ++){                
                tobechecked[i2] = true;
            }
        }
        i++;
        while(i < tobechecked.size()){            
            if(tobechecked[i]){
                break;
            }
            i++;
        }

    }
    delete buff;
}


int_type DAG::get_width(){
    return _width;
}

int_type DAG::get_max_depth(){
    return _depth;
}

GraphEdgeType* DAG::get_head(){
    return _edges[0];
}

DAG::~DAG(){
    _edges.clear();
}


GraphEdgeType* DAG::branch_from_edge(GraphEdgeType* x,int_type num_children, int_type first_child_edge_id ){
    if(! (x->is_leaf())){
        throw std::invalid_argument("The edge " + std::to_string(x->get_edge_id()) + " is not a leaf!");
    }
    if(num_children == 0){
        return nullptr;
    }
    x->set_num_children(num_children);
    if(first_child_edge_id == NLLC){    
        first_child_edge_id = _edges.size();
    }
    
    x->add_children(&_edges,first_child_edge_id);
    int_type new_depth = x->get_depth() + 1;
    if(new_depth > _depth){
        _depth = new_depth;
    }
    return x->get_children();
}


void DAG::print(){
    int_type parent_id = -1;
    int_type edge_id;
    int_type width = 6;
    std::cout << "max number of branches = " << _width <<std::endl;
    std::cout << "depth = " << _depth <<std::endl;
    for(auto edgeptr: _edges){
        std::cout << edgeptr->to_string() << std::endl;
        // if(!(edgeptr->is_first())){
        //     parent_id = edgeptr->get_parent()->get_edge_id();
        // }
        // edge_id = edgeptr->get_edge_id();
        // std::cout <<std::setw(width/2)<< edge_id << std::setw(width) 
        //         << "<<" <<std::setw(width) << parent_id<<std::endl;        
    }
}   