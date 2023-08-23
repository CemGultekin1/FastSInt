#pragma once
#include <fstream>
#include <iostream>
#include "constants.h"
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;


class BinarySegmentWriter{
    std::vector<char>* _binary;
    int_type _size;
    public:
        BinarySegmentWriter(std::vector<char> * binary);
        void write(char* x,size_t size);
};



class BinarySegmentReader{    
    const char* const  _binary;
    const int_type _sizeof;
    char* _cursor;
    const int_type _segment_id;
    public:
        BinarySegmentReader(const char* const  binary, int_type __sizeof,const int_type segment_id):
                            _binary(binary),
                            _sizeof(__sizeof),
                            _segment_id(segment_id),
                            _cursor((char*) binary){}
        char* next(size_t size);
        const int_type get_segment_id() const{return _segment_id;};
        const int_type get_size() const{return _sizeof;};
        const char* const get_binary() const{return _binary;};
};


class BinaryBuffer{
    std::vector<char> _binary;
    std::vector<int_type> _binary_beginnings;
    int_type _segment_counter;
    public: 
        BinaryBuffer();
        BinarySegmentWriter* new_segment();
        BinarySegmentReader* read_segment(int_type segmentid) const;
        void to_file(std::string & path) const;
        void from_file(std::string & path);
        void print() const;
        ~BinaryBuffer();
        int_type num_segments();
};
