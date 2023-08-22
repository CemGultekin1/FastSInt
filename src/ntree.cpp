#include  "ntree.h"





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

void NodeType::from_binary(BinarySegmentReader* nd,node_list* nodes){
    _node_id = nd->get_segment_id();
    const char* const binary = nd->get_binary();
    int_type* _children_node_ids = (int_type*) binary;
    _num_children = nd->get_size()/sizeof(int_type);
    // _children = (NodeType*) malloc(sizeof(NodeType)*_num_children);
    int_type zero_children = 0;
    NodeType* child;
    for(int_type i=0; i < _num_children; i++){
        child = new NodeType(this,(NodeType*) nullptr,true,zero_children,_children_node_ids[i]);
        nodes->push_back(child);
        _children[i] = *child;
    }
    return;
}

void NodeType::add_children(int_type num_children, node_list* nodes){
    int_type first_node = nodes->size();
    for(int_type i = 0; i < num_children; i++){
        return;
    }
    return;
}

void NodeType::to_binary(BinarySegmentWriter* bb) const{
    // bb->write((char*) &_node_id, sizeof(_node_id));
    int_type node_id;
    for(int_type i=0; i<_num_children; i++){        
        bb->write((char*) &_children[i]._node_id, sizeof(_node_id));        
    }
}

bool NodeType::is_leaf(){
    return _leaf;
}

bool NodeType::is_first(){
    return _mother == nullptr;
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
    _buff = buff;
    _node_counter = 0;
    _nodes  = node_list();
    _nodes.push_back(new NodeType());
}
void to_file(std::string _filename){
    std::ofstream file(_filename);
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


NodeType* NTree::add_children(NodeType* x,int_type num_children){

}

// void from_file(std::string _filename);