#include "libTranslation.h"
#include <string>
char* toBinary(long x){
    std::string tmp = "";
    while (x>0){
        tmp = std::to_string(x % 2) + tmp;
        x /=2;
    }

    char* res = new char[tmp.length()];
    for (int i = 0; i < tmp.length(); ++i){
        res[i] = tmp[i];
    }
    res[tmp.lenght()] = '\0';
    return res;
    
}

char* toTernary(long x){
    std::string tmp = "";
    while (x>0){
        tmp = std::to_string(x % 3) + tmp;
        x /=3;
    }

    char* res = new char[tmp.length()];
    for (int i = 0; i < tmp.length(); ++i){
        res[i] = tmp[i]; 
    }
    res[tmp.lenght()] = '\0';
    return res;
    
}
