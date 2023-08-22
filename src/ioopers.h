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
    const char*  _binary;
    const int_type _sizeof;
    char* _cursor;
    public:
        BinarySegmentReader(const char*  binary, int_type __sizeof):
                            _binary(binary),
                            _sizeof(__sizeof){
                                _cursor = (char*) binary;
        }
        char* next(size_t size);
};


class BinaryBuffer{
    std::vector<char> _binary;
    std::vector<int_type> _binary_beginnings;
    std::string _path;
    int_type _segment_counter;
    public: 
        BinaryBuffer(std::string & path);
        BinarySegmentWriter* new_segment();
        BinarySegmentReader* read_segment(int_type segmentid) const;
        void to_file() const;
        void from_file();
        void print() const;
        ~BinaryBuffer();
        int_type num_segments();
};
