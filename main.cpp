#include <iostream>
#include "topology.h"


int main(){
    Node* root_network = createNode(1);
    insertNode(root_network, 2);
    insertNode(root_network, 5);
    insertNode(root_network, -1);
    
    insertNode(root_network, 6);
    insertNode(root_network, 7);
    insertNode(root_network, 8);
    insertNode(root_network, 3);
    inOrderTraversal(root_network);
    if(1){
        std::cout << "gay";
    }
}


