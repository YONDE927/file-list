#include "block.h"

namespace fique{
    block::block(shared_ptr<fstream> _file_stream, int _this_offset, int _block_size){
        file_stream = _file_stream;
        this_offset = _this_offset;
        block_size = _block_size;
    }

    int block::get_next_offset(){
        int offset{0};
        file_stream->seekg(this_offset);
        file_stream->read((char*)&offset, sizeof(int)); 
        return offset;
    }

    int block::set_next_offset(int offset){
        file_stream->seekg(this_offset);
        file_stream->write((const char*)&offset, sizeof(int)); 
        return offset;
    }

    int block::get_prev_offset(){
        int offset{0};
        file_stream->seekg(this_offset + sizeof(int));
        file_stream->read((char*)&offset, sizeof(int)); 
        return offset;
    }

    int block::set_prev_offset(int offset){
        file_stream->seekg(this_offset + sizeof(int));
        file_stream->write((const char*)&offset, sizeof(int)); 
        return offset;
    }

    shared_ptr<block> block::next_block(){
        int next_offset = get_next_offset();
        if(next_offset == -1){
            return shared_ptr<block>();
        }else{
            return std::make_shared<block>(file_stream, next_offset, block_size);
        }
    }
}
