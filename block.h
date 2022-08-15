#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <unistd.h>

using std::shared_ptr, std::fstream, std::vector, std::string;

namespace fique{
    //ブロックは自身の設定の読み書きとデータ部の読み書きができ、隣接ブロックへの参照が可能
    struct block{
        int this_offset;
        int block_size;
        shared_ptr<fstream> file_stream;

        block(shared_ptr<fstream> _file_stream, _this_offset, _block_size);
        int get_next_offset();
        int set_next_offset();
        int get_prev_offset();
        int set_prev_offset();
        int get_data_offset();
        int write_buffer(const void* buffer, int size);
        int read_buffer(void* buffer, int size);
    };
    //file_queueはブロックの走査・参照・追加・削除をする。使用済みと空ブロックの管理を実行し、super_blockに保存する
    struct file_list {
        shared_ptr<fstream> file_stream;
        file_list(string path, int block_size);
        shared_ptr<block> new_block();
        shared_ptr<block> first_block();
    };
}

