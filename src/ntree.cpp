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
    int_type zero_children = 0;
    int_type node_id = 0;
    NodeType* child;
    _children = (NodeType*) malloc(sizeof(NodeType)*_num_children);
    for(int_type i=0; i < _num_children; i++){
        node_id =  first_child_node_id + i;
        child = new NodeType(this,(NodeType*) nullptr,true,zero_children,node_id);
        while(nodes->size() <= node_id){
            nodes->push_back(nullptr);
        }
        if( (*nodes)[node_id] != nullptr){
            throw std::range_error("The node id " + std::to_string(node_id) + " has already been initiated!");
        }
        (*nodes)[node_id] = child;
        _children[i] = *child;
    }
}

void NodeType::set_num_children(int_type num_children){
    _num_children = num_children;
}

void NodeType::from_binary(BinarySegmentReader* nd,std::vector<NodeType*> * nodes){
    const char* const binary = nd->get_binary();
    int_type* num_child = (int_type*) binary;
    _num_children = num_child[0];
    if(_num_children>0){
        int_type first_child_node_id = num_child[1];
        add_children(nodes,first_child_node_id);
    }    
}
void NodeType::to_binary(BinarySegmentWriter* bb) const{
    // bb->write((char*) &_node_id, sizeof(_node_id));
    bb->write((char*) &_num_children, sizeof(_num_children));    
    if(_num_children > 0){
        bb->write((char*) &_children[0]._node_id, sizeof(_node_id));
    }
    
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

/*
NTreeIndex
*/
void NTreeIndex::print(){
    std::cout << _depth << " " << _child_ind << std::endl;
}


/*
NTreeIndexer
*/

NTreeIndexer::NTreeIndexer(int_type max_depth,int_type max_width){
    /*
    _current_depth
    _cursor_arr
    _max_width
    _max_depth
    */
    _current_depth = 0;
    _max_depth = max_depth;
    _max_width = max_width;
    _cursor_arr = nullptr;
    _cur_coordinate = nullptr;
    _empty = max_depth == 0;
    if(!_empty){
        _cursor_arr = (int_type*) malloc(max_depth*sizeof(int_type));
        _cursor_arr[0] = 0;
        _cur_coordinate = new NTreeIndex(0,0);
    }
}

NTreeIndex* NTreeIndexer::next(){
    if(_empty){
        return nullptr;
    }
    
    int_type curw = _cursor_arr[_current_depth];

    _cur_coordinate->_depth = _current_depth;
    _cur_coordinate->_child_ind = curw;

    if(_current_depth < _max_depth - 1){
        _cursor_arr[_current_depth]+=1;
        _current_depth += 1;
        _cursor_arr[_current_depth] = 0;
        return _cur_coordinate;
    }else if(curw < _max_width){
        _cursor_arr[_current_depth]+=1;
        return _cur_coordinate;
    }
    while(curw == _max_width){
        _cursor_arr[_current_depth] = 0;
        _current_depth -= 1;
        if(_current_depth >= 0){
            curw = _cursor_arr[_current_depth];
        }else{
            break;
        }
    }
    if(_current_depth < 0){
        return nullptr;
    }
    _cur_coordinate->_depth = _current_depth;
    _cur_coordinate->_child_ind = curw;

    _cursor_arr[_current_depth]+=1;
    _current_depth+= 1;

    return _cur_coordinate;
}

NTreeIndexer::~NTreeIndexer(){
    if(!_empty){
        delete[] _cursor_arr;
        delete[] _cur_coordinate;
    }
}    

void NTreeIndexer::print(){
    if(_empty){
        return;
    }
    for(int_type i = 0; i < _max_depth; i++){
        std::cout << _cursor_arr[i] << ", ";
    }
    std::cout << std::endl;
}
/*
NTreeIterator
*/


/*

class NTreeIterator{
    int_type _nbranch;
    int_type _cur_depth;
    NodeType* _current;
    NTreeIndexer* _counter;
    public:
        NTreeIterator(NTree* NTree);
        NodeType* next();
};
*/
NTreeIterator::NTreeIterator(NTree* NTree){
    _nbranch = NTree->get_nbranch();
    _cur_depth = 0;
    _current = NTree->get_head();
    int_type max_depth = NTree->get_max_depth();
    _counter = new NTreeIndexer(max_depth,_nbranch);
    _head_counted = false;
}
NTreeIterator::~NTreeIterator(){
    _counter->~NTreeIndexer();
}

NodeType* NTreeIterator::next(){
    if( !_head_counted){
        _head_counted = true;
        return _current;
    }
    NTreeIndex* tc = _counter->next();
    if(tc == nullptr){
        return nullptr;
    }
    int_type depth = tc->_depth;
    while(depth < _cur_depth)
    if(depth - _cur_depth == 1){
        return nullptr;
    }
    return nullptr;
}





NTree::NTree(int_type nbranch,BinaryBuffer* buff){
    _nbranch = nbranch;
    _node_counter = 0;
    _nodes  = std::vector<NodeType*>();
    NodeType* head = new NodeType();
    _nodes.push_back(head);
    _buff = buff;

    BinarySegmentWriter* bsw = buff->new_segment();
    head->to_binary(bsw);
    delete bsw;
}

void NTree::to_file(std::string _filename){
    _buff->to_file(_filename);
}

void NTree::from_file(std::string _filename){
    _buff->from_file(_filename);   
    int_type nsegs = _buff->num_segments();
    BinarySegmentReader* bsr;    
    for(int_type segi = _nodes.size(); segi < nsegs; segi ++ ){
        _nodes.push_back(nullptr);
    }
    NodeType * nt = _nodes[0];
    bsr = _buff->read_segment(0);
    nt->from_binary(bsr,&_nodes);
    delete bsr;
    std::vector<bool> tobechecked(_nodes.size(),false);
    int_type num_children = nt->get_num_children() + 1;
    if(num_children == 1){
        return;
    }
    for(int_type segi = 1; segi < num_children; segi ++){
        tobechecked[segi] = true;
    }
    int_type i = 1;
    int_type i1;
    int_type i2;
    while(i < tobechecked.size()){
        bsr = _buff->read_segment(i);        
        nt = new NodeType();
        nt->from_binary(bsr,&_nodes);
        delete bsr;

        tobechecked[i] = false;
        num_children = nt->get_num_children() + 1;
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


NodeType* NTree::branch_from_node(NodeType* x,int_type num_children){
    if(! (x->is_leaf())){
        throw std::invalid_argument("The node " + std::to_string(x->get_node_id()) + " is not a leaf!");
    }
    x->set_num_children(num_children);
    int_type first_child_node_id = _nodes.size();
    x->add_children(&_nodes,first_child_node_id);
    NodeType* cursor = x->get_children();
    BinarySegmentWriter* bsw;
    for(int_type i = 0; i < num_children; i ++, cursor++ ){
        bsw = _buff->new_segment();
        cursor->to_binary(bsw);
        delete bsw;
    }    
    return x->get_children();
}


void NTree::print(){
    int_type mother_id = -1;
    int_type node_id;
    int_type width = 6;
    for(auto nodeptr: _nodes){
        if(!(nodeptr->is_first())){
            mother_id = nodeptr->get_mother()->get_node_id();
        }
        node_id = nodeptr->get_node_id();
        std::cout <<std::setw(width/2)<< node_id << std::setw(width) 
                << "<<" <<std::setw(width) << mother_id<<std::endl;        
    }
}   