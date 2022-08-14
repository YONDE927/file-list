#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <unistd.h>

using std::shared_ptr, std::fstream, std::vector, std::string;

namespace fique{
    //block
    //次のブロックオフセットを持ち、参照することができる。
    struct block{
        int this_offset;
        int prev_offset;
        int next_offset; 
        int block_size;
        vector<char> buffer;
        shared_ptr<fstream> file_stream;

        //既存ブロックからの初期化
        block(shared_ptr<fstream> _fs, int _offset, int _block_size);
        //新規ブロックからの初期化
        block(shared_ptr<fstream> _fs, int _offset, int _block_size, int _prev_offset, int _next_offset);
        shared_ptr<block> prev_block();
        shared_ptr<block> next_block();
        void delete_block();
    };
    
    struct super_block: public block{
        int block_num;
        int first_empty_block_offset;
        int first_used_block_offset;
        int final_used_block_offset;

        super_block(shared_ptr<fstream> _file_stream, int _block_size);
        shared_ptr<block> new_block();
    };

    struct used_block: public block{
        std::unique_ptr<char> read_block();
        int write_block(std::vector<char>& buffer);
    };

    class empty_block: public block{
    };
}

