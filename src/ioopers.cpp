#include "ioopers.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;



BinaryBuffer::BinaryBuffer(){
    _binary = std::vector<char>();
    _binary_beginnings = std::vector<int_type>();
    _segment_counter = 0;
}

BinarySegmentWriter* BinaryBuffer::new_segment(){
    BinarySegmentWriter* bsw = new BinarySegmentWriter(&_binary);
    _binary_beginnings.push_back(_binary.size());
    _segment_counter += 1;
    return bsw;
}

BinarySegmentReader* BinaryBuffer::read_segment(int_type segmentid) const{
    int_type start = _binary_beginnings[segmentid];
    int_type end = 0;
    if(segmentid == _segment_counter - 1){
        end = _binary.size();
    }else{
        end = _binary_beginnings[segmentid+1];
    }
    const int_type len = end - start;
    const char*  binary_pointer = _binary.data();
    return new BinarySegmentReader(&binary_pointer[start],len,segmentid);
}

void BinaryBuffer::print() const{
    std::cout << "segments : " << std::endl;
    for(auto x : _binary_beginnings){
        std::cout << x << ", ";
    }
    std::cout << std::endl;
}

void BinaryBuffer::to_file(std::string & path) const{
    std::ofstream myfile;

    myfile.open(path);
    int_type x = _binary_beginnings.size()*sizeof(int_type);
    myfile.write((char*) &x, sizeof(x));
    myfile.write((char*) _binary_beginnings.data(), x);
    // myfile.close();

    // path = fs::path(_rootdir) / fs::path(_binary_path);
    // myfile.open(path.string());
    x = _binary.size()*sizeof(char);

    myfile.write((char*) &x, sizeof(x));
    myfile.write(_binary.data(), x);
    myfile.close();
}

void BinaryBuffer::from_file(std::string & path){
    std::ifstream myfile;
    myfile.open(path);
    int_type x;

    myfile.read((char*) &x, sizeof(x));
    int_type y;
    _segment_counter = 0;
    for(int_type i = 0; i < x/sizeof(y); i++){
        myfile.read((char*) &y, sizeof(y));
        _binary_beginnings.push_back(y);
        _segment_counter += 1;
    }


    char r;
    myfile.read((char*) &x, sizeof(x));
    for(int_type i = 0; i < x; i++){
        myfile.read((char*) &r, sizeof(r));
        _binary.push_back(r);
    }
    myfile.close();
}

int_type BinaryBuffer::num_segments(){
    return (int_type) _binary_beginnings.size();
}

BinaryBuffer::~BinaryBuffer(){
    _binary.clear();
    _binary_beginnings.clear();
}





BinarySegmentWriter::BinarySegmentWriter(std::vector<char> * binary){
    _binary = binary;
    _size = 0;
}
void BinarySegmentWriter::write(char*x, size_t size){
    _size += size;
    for(char* xp = x; xp - x < size; xp ++ ){
        _binary->push_back(*xp);
    }
}


char* BinarySegmentReader::next(size_t size){
    char* cursor = _cursor;
    _cursor += size;
    if(_cursor  - _binary> _sizeof ){
        return (char*) nullptr;
    }
    return cursor;
}

