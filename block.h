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

        block(shared_ptr<fstream> _file_stream, int _this_offset, int _block_size);
        int get_next_offset();
        int set_next_offset(int offset);
        int get_prev_offset();
        int set_prev_offset(int offset);
        int get_data_offset();
        template<class T>
        void write_buffer(const T& buffer){
            int writable_size = block_size - sizeof(int) * 2;
            int type_size = sizeof(T);
            if(type_size > writable_size){
                std::cout << "write type size is bigger than block writable size" << std::endl;
                return;
            }
            file_stream->seekg(this_offset + sizeof(int) * 2);
            file_stream->write((const char*)&buffer, type_size);
        }
        template<class T>
        void read_buffer(T& buffer){
            int writable_size = block_size - sizeof(int) * 2;
            int type_size = sizeof(T);
            if(type_size > writable_size){
                std::cout << "read type size is bigger than block writable size" << std::endl;
                return;
            }
            file_stream->seekg(this_offset + sizeof(int) * 2);
            file_stream->read((char*)&buffer, type_size);
        }
        shared_ptr<block> next_block();
        shared_ptr<block> prev_block();
    };
    //file_queueはブロックの走査・参照・追加・削除をする。使用済みと空ブロックの管理を実行し、super_blockに保存する
    struct file_list {
        shared_ptr<fstream> file_stream;

        file_list(string path, int block_size);
        shared_ptr<block> new_block();
        void delete_block(int block_offset);
        shared_ptr<block> first_block();
        shared_ptr<block> final_block();
    };
}

