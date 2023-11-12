#include <iostream>
#include "libProst.h"

int main(int argc, char* argv[]){
    if (argc != 4){
        std::cout << "Usage: func(1/2) leftborder rightborder" << std::endl;
    }
    else if (std::atoi(argv[1]) == 1){
        std::cout << "Simple Function. Result: " << simple(std::atoi(argv[2]) , std::atoi(argv[3])) << "\n";
    }
    else if (std::atoi(argv[1]) == 2){
        std::cout << "Eratosfen Function. Result: " << eratosfen(std::atoi(argv[2]) , std::atoi(argv[3])) << "\n";
    }
    else{
        std::cout << "Unknown command" << "\n";
    }
    
}