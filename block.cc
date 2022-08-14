#include "block.h"

namespace fique{
    block::block(shared_ptr<fstream> _file_stream, int _offset, int _block_size){
        file_stream = _file_stream;
        this_offset = _offset;
        block_size = _block_size;
        file_stream->seekg(_offset);
        file_stream->read((char*)&prev_offset, sizeof(int));
        file_stream->read((char*)&next_offset, sizeof(int));
    }
    
    block::block(shared_ptr<fstream> _file_stream, int _offset, int _block_size, int _prev_offset, int _next_offset){
        file_stream = _file_stream;
        this_offset = _offset;
        block_size = _block_size;
        prev_offset = _prev_offset;
        next_offset = _next_offset;
    }

    shared_ptr<block> block::prev_block(){
        return std::make_shared<block>(file_stream, prev_offset, block_size);
    }

    shared_ptr<block> block::next_block(){
        return std::make_shared<block>(file_stream, prev_offset, block_size);
    }

    void block::delete_block(){
        file_stream->seekg(prev_offset + sizeof(int));
        file_stream->write((const char*)&next_offset, sizeof(int));
    }

    super_block::super_block(shared_ptr<fstream> _file_stream, int _block_size): block(_file_stream, 0, _block_size, -1, -1){
        block_num = 0; 
        first_empty_block_offset = block_size; 
        first_used_block_offset = -1; 
        final_used_block_offset = -1;
        //ファイルへの書き込み
    }

    shared_ptr<block> super_block::new_block(){}
}
