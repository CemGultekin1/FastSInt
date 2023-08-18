#include  "iotree.h"

/*
NodeData
*/

NodeData::NodeData(){
    _binary = nullptr;
    _sizeof = 0;
}

NodeData::NodeData(char* binary,int_type len, int_type typesize){
    _binary = binary;
    _sizeof = len*typesize;
}

void NodeData::write(std::ofstream &file){
    file.write((char*) &_sizeof,sizeof(_sizeof));
    file.write(_binary,_sizeof);
    return;
}
void NodeData::read(std::ifstream &file){
    file.read((char*) &_sizeof,sizeof(_sizeof));
    _binary = (char*) malloc(_sizeof);
    file.read(_binary,_sizeof);
    return;
}
NodeData::~NodeData(){
    if(_binary != nullptr){
        delete [] _binary;
    }    
}



/*
NodeType
*/

NodeType::NodeType(NodeType* mother ,NodeType* children , bool* leaf,int_type& num_children){
    _mother = mother;
    _children = children;
    _leaf = leaf;
    _node_data = nullptr;
    _num_children = num_children;
}
void NodeType::assimilate_data(NodeData* nd){
    _node_data = nd;
}
void NodeType::write(std::ofstream &file, int_type& num){
    file.write((char*) &_num_children,sizeof(_num_children));
    if( _num_children == 0){
        return;
    }
    if(num == _num_children){ /* non full children */
        file.write((char*) _leaf,sizeof(bool)*num);
    }
    _node_data->write(file);
    return;
}


void NodeType::read(std::ifstream &file, int_type& num){
    file.read((char*) &_num_children,sizeof(_num_children));
    if(_num_children == 0){
        return;
    }
    if(num == _num_children){ /* non full children */
        file.read((char*) _leaf,sizeof(bool)*num);
    }
    _node_data->read(file);
    return;
}

bool NodeType::is_leaf(){
    return _node_data == nullptr;
}

bool NodeType::is_first(){
    return _mother == nullptr;
}


/*
TreeCoordinate
*/
void TreeCoordinate::print(){
    std::cout << _depth << " " << _child_ind << std::endl;
}


/*
TreeCounter
*/

TreeCounter::TreeCounter(int_type max_depth,int_type max_width){
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
        _cur_coordinate = new TreeCoordinate(0,0);
    }
}

TreeCoordinate* TreeCounter::next(){
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

TreeCounter::~TreeCounter(){
    if(!_empty){
        delete[] _cursor_arr;
        delete[] _cur_coordinate;
    }
}    

void TreeCounter::print(){
    if(_empty){
        return;
    }
    for(int_type i = 0; i < _max_depth; i++){
        std::cout << _cursor_arr[i] << ", ";
    }
    std::cout << std::endl;
}
/*
TreeWalker
*/


/*

class TreeWalker{
    int_type _nbranch;
    int_type _cur_depth;
    NodeType* _current;
    TreeCounter* _counter;
    public:
        TreeWalker(IOTree* iotree);
        NodeType* next();
};
*/
TreeWalker::TreeWalker(IOTree* iotree){
    _nbranch = iotree->get_nbranch();
    _cur_depth = 0;
    _current = iotree->get_head();
    int_type max_depth = iotree->get_max_depth();
    _counter = new TreeCounter(max_depth,_nbranch);
    _head_counted = false;
}
TreeWalker::~TreeWalker(){
    _counter->~TreeCounter();
}

NodeType* TreeWalker::next(){
    if( !_head_counted){
        _head_counted = true;
        return _current;
    }
    TreeCoordinate* tc = _counter->next();
    if(tc == nullptr){
        return nullptr;
    }
    int_type depth = tc->_depth;
    if(depth - _cur_depth == 1){
        return nullptr;
    }
    return nullptr;
}





IOTree::IOTree(int_type nbranch){
    _nbranch = nbranch;
    int_type numchildren = 0;
    NodeType* _head = new NodeType(nullptr,nullptr,nullptr,numchildren);
}
void to_file(std::string _filename){
    std::ofstream file(_filename);
}

int_type IOTree::get_nbranch(){
    return _nbranch;
}

int_type IOTree::get_max_depth(){
    return _depth;
}

NodeType* IOTree::get_head(){
    return _head;
}
// void from_file(std::string _filename);