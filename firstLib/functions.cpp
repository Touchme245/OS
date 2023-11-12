#include "libProst.h"
#include <vector>


int eratosfen(int a, int b){
    int res = 0;
    std::vector<bool> isPrime(b+1, true);

    for (int p = 2; p* p<= b; ++p){
        if (isPrime[p]){
            for (int i = p * p; i <= b; i+=p){
                isPrime[i] = false;
            }
        }
    }


    for (int i = a; i <= b; ++i){
        if (isPrime[i]){
            res +=1;
        }
    }

    return res;
}


int simple(int a, int b){
    int res = 0;
    for (int i = a; i <= b; ++i){
        bool flag = 1;
        for (int d = 2; d < i; ++d){
            if (i ==2){
                break;
            }
            if (i % d == 0){
                flag = 0;
                break;
            }
        }
        if (flag){
            res+=1;
        }

    }
    return res;
}