#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <fcntl.h>
#include <sstream>
#include <sys/mman.h>
#include "ClientData.h"

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

ClientData send{"System", "System", "Send"};
ClientData joinChat{"System", "System", "JoinChat"};

int main(){
    int size = sizeof(char) * 280;
    int shm_fd = shm_open("shared_memory", O_CREAT | O_RDWR, 0777);
    if (shm_fd == -1){
        std::cout << "Error with fd\n";
        return 1;
    }
    if (ftruncate(shm_fd, size) == -1){
        std::cout << "error with trunc" << std::endl;
        return 1;
    }
    void* shm_ptr = mmap(nullptr , size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (shm_ptr == MAP_FAILED) {
        std::cout << "Error mapping shared memory" << std::endl;
        munmap(shm_ptr, size);
        close(shm_fd);
        return 1;
    }
    
    ClientData* arr = (ClientData*)shm_ptr;

   
    // std::cout << arr[0].message << "\n";
    std::cout << "Введите логин" << std::endl;
    std::string login;
    std::cin >> login;
    std::cout << "Доступные команды:" << std::endl;
    std::cout << "1 - Отправить сообщение" <<std::endl;
    std::cout << "2 - Вступить в  чат" <<std::endl;
    std::cout << "3 - Выйти" <<std::endl;

    while(1){
        
        if (equals(arr[0].message, "Write")){
            std::cout << "Введите команду: ";
            int command;
            std::cin >> command;

            if (command == 3){
                break;
            }

            if (command == 1){
                std::string reciever;
                std::string message;
                std::cout << "Введите получателя сообщения\n";
                std::cin >> reciever;
                std::cout << "Введите сообщение" << std::endl;
                std::cin.clear();
                std::getline(std::cin, message);
                std::getline(std::cin, message);
                // std::cout << "was red" << " " << message << "\n";
                // std::cin.clear();
                //std::cin >> message;
                ClientData clientMessage{login.c_str(), reciever.c_str(), message.c_str()};
                arr[1] = clientMessage;
                
                ClientData data{"System", "System", "Read"};
                arr[0] = data;


            }

            if (command == 2){
                std::cout << "Введите имя чата, если его нет то он будет создан\n";
                std::string chatName;
                std::cin >> chatName;

                
                // char* newMessage = arr[1].message;
                // std::cout << "У вас новое сообщение: " << newMessage << std::endl;
                ClientData chatData{login.c_str(), chatName.c_str(), "JoinChat"};
                arr[1] = chatData;
                arr[0] = joinChat;
            }


        }
        if (equals(arr[0].message, "Show")){
           char* newMessage = arr[1].message;
           std::cout << "У вас новое сообщение от " << newMessage << std::endl;
           arr[0] = send;

        }

        // if (equals(arr[0].message, "JoinChat")){
            

        // }

    }
    munmap(shm_ptr, size);
    close(shm_fd);
    

}