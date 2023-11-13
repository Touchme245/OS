#include <iostream>
#include <dlfcn.h>
#include "libTranslation.h"

typedef char* (*FunctionPointer)(long x);

int main(){
    void* lib = dlopen("./libTranslation.so", RTLD_LAZY);
    if (!lib){
        std::cout << "Error with opening lib: " << dlerror() << "\n";
        return 1;
    }
    FunctionPointer toBinary = reinterpret_cast<FunctionPointer>(dlsym(lib, "toBinary"));

    if (!toBinary){
        std::cout << "Error with toBinary pointer: " << dlerror() << "\n"; 
    }

    FunctionPointer toTernary = reinterpret_cast<FunctionPointer>(dlsym(lib, "toTernary"));

    if (!toTernary){
        std::cout << "Error with toTernary pointer: " << dlerror() << "\n";
    }

    //toBinary(3);
    //toTernary(3);
    // std::cout << toBinary(5) << std::endl;
    // std::cout << toTernary(5) << std::endl;
    std::cout << "Enter number and it will be translate to other notation" << std::endl;
    std::cout << "Notations: Binary Ternary" << std::endl;
    std::cout << "Enter 0 to change notation (default - Binary)" << std::endl;
    bool flag = false;
    long x;
    while(1){
        std::cout << "Enter x" << std::endl;
        std::cin >> x;
        if (std::cin.fail()){
            std::cout <<"Incorrect value" << std::endl;
            return 1;
        }
        if (x == 0){
            flag = !flag;
            std::cout << "Changed notation" << std::endl;
            continue;
        } 
        if(!flag){
            char* res = toBinary(x);
            std::cout <<"Result: " << res << std::endl;
            delete[] res;
        }
        else {
            char* res = toTernary(x);
            std:: cout << "Result: " << res << std::endl;
            delete[] res;
        }
        
        
    }

    dlclose(lib);

}
