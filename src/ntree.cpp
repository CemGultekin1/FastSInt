#include  "ntree.h"
#include <stdexcept>
#include <string>



/*
NodeType
*/


NodeType::NodeType(NodeType* mother ,NodeType* children , bool leaf,int_type& num_children,int_type& node_id){
    _mother = mother;
    _children = children;
    _leaf = leaf;
    _num_children = num_children;
    _node_id = node_id;
}
NodeType::NodeType(){
    _mother = nullptr;
    _children = nullptr;
    _leaf = true;
    _num_children = 0;
    _node_id = 0;
}
int_type NodeType::get_node_id(){
    return _node_id;
}
void NodeType::delete_pointers(){
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
NodeType::~NodeType(){
    delete_pointers();
}

int_type NodeType:: get_num_children(){
    return _num_children;
}

void NodeType::add_children(std::vector<NodeType*> * nodes,int_type first_child_node_id){
    int_type node_id = first_child_node_id;
    
    _children =  new NodeType[_num_children];
    NodeType* child;
    for(child=_children;child - _children  < _num_children; child++, node_id++){
        child->_mother = this;
        child->_node_id = node_id;
        
        while(nodes->size() <= node_id){
            nodes->push_back(nullptr);
        }        
        if( (*nodes)[node_id] != nullptr){
            throw std::range_error("The node id " + std::to_string(node_id) + " has already been initiated!");
        }
        (*nodes)[node_id] = child;
    }
}

void NodeType::set_num_children(int_type num_children){
    _num_children = num_children;
}

void NodeType::from_binary(BinarySegmentReader* nd,std::vector<NodeType*> * nodes){
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
        add_children(nodes,*cursor);
    }    
}

void NodeType::to_binary(BinarySegmentWriter* bb) const{
    // bb->write((char*) &_node_id, sizeof(_node_id));
    bb->write((char*) &_num_children, sizeof(_num_children));    
    if(_num_children > 0){
        bb->write((char*) &_children[0]._node_id, sizeof(_node_id));
    }
    
}

std::string NodeType::to_string() const{
    std::string printstring =  "nodeid = " + std::to_string(_node_id) + ", number of children = " + std::to_string(_num_children)  + ": (";
    if(_num_children > 0){
        for(NodeType* x = _children; x - _children < _num_children; x ++){
            printstring +=  std::to_string(x->_node_id) + ", ";
        }
    }
    printstring += ")";
    return printstring;
}

void NodeType::print() const{
    std::cout << to_string() << std::endl;
}

bool NodeType::is_leaf(){
    return _leaf;
}

bool NodeType::is_first(){
    return _mother == nullptr;
}


NodeType* NodeType::get_mother(){
    return _mother;
}
NodeType* NodeType::get_children(){
    return _children;
}




int_type NodeType::get_depth(){
    int_type depth = 0;
    NodeType* mother = _mother;
    while(mother != nullptr){
        depth+=1;
        mother = mother->_mother;
    }
    return depth;
}



NTree::NTree(int_type nbranch){//BinaryBuffer* buff){
    _nbranch = nbranch;
    _depth = 0;
    _nodes  = std::vector<NodeType*>();
    NodeType* head = new NodeType();
    _nodes.push_back(head);
}

void NTree::to_file(std::string _filename){
    BinaryBuffer* buff = new BinaryBuffer();
    BinarySegmentWriter* bsw;
    // writing the headers
    bsw = buff->new_segment();
    bsw->write((char*) &_nbranch, sizeof(_nbranch));
    bsw->write((char*) &_depth, sizeof(_depth));
    delete bsw;
    
    // writing the nodes
    for(auto nodeptr: _nodes){
        bsw = buff->new_segment();
        nodeptr->to_binary(bsw);
        delete bsw;
    }
    buff->to_file(_filename);
    delete buff;
}

void NTree::from_file(std::string _filename){
    BinaryBuffer* buff = new BinaryBuffer();
    buff->from_file(_filename);  
    int_type nsegs = buff->num_segments() - 1;
    BinarySegmentReader* bsr;    
    _nodes.resize(nsegs,nullptr);
    NodeType * nt = _nodes[0];
    
    
    bsr = buff->read_segment(0);
    _nbranch = *((int_type*) bsr->next(sizeof(_nbranch)));
    _depth = *((int_type*) bsr->next(sizeof(_depth)));
    delete bsr;

    bsr = buff->read_segment(1);
    nt->from_binary(bsr,&_nodes);
    delete bsr;


    std::vector<bool> tobechecked(_nodes.size(),false);
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
        nt = _nodes[i]; //new NodeType();
        nt->from_binary(bsr,&_nodes);
        delete bsr;

        tobechecked[i] = false;
        num_children = nt->get_num_children();
        if(num_children > 0 ){
            i1 = nt->get_children()->get_node_id();
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


int_type NTree::get_nbranch(){
    return _nbranch;
}

int_type NTree::get_max_depth(){
    return _depth;
}

NodeType* NTree::get_head(){
    return _nodes[0];
}

NTree::~NTree(){
    _nodes.clear();
}


NodeType* NTree::branch_from_node(NodeType* x,int_type num_children, int_type first_child_node_id ){
    if(! (x->is_leaf())){
        throw std::invalid_argument("The node " + std::to_string(x->get_node_id()) + " is not a leaf!");
    }
    if(num_children == 0){
        return nullptr;
    }
    x->set_num_children(num_children);
    if(first_child_node_id == NLLC){    
        first_child_node_id = _nodes.size();
    }
    
    x->add_children(&_nodes,first_child_node_id);
    int_type new_depth = x->get_depth() + 1;
    if(new_depth > _depth){
        _depth = new_depth;
    }
    return x->get_children();
}


void NTree::print(){
    int_type mother_id = -1;
    int_type node_id;
    int_type width = 6;
    std::cout << "max number of branches = " << _nbranch <<std::endl;
    std::cout << "depth = " << _depth <<std::endl;
    for(auto nodeptr: _nodes){
        std::cout << nodeptr->to_string() << std::endl;
        // if(!(nodeptr->is_first())){
        //     mother_id = nodeptr->get_mother()->get_node_id();
        // }
        // node_id = nodeptr->get_node_id();
        // std::cout <<std::setw(width/2)<< node_id << std::setw(width) 
        //         << "<<" <<std::setw(width) << mother_id<<std::endl;        
    }
}   