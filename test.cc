#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "block.h"

using namespace std;

struct obj{
    int a;
    int b;
    double c;
};

int main(){
    string path = "./file.li";
    fili::file_list list(path, 32);

    list.reset();

    auto block_ptr1 = list.new_back_block();
    obj buffer1{1,2,3};
    obj rbuffer1{0};
    block_ptr1->write_buffer<obj>(buffer1);
    //fili::show_fili(path, 32);
    block_ptr1->read_buffer<obj>(rbuffer1);
    cout << endl << rbuffer1.a << " " << rbuffer1.b << " " << rbuffer1.c << endl;

    cout << endl;

    auto block_ptr2 = list.new_back_block();
    obj buffer2{100,200,300};
    block_ptr2->write_buffer<obj>(buffer2);
    //fili::show_fili(path, 32);
    //auto block_ptr3 = list.new_front_block();
    //list.delete_block(block_ptr2);
    //auto block_ptr4 = list.new_back_block();
    return 0;
}
