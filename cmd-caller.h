#pragma once
#include <stdio.h>
#include <string.h>

void call_cmd(const char* cmd){
    FILE* pipe;

    pipe = popen(cmd, "r");
    if(pipe == NULL){
        return;
    }

    size_t n = 1;
    char buf[64];
    while(n > 0){
        bzero(buf, 64);
        n = fread(buf, 1, 64, pipe);
        printf("%s", buf);
    }
}
