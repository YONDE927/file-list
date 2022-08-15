#include "block.h"

namespace fique{
    super_block::super_block(string _path, int _block_size){
        block_num = 0; 
        first_empty_block_offset = block_size; 
        final_empty_block_offset = block_size; 
        first_used_block_offset = -1; 
        final_used_block_offset = -1;
        //ファイルへの書き込み
        file_stream->seekg(0);
        file_stream->write((const char*)&block_num, sizeof(int));
        file_stream->write((const char*)&first_empty_block_offset, sizeof(int));
        file_stream->write((const char*)&final_empty_block_offset, sizeof(int));
        file_stream->write((const char*)&first_used_block_offset, sizeof(int));
        file_stream->write((const char*)&final_used_block_offset, sizeof(int));
    }

    shared_ptr<block> super_block::new_block(){
        int new_block_offset{0};
        //空ブロックが存在しない場合、使用済みオフセットに1ブロック分のサイズを加算したオフセットを作成する
        if(first_empty_block_offset == -1){
            new_block_offset = final_used_block_offset + block_size;
        }else{
            new_block_offset = first_empty_block_offset;
            auto eblock = std::make_shared<block>();
            first_empty_block_offset = 
        }
    }

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
        //ファイルへの書き込み
        file_stream->seekg(this_offset);
        file_stream->write((const char*)&block_size, sizeof(int));
        file_stream->write((const char*)&prev_offset, sizeof(int));
        file_stream->write((const char*)&next_offset, sizeof(int));
    }

    shared_ptr<block> block::prev_block(){
        if(prev_offset == -1){
            shared_ptr<block> pb(nullptr);
            return pb;
        }
        return std::make_shared<block>(file_stream, prev_offset, block_size);
    }

    shared_ptr<block> block::next_block(){
        if(next_offset == -1){
            shared_ptr<block> pb(nullptr);
            return pb;
        }
        return std::make_shared<block>(file_stream, next_offset, block_size);
    }

    void block::delete_block(){
        file_stream->seekg(prev_offset + sizeof(int));
        file_stream->write((const char*)&next_offset, sizeof(int));
        //削除ブロックのオフセットが使用済みブロックの手前にある場合、空ブロックとして登録する。
        if(this_offset < final_used_block_offset){
            //final
            file_stream->seekg(sizeof(int));
        }
    }

}
