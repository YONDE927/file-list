#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include "block.h"

using namespace std;

int main(){
    fili::file_list list("./file.li", 32);

    list.reset();

    auto block_ptr1 = list.new_back_block();
    auto block_ptr2 = list.new_back_block();
    auto block_ptr3 = list.new_front_block();
    list.delete_block(block_ptr2);
    auto block_ptr4 = list.new_back_block();
    return 0;
}
