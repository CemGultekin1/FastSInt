

template <class Feature>
DAG<Feature>::DAG(int_type width){
    _width = width;
    _depth = 0;
    _edges  = EdgeVec();
    Edge* head = new Edge();
    _edges.push_back(head);
    _edge_features = FeatureVec();
}


// template <class Feature>
// void DAG<Feature>::edge_to_file(BinarySegmentWriter*bsw,DAG<Feature>::Edge* edgeptr) const{
//     edgeptr->to_binary(bsw);
// }
    
    
// template <class Feature>
// void DAG<Feature>::edge_from_file(BinarySegmentReader*bsr,DAG<Feature>::Edge* edgeptr){
//     edgeptr->from_binary(bsr,&_edges);
//     int_type edge_id = edgeptr->_edge_id;
//     if(_edge_features.size() > edge_id){
//         Feature* feat = _edge_features[edge_id]
//         edgeptr->add_feature(feat);
//     }

// }


template <class Feature>
void DAG<Feature>::to_file(std::string _filename){
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

template <class Feature>
void DAG<Feature>::from_file(std::string _filename){
    BinaryBuffer* buff = new BinaryBuffer();
    buff->from_file(_filename);  
    int_type nsegs = buff->num_segments() - 1;
    BinarySegmentReader* bsr;    
    _edges.resize(nsegs,nullptr);
    Edge * nt = _edges[0];    
    
    bsr = buff->read_segment(0);
    _width = *((int_type*) bsr->next(sizeof(_width)));
    _depth = *((int_type*) bsr->next(sizeof(_depth)));
    delete bsr;

    bsr = buff->read_segment(1);
    nt->from_binary(bsr,&_edges,&_edge_features);
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
        nt = _edges[i]; 
        nt->from_binary(bsr,&_edges,&_edge_features);
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

template <class Feature>
int_type DAG<Feature>::get_width(){
    return _width;
}
template <class Feature>
int_type DAG<Feature>::get_max_depth(){
    return _depth;
}
template <class Feature>
GraphEdgeType<Feature>* DAG<Feature>::get_head(){
    return _edges[0];
}
template <class Feature>
DAG<Feature>::~DAG(){
    delete _edges[0];
    _edges.clear();
    _edge_features.clear();
}

template <class Feature>
GraphEdgeType<Feature>* DAG<Feature>::branch_from_edge(Edge* x,int_type num_children, int_type first_child_edge_id ){
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
    int_type new_depth = x->_depth + 1;
    if(new_depth > _depth){
        _depth = new_depth;
    }
    return x->get_children();
}


template <class Feature>
void DAG<Feature>::print(){
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
template <class Feature>
void DAG<Feature>::add_feature(Feature*feat,Edge*edge){
    int_type edge_id = edge->_edge_id;
    _edge_features->resize(edge_id +1 ,nullptr);
    _edge_features[edge_id] = feat;
    edge->add_features(feat);
}