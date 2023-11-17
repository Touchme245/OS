#include "Bin.h"
#include <string>

char* translate(long x){
    std::string tmp = "";
    int k =0;
    while (x>0){
        tmp = std::to_string(x % 2) + tmp;
        x /=2;
    }

    char* res = new char[k];
    for (int i = 0; i < tmp.length(); ++i){
        res[i] = tmp[i];
    }
    res[tmp.length()] = '\0';
    return res;
    
}
