#include "ioopers.h"
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;


/*
class BinaryBuffer{
    std::vector<char> _binary;
    std::vector<int_type> _nbinaries;
    std::vector<int_type> _binary_beginnings;
    std::string _path;
    int_type _segment_counter;
    public: 
        BinaryBuffer(std::string path);
        BinarySegmentWriter* new_segment();
        BinarySegment* read_segment(int_type& segmentid);
        void to_file();
        void from_file();
};
*/

BinaryBuffer::BinaryBuffer(std::string & rootdir){
    if (!fs::is_directory(rootdir) || !fs::exists(rootdir)) { 
        fs::create_directory(rootdir); 
    }
    _rootdir = rootdir;
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
    std::printf("segmentid = %d,\t segment_counter = %d\n",(int)segmentid,(int) _segment_counter);
    if(segmentid == _segment_counter - 1){
        end = _binary.size();
    }else{
        end = _binary_beginnings[segmentid+1];
    }
    const int_type len = end - start;
    const char*  binary_pointer = _binary.data();
    return new BinarySegmentReader(&binary_pointer[start],len);
}

void BinaryBuffer::print() const{
    std::cout << "segments : " << std::endl;
    for(auto x : _binary_beginnings){
        std::cout << x << " ";
    }
    std::cout << std::endl;
}

void BinaryBuffer::to_file() const{
    std::ofstream myfile;

    fs::path path = fs::path(_rootdir) / fs::path(_segmentation_file_name);
    myfile.open(path.string());
    int_type x = _binary_beginnings.size()*sizeof(int_type);
    std::printf("myfile.write((char*) &%d, %d);\n",(int) x, (int) sizeof(x));
    myfile.write((char*) &x, sizeof(x));
    myfile.write((char*) _binary_beginnings.data(), x);
    myfile.close();

    path = fs::path(_rootdir) / fs::path(_binary_file_name);
    myfile.open(path.string());
    x = _binary.size()*sizeof(char);
    std::printf("myfile.write((char*) &%d, %d);\n",(int) x, (int) sizeof(x));
    myfile.write((char*) &x, sizeof(x));
    myfile.write(_binary.data(), x);
    myfile.close();

    
}

void BinaryBuffer::from_file(){
    std::ifstream myfile;
    fs::path path = fs::path(_rootdir) / fs::path(_binary_file_name);
    myfile.open(path.string());
    int_type x;
    char r;
    myfile.read((char*) &x, sizeof(x));
    std::printf("_binary number of elements = %d\n",(int) x);
    for(int_type i = 0; i < x; i++){
        myfile.read((char*) &r, sizeof(r));
        _binary.push_back(r);
    }
    myfile.close();

    
    path = fs::path(_rootdir) / fs::path(_segmentation_file_name);
    myfile.open(path.string());    
    myfile.read((char*) &x, sizeof(x));
    int_type y;
    std::printf("_binary_beginnings number of elements = %d\n",(int) x);
    _segment_counter = 0;
    for(int_type i = 0; i < x/sizeof(y); i++){
        myfile.read((char*) &y, sizeof(y));
        std::printf("_binary_beginnings.push_back(%d);\n",(int) y);
        _binary_beginnings.push_back(y);
        _segment_counter += 1;
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




/*
class BinarySegmentWriter{
    BinaryBuffer* _buff;
    int_type _segment_id;
    public:
        BinarySegmentWriter(BinaryBuffer* buff,int_type & segment_id);
        void write(char* x,size_t size);
};
*/

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

