#include "Ter.h"
#include <string>

char* translate(long x){
    std::string tmp = "";
    while (x>0){
        tmp = std::to_string(x % 3) + tmp;
        x /=3;
    }

    char* res = new char[tmp.length()];
    for (int i = 0; i < tmp.length(); ++i){
        res[i] = tmp[i];
    }
    res[tmp.length()] = '\0';
    return res;
    
}