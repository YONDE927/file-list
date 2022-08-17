#include <stdexcept>
#include "block.h"

namespace fili{
    int get_int(shared_ptr<fstream> file_stream, int offset){
        int value{0};
        file_stream->seekg(offset);
        file_stream->read((char*)&value, sizeof(int));
        return value;
    }

    void set_int(shared_ptr<fstream> file_stream, int offset, int value){
        file_stream->seekg(offset);
        file_stream->write((const char*)&value, sizeof(int));
    }

    block::block(shared_ptr<fstream> _file_stream, int _this_offset, int _block_size){
        file_stream = _file_stream;
        this_offset = _this_offset;
        block_size = _block_size;
    }

    int block::get_this_offset(){
        return this_offset;
    }

    int block::get_next_offset(){
        return get_int(file_stream, this_offset + sizeof(int));
    }

    void block::set_next_offset(int offset){
        set_int(file_stream, this_offset + sizeof(int), offset);
    }

    int block::get_prev_offset(){
        return get_int(file_stream, this_offset);
    }

    void block::set_prev_offset(int offset){
        set_int(file_stream, this_offset, offset);
    }

    shared_ptr<block> block::next_block(){
        int next_offset = get_next_offset();
        if(next_offset == -1){
            return shared_ptr<block>();
        }else{
            return std::make_shared<block>(file_stream, next_offset, block_size);
        }
    }

    shared_ptr<block> block::prev_block(){
        int prev_offset = get_prev_offset();
        if(prev_offset == -1){
            return shared_ptr<block>();
        }else{
            return std::make_shared<block>(file_stream, prev_offset, block_size);
        }
    }

    file_list::file_list(string _path){
        path = _path;
        file_stream = std::make_shared<fstream>(path, std::ios::in | std::ios::out | std::ios::app);
        block_size = get_int(file_stream, 0);
        first_block_offset = get_int(file_stream, sizeof(int));
        final_block_offset = get_int(file_stream, 0 + sizeof(int) * 2);
        empty_block_offset = get_int(file_stream, 0 + sizeof(int) * 3);
        real_final_block_offset = get_int(file_stream, 0 + sizeof(int) * 4);
    }

    file_list::file_list(string _path, int _block_size){
        if(_block_size < 32){
            throw std::runtime_error("block_size must be bigger than 32");
        }
        path = _path;
        file_stream = std::make_shared<fstream>(path, std::ios::in | std::ios::out | std::ios::app);
        set_block_size(_block_size);
        set_first_block_offset(-1);
        set_final_block_offset(-1);
        set_empty_block_offset(-1);
        set_real_final_block_offset(0);
    }

    int file_list::get_block_size(){
        return get_int(file_stream, 0);
    }
    void file_list::set_block_size(int size){
        block_size = size;
        set_int(file_stream, 0, size);
    }
    int file_list::get_first_block_offset(){
        return get_int(file_stream, sizeof(int));
    }
    void file_list::set_first_block_offset(int offset){
        first_block_offset = offset;
        set_int(file_stream, 0 + sizeof(int), offset);
    }
    int file_list::get_final_block_offset(){
        return get_int(file_stream, 0 + sizeof(int) * 2);
    }
    void file_list::set_final_block_offset(int offset){
        final_block_offset = offset;
        set_int(file_stream, 0 + sizeof(int) * 2, offset);
    }
    int file_list::get_empty_block_offset(){
        return get_int(file_stream, 0 + sizeof(int) * 3);
    }
    void file_list::set_empty_block_offset(int offset){
        empty_block_offset = offset;
        set_int(file_stream, 0 + sizeof(int) * 3, offset);
    }

    int file_list::get_real_final_block_offset(){
        return real_final_block_offset;
    }
    void file_list::set_real_final_block_offset(int offset){
        real_final_block_offset = offset;
        set_int(file_stream, 0 + sizeof(int) * 4, offset);
    }

    shared_ptr<block> file_list::new_back_block(){
        //オフセットを取得
        int new_offset{0};
        //空ブロックがあるならば
        if(empty_block_offset != -1){
            new_offset = empty_block_offset;
            //最初の空ブロックから次の空ブロックを取得
            auto empty_block = std::make_shared<block>(file_stream, empty_block_offset, block_size);
            //次の空ブロックを空ブロックの先頭に変更
            set_empty_block_offset(empty_block->get_next_offset());
        }else{
        //空ブロックがなければ
            new_offset = real_final_block_offset + block_size;
            //累積ブロック数に追加
            set_real_final_block_offset(real_final_block_offset + block_size);
        }

        //ファイル上のブロック初期化
        //prev_offset
        set_int(file_stream, new_offset, final_block_offset);
        //next_offset
        set_int(file_stream, new_offset + sizeof(int), -1);

        //前のブロックの編集
        auto fi_block = final_block();
        if(fi_block != nullptr){
            fi_block->set_next_offset(new_offset);
        }

        //始祖ブロックの編集
        //生成したブロックが唯一ならば
        if((first_block_offset == -1) && (final_block_offset == -1)){
            set_first_block_offset(new_offset);
        }
        //生成したブロックを最終ブロックに設定
        set_final_block_offset(new_offset);

        //ブロックインスタンスを初期化
        return std::make_shared<block>(file_stream, new_offset, block_size);
    }

    shared_ptr<block> file_list::new_front_block(){
        //オフセットを取得
        int new_offset{0};
        //空ブロックがあるならば
        if(empty_block_offset != -1){
            new_offset = empty_block_offset;
            //最初の空ブロックから次の空ブロックを取得
            auto empty_block = std::make_shared<block>(file_stream, empty_block_offset, block_size);
            //次の空ブロックを空ブロックの先頭に変更
            set_empty_block_offset(empty_block->get_next_offset());
        }else{
        //空ブロックがなければ
            new_offset = real_final_block_offset + block_size;
            //累積ブロック数に追加
            set_real_final_block_offset(real_final_block_offset + block_size);
        }

        //新規ブロックの初期化
        //prev_offset
        set_int(file_stream, new_offset, -1);
        //next_offset
        set_int(file_stream, new_offset + sizeof(int), first_block_offset);

        //後のブロックの編集
        auto fi_block = first_block();
        if(fi_block != nullptr){
            fi_block->set_prev_offset(new_offset);
        }

        //始祖ブロックの編集
        //生成したブロックが唯一ならば
        if((first_block_offset == -1) && (final_block_offset == -1)){
            set_final_block_offset(new_offset);
        }
        set_first_block_offset(new_offset);

        //ブロックインスタンスを初期化
        return std::make_shared<block>(file_stream, new_offset, block_size);
    }

    shared_ptr<block> file_list::first_block(){
        if(first_block_offset == -1){
            return shared_ptr<block>(nullptr);
        }else{
            return std::make_shared<block>(file_stream, first_block_offset, block_size);
        }
    }

    shared_ptr<block> file_list::final_block(){
        if(final_block_offset == -1){
            return shared_ptr<block>(nullptr);
        }else{
            return std::make_shared<block>(file_stream, final_block_offset, block_size);
        }
    }

    void file_list::reset(){
        file_stream = std::make_shared<fstream>(path, std::ios::in | std::ios::out | std::ios::trunc);
        set_block_size(block_size);
        set_first_block_offset(-1);
        set_final_block_offset(-1);
        set_empty_block_offset(-1);
        set_real_final_block_offset(0);
    }

    void file_list::delete_block(int block_offset){
        if(block_offset == -1){
            return;
        }
        auto this_block = std::make_shared<block>(file_stream, block_offset, block_size);
        
        delete_block(this_block);
    }

    void file_list::delete_block(shared_ptr<block> this_block){
        if(this_block == nullptr){
            return;
        }

        int block_offset = this_block->get_this_offset();
       
        //前ブロックを編集
        shared_ptr<block> prev_block = this_block->prev_block();
        if(prev_block != nullptr){
            prev_block->set_next_offset(this_block->get_next_offset());
        }
        //後ブロックを編集
        shared_ptr<block> next_block = this_block->next_block();
        if(next_block != nullptr){
            next_block->set_prev_offset(this_block->get_prev_offset());
        }
        //先頭ブロックの場合
        if(block_offset == first_block_offset){
            set_first_block_offset(this_block->get_next_offset());
        }
        //後尾ブロックの場合
        if(block_offset == final_block_offset){
            set_final_block_offset(this_block->get_prev_offset());
        }
        //真の後尾ブロックの場合、総オフセットを切り詰め
        if(block_offset == real_final_block_offset){
            set_real_final_block_offset(real_final_block_offset - block_size);
        }else if(block_offset < real_final_block_offset){
            this_block->set_prev_offset(0); 
            this_block->set_next_offset(empty_block_offset); 
            set_empty_block_offset(this_block->get_this_offset());
        }
    }
}
