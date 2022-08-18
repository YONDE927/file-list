#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <unistd.h>

using std::shared_ptr, std::fstream, std::vector, std::string;

namespace fili{
    //ブロックは自身の設定の読み書きとデータ部の読み書きができ、隣接ブロックへの参照が可能,始祖ブロックのアクセスはできない
    //相互にオフセットを変更するので不変である自身のオフセットとブロックサイズ以外はメンバ変数に持たずに毎度読み込む。
    class block{
        private:
            int this_offset;
            int block_size;
            shared_ptr<fstream> file_stream;

        public:
            block(shared_ptr<fstream> _file_stream, int _this_offset, int _block_size);
            int get_this_offset();
            int get_next_offset();
            void set_next_offset(int offset);
            int get_prev_offset();
            void set_prev_offset(int offset);
            shared_ptr<block> next_block();
            shared_ptr<block> prev_block();

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
                int readable_size = block_size - sizeof(int) * 2;
                int type_size = sizeof(T);
                if(type_size > readable_size){
                    std::cout << "read type size is bigger than block readable size" << std::endl;
                    return;
                }
                file_stream->seekg(this_offset + sizeof(int) * 2);
                file_stream->read((char*)&buffer, type_size);
            }
    };
    //file_queueはブロックの走査・参照・追加・削除をする。使用済みと空ブロックの管理を実行し、始祖ブロックに保存する
    //block_offset系については高速化のための妥協である。メンバ変数だけ変えてファイルを変更しない実装のリスクがある。
    class file_list {
        private:
            string path;
            shared_ptr<fstream> file_stream;
            int block_size;
            int first_block_offset;
            int final_block_offset;
            int empty_block_offset;
            int real_final_block_offset;

            int get_block_size();
            void set_block_size(int size);
            int get_first_block_offset();
            void set_first_block_offset(int offset);
            int get_final_block_offset();
            void set_final_block_offset(int offset);
            int get_empty_block_offset();
            void set_empty_block_offset(int offset);
            int get_real_final_block_offset();
            void set_real_final_block_offset(int offset);

        public:
            file_list(string _path);
            file_list(string _path, int _block_size);
            shared_ptr<block> new_front_block();
            shared_ptr<block> new_back_block();
            shared_ptr<block> first_block();
            shared_ptr<block> final_block();
            void reset();
            void delete_block(int block_offset);
            void delete_block(shared_ptr<block> block_ptr);
    };
}

