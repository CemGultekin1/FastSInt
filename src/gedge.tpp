#include  "gedge.h"
#include <stdexcept>
#include <string>



/*
GraphEdgeType
*/

template<class Feature>
GraphEdgeType<Feature>::GraphEdgeType(GraphEdgeType<Feature>** parents ,GraphEdgeType<Feature>* children , \
            int_type& num_parents,int_type& num_children, \
                int_type& edge_id,int_type & depth){
    _parents = parents;
    _children = children;
    _num_children = num_children;
    _num_parents = num_parents;
    _edge_id = edge_id;
    _depth = depth;
}
template<class Feature>
GraphEdgeType<Feature>::GraphEdgeType(){
    _parents = nullptr;
    _children = nullptr;
    _num_parents = 0;
    _num_children = 0;
    _edge_id = 0;
    _depth = 0;
}
template<class Feature>
int_type GraphEdgeType<Feature>::get_edge_id(){
    return _edge_id;
}
template<class Feature>
void GraphEdgeType<Feature>::delete_pointers(){
    

    if(!is_leaf()){
        if(_children != nullptr){
            for(int_type i = 0; i < _num_children; i ++){
                _children[i].delete_pointers();
            }
            _num_children = 0;
        }
    }
    if(_parents != nullptr){
        delete _parents;
    }
   
}
template<class Feature>
GraphEdgeType<Feature>::~GraphEdgeType(){
    delete_pointers();
}
template<class Feature>
int_type GraphEdgeType<Feature>:: get_num_children(){
    return _num_children;
}
template<class Feature>
void GraphEdgeType<Feature>::add_children(std::vector<GraphEdgeType<Feature>*> * edges,int_type first_child_edge_id){
    int_type edge_id = first_child_edge_id;
    _children =  new GraphEdgeType<Feature>[_num_children];
    GraphEdgeType<Feature>* child;
    for(child=_children;child - _children  < _num_children; child++, edge_id++){
        child->_parents = new GraphEdgeType<Feature>*[1];
        child->_parents[0] = this;
        child->_edge_id = edge_id;
        child->_depth = _depth + 1;
        child->_num_parents = 1;

        
        while(edges->size() <= edge_id){
            edges->push_back(nullptr);
        }        
        if( (*edges)[edge_id] != nullptr){
            throw std::range_error("The edge id " + std::to_string(edge_id) + " has already been initiated!");
        }
        (*edges)[edge_id] = child;
    }
}
template<class Feature>
void GraphEdgeType<Feature>::add_parent(GraphEdgeType<Feature>* parent){
    _num_parents += 1;
    GraphEdgeType<Feature>** parents = new GraphEdgeType<Feature>*[_num_children];
    for(int_type i= 0 ; i < _num_children - 1; i++){
        parents[i] = _parents[i];
    }
    parents[_num_children - 1] = parent;
    delete _parents;
    _parents = parents;
    int_type depth_candidate = parent->_depth + 1;
    if(depth_candidate > _depth){
        _depth = depth_candidate;
    }
}
template<class Feature>
void GraphEdgeType<Feature>::set_num_children(int_type num_children){
    _num_children = num_children;
}
template<class Feature>
void GraphEdgeType<Feature>::from_binary(BinarySegmentReader* nd,\
                std::vector<GraphEdgeType<Feature>*> * edges,\
                std::vector<Feature*>* features){
    int_type* cursor;
    cursor = (int_type*) nd->next(sizeof(int_type));
    _num_children = *cursor;

    if(_num_children>0){
        cursor = (int_type*) nd->next(sizeof(int_type));
        if(cursor == nullptr){
            throw std::range_error("No number of children is written for this!");
        }
        add_children(edges,*cursor);
    }    
    add_feature(features);
}
template<class Feature>
void GraphEdgeType<Feature>::to_binary(BinarySegmentWriter* bb) const{
    bb->write((char*) &_num_children, sizeof(_num_children));    
    if(_num_children > 0){
        bb->write((char*) &_children[0]._edge_id, sizeof(_edge_id));
    }
    
    
}
template<class Feature>
std::string GraphEdgeType<Feature>::to_string() const{
    std::string printstring =  "edgeid = " + std::to_string(_edge_id) + ", number of children = " + std::to_string(_num_children)  + ": (";
    if(_num_children > 0){
        for(GraphEdgeType<Feature>* x = _children; x - _children < _num_children; x ++){
            printstring +=  std::to_string(x->_edge_id) + ", ";
        }
    }
    printstring += ")";
    return printstring;
}
template<class Feature>
void GraphEdgeType<Feature>::print() const{
    std::cout << to_string() << std::endl;
}
template<class Feature>
bool GraphEdgeType<Feature>::is_leaf(){
    return _num_children == 0;
}
template<class Feature>
bool GraphEdgeType<Feature>::is_first(){
    return _parents == nullptr;
}

template<class Feature>
GraphEdgeType<Feature>** GraphEdgeType<Feature>::get_parents() const{
    return _parents;
}
template<class Feature>
GraphEdgeType<Feature>* GraphEdgeType<Feature>::get_children(){
    return _children;
}

template<class Feature>
bool GraphEdgeType<Feature>::add_feature(std::vector<Feature*>* features){
    if(features->size() > _edge_id){
        _features = (*features)[_edge_id];
        return true;
    }
    return false;
}


template<class Feature>
int_type GraphEdgeType<Feature>::get_exit_index(int_type parent_id) const{
    if(is_first()){
        return NLLC;
    }
    GraphEdgeType<Feature>* sibling = _parents[parent_id]->_children;
    int_type diff = (this - sibling)/sizeof(GraphEdgeType<Feature>);
    return diff;
}