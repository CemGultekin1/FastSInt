

template <class DB>
DataBaseInterface<DB>::DataBaseInterface(DB* adb){
    _abstr_db = adb;
    _expressive_data_id_subset = 
        std::vector<int_type>();    
    _length = 0;
    _counter = 0;
    _dim = NLLC;    
}

template <class DB>
void DataBaseInterface<DB>::init_virtual_nodes(){
    _expressive_data_id_subset.resize(_dim+1, NLLC);
    _counter = _expressive_data_id_subset.size();
}

template <class DB>
DataPoint* DataBaseInterface<DB>::next(){
    DataPoint* dp = (*_abstr_db)[_counter];
    if(dp == nullptr){
        return dp;
    }
    if(_dim == NLLC){
        _dim = dp->_dim;
        init_virtual_nodes();
    }
    dp->_data_id = _counter;
    if(dp != nullptr){
        _counter ++;
        if(_length < _counter){
            _length = _counter;
        }
    }
    return dp;
}


template <class DB>
void DataBaseInterface<DB>::reset_counter(){
    _counter = 0;
}


template <int_type DIM,int_type LENGTH>
DataPoint*  RandomDataBase<DIM,LENGTH>::operator[](int_type i){
    if(i>=LENGTH){
        return nullptr;
    }
    float_type* weights = new float_type[DIM];
    random_vector_generator rvg(i+1);    
    return allocate_random_dp(DIM,rvg);
}
