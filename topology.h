#include <iostream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <fcntl.h>

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;

};

int find(Node * root, int id){
    if (root == NULL){
        return -1;
    }
    if (root->data == id){
        return id;
    }
    else if(id < root->data){
        return find(root->left, id);
    }
    else {
        return find(root->right, id);
    }
}


Node* createNode(int data) {
    Node* newNode = new Node();
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    return newNode;
}


int getHeight(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}


int getBalance(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}


void updateHeight(Node* node) {
    int leftHeight = getHeight(node->left);
    int rightHeight = getHeight(node->right);
    node->height = std::max(leftHeight, rightHeight) + 1;
}


Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}


Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}


void insertNode(Node* root, int id){
    if (root == NULL){
        root = createNode(id);
    }
    else if (root->data > id){
        if (root->left == NULL){
            root->left = createNode(id);
            return;
        }
        insertNode(root->left, id);
    }
    else if (root->data < id){
        if (root->right == NULL){
            root->right = createNode(id);
            return;
        }
        insertNode(root->right, id);
    }
}
// Node* insertNode(Node* root, int data) {
//     if (root == nullptr) {
        
//         return createNode(data);
//     }

//     if (data < root->data) {
//         root->left = insertNode(root->left, data);
//     } else if (data > root->data) {
//         root->right = insertNode(root->right, data);
//     } else {
//         return root; // Игнорируем дубликаты
//     }

    // updateHeight(root);

    // int balance = getBalance(root);

    // // Проверяем баланс и выполняем необходимые повороты
    // if (balance > 1 && data < root->left->data) {
    //     return rotateRight(root);
    // }

    // if (balance < -1 && data > root->right->data) {
    //     return rotateLeft(root);
    // }

    // if (balance > 1 && data > root->left->data) {
    //     root->left = rotateLeft(root->left);
    //     return rotateRight(root);
    // }

    // if (balance < -1 && data < root->right->data) {
    //     root->right = rotateRight(root->right);
    //     return rotateLeft(root);
    // }

    // return root;
//}


void inOrderTraversal(Node* root) {
    if (root != nullptr) {
        inOrderTraversal(root->left);
        std::cout << root->data << " ";
        inOrderTraversal(root->right);
    }
}

// Node* replacingNode(Node* node) {
//     Node* current = node;
//     while (current && current->left != NULL) {
//         current = current->left;
//     }
//     return current;
// }


// Node* deleteNode(Node* root, int data) {
//     if (root == NULL) {
//         return root;
//     }
//     if (data < root->data) {
//         root->left = deleteNode(root->left, data);
//     }
//     else if (data > root->data) {
//         root->right = deleteNode(root->right, data);
//     }
//     else {
//         if (root->left == NULL) {
//             Node* temp = root->right;
//             delete root;
//             return temp;
//         }
//         else if (root->right == NULL) {
//             Node* temp = root->left;
//             delete root;
//             return temp;
//         }
//         Node* temp = replacingNode(root->right);
//         root->data = temp->data;
//         root->right = deleteNode(root->right, temp->data);
//     }
//     return root;
// }

void deleteNode(Node* root, int data){
    if (root == NULL){
        return;
    }
    if (root->left != NULL){
        if (root->left->data == data){
            root->left = NULL;
            return;
        }
    }
    if (root->right != NULL){
        if (root->right->data == data){
            root->right = NULL;
            return;
        }
    }
    
    deleteNode(root->left,data);
    deleteNode(root->right, data);

}


