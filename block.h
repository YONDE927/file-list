#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>

using std::shared_ptr, std::fstream, std::vector;

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

        block(shared_ptr<fstream> _fs, int _offset, int _block_size);
        shared_ptr<block> prev_block();
        shared_ptr<block> next_block();
        int delete_block();
    };
    
    struct super_block: public block{
        int block_size;
        int block_num;
        int final_block_offset;
        char path[256];
    };

    struct used_block: public block{
        std::unique_ptr<char> read_block();
        int write_block(std::vector<char>& buffer);
    };

    class empty_block: public block{
    };
}

