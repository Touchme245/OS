#pragma once
#include <iostream>
bool equals(char* st1, const char * st2){
    int i = 0;
    // std::cout << st1 << " " << st2 << "\n";
    while (st1[i] != '\0'){
        if (st1[i] != st2[i]){

            // std::cout << st1[i] << " " << st2[i] <<  "  " << i <<std::endl;
            return false;
        }
        i++;
    }
    if (st2[i] == '\0'){
        return true;
    }
    return false;
}