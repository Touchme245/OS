#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <fcntl.h>
#include <sys/mman.h>
#include <fstream>
#include <condition_variable>
#include <sstream>
#include <ctime>
#include <pthread.h>
#include <mutex>
#include "ClientData.h"
#include <map>
#include <vector>
#include "Utils.h"
#include <pthread.h>
#include <semaphore.h>
#include <csignal>


sem_t* sem = sem_open("serverSemap", O_CREAT, 0644, 1);

void signalHandler(int signal){
    if (signal == SIGINT){
        sem_post(sem);
        sem_close(sem);
        sem_unlink("serverSemap");
        exit(signal);
    }
}

//user - msgs to him
std::map<const std::string,std::vector< std::string >> storage;
//chatname - users
std::map<const std::string,std::vector< std::string >> chats;

int main(){
    std::signal(SIGINT, signalHandler);
    if (sem_trywait(sem) == -1){
        std::cout << "Server has already been started, you cant run it now\n";
        return 0;
    }
    std::cout << "Server" << std::endl;
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
   
    ClientData* arr = (ClientData*)(shm_ptr);
    ClientData data{"System", "System", "Write"};
    ClientData show{"System", "System", "Show"};
    ClientData send{"System", "System", "Send"};
    
    arr[0] = data;
   

   
     while(1){
        // std::cout << arr[0].message << "\n";
        if (arr[0].message == "Exit"){
            break;
        }
        if (equals(arr[0].message, "Read")){
            
            char*  reciever = arr[1].reciever;
            
            char* message = arr[1].message;

            std::cout << message << "\n";
           
            char* login = arr[1].login;

            std::string sLogin(login);
            std::string sReciever(reciever);
            std::string sMessage(message);
            
            if (!chats[sReciever].empty()){
                for (auto elem : chats[sReciever]){
                    storage[elem].push_back(sReciever + "/" + sLogin + ": " + sMessage);
                }
            }
            storage[sReciever].push_back(sLogin + ": " + sMessage);
           // std::cout << storage[sLogin].size() << "\n";
           // std::cout << storage[sReciever].size() << "\n";
           // std::cout << sLogin << " " << sReciever << "\n";
         //   std::cout << "login: " << login << " reciever: " << reciever << " message: " << message << std::endl;
            if (!storage[sLogin].empty()){
                int i = 0;
                arr[0] = send;
                while(1){
                    if (equals(arr[0].message, "Send")){
                        std::cout << i << " " << storage[sLogin].size() << "\n";
                        if (i >= storage[sLogin].size()){
                            
                            
                            storage[sLogin].clear();
                            break;
                        }
                        std::string msg = storage[sLogin][i];
                        ClientData messageToClient{"System", "System", msg.c_str()};
                        arr[1] = messageToClient;
                        arr[0] = show;
                        i++;
                    }
                        
                        

                }
                
            
                
            }

            
            arr[0] = data;

        }
        if (equals(arr[0].message,"JoinChat")){
            char* chatname = arr[1].reciever;
            char* creator = arr[1].login;

            std::string sChatName(chatname);
            std::string sCreator(creator);
            chats[sChatName].push_back(sCreator);
            arr[0] = data;
        }
        
        
    }
    
    munmap(shm_ptr, size);
    close(shm_fd);
    sem_post(sem);
    sem_close(sem);
    sem_unlink("serverSemap");
    return 0;
    
    
}
