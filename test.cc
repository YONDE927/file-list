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

    obj rbuffer{0};

    auto block_ptr1 = list.new_back_block();
    obj buffer1{1,2,3};
    block_ptr1->write_buffer<obj>(buffer1);
    block_ptr1->read_buffer<obj>(rbuffer);
    cout << rbuffer.a << " " << rbuffer.b << " " << rbuffer.c << endl;

    auto block_ptr2 = list.new_back_block();
    obj buffer2{100,200,300};
    block_ptr2->write_buffer<obj>(buffer2);
    
    auto block_ptr3 = list.new_back_block();
    obj buffer3{1000,2000,3000};
    block_ptr3->write_buffer<obj>(buffer3);

    auto blo = list.first_block();
    int cnt{0};
    while(blo != nullptr){
        blo->read_buffer<obj>(rbuffer);
        cout << "index: " << cnt++ << endl;
        cout << rbuffer.a << " " << rbuffer.b << " " << rbuffer.c << endl;
        blo = blo->next_block();
    }

    return 0;
}
