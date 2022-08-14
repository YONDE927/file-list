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

    shared_ptr<block> block::prev_block(){
        return std::make_shared<block>(file_stream, prev_offset, block_size);
    }
}
