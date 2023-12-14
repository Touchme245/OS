#include <iostream>
#include <sstream>
#include <set>
#include "zmq_functions.h"
#include "topology.h"


int main(){
    
    zmq::context_t context; 
    int parent_id;

    // создание мастер ноды
    std::string firstCommand;
    std::cin >> firstCommand;
    std::cin >> parent_id;
    Node* root_network = createNode(parent_id);
    root_network->data = parent_id;
    pid_t pid = fork();
        if (pid < 0) {
            perror("Can't create new process");
            return -1;
        }
        if (pid == 0) {
            if (execl("./counting", "./counting", std::to_string(parent_id).c_str(), NULL) < 0) {
                perror("Can't execute new process");
                return -2;
            }
        }
        else{
            zmq::socket_t branch(context, ZMQ_REQ); 
        branch.setsockopt(ZMQ_SNDTIMEO, 5000);
        bind(branch, parent_id);
        send_message(branch, std::to_string(parent_id) + "pid");
                
        std::string reply = receive_message(branch);
        std::cout << reply << std::endl;
       


            std::string cmd;
            while (std::cin >> cmd) {
            if (cmd == "create") {
                    int node_id;
                    std::cin >> node_id;

                    if (find(root_network,node_id) != -1) {
                        std::cout << "Error: already exists" << std::endl;
                    }

                    else {
                        
                        send_message(branch, std::to_string(parent_id) + "create " + std::to_string(node_id));
                        

                        std::string t = receive_message(branch);
                        //std::cout << reply << " ";
                        send_message(branch, std::to_string(node_id) + "pid ");
                        std::string reply = receive_message(branch);
                        std::cout << reply << std::endl;
                        insertNode(root_network, node_id);
                    }
                }
                else if (cmd == "exec") {
                    int dest_id;
                    std::string numbers;
                    std::cin >> dest_id;
                    std::getline(std::cin, numbers);


                    send_message(branch, std::to_string(dest_id) + "exec " + numbers);
                    std::string reply = receive_message(branch);
                    std::cout << reply << std::endl;
                    
                }

                else if (cmd == "kill") {
                    int id;
                    std::cin >> id;
                    int t = find(root_network, id);
                    if (t == -1) {
                        std::cout << "ERROR: incorrect node id" << std::endl;
                    }
                    else {
                        send_message(branch, std::to_string(id) + " kill");
                        
                        std::string reply = receive_message(branch);
                        std::cout << reply << std::endl;
                        deleteNode(root_network,id);
                        if (id == parent_id) {
                            unbind(branch, id);
                            exit(0);
                        }
                    }
                }

                else if (cmd == "heartbeat") {
                    std::set<int> available_nodes;
                    int milis;
                    std::cin >> milis;
                        if (milis < 0){
                            milis *= -1;
                            
                        }
                        branch.setsockopt(ZMQ_RCVTIMEO, 4 * milis);
                        send_message(branch, std::to_string(parent_id) + " heartbeat");
                    
                        std::string received_message = receive_message(branch);
                        std::istringstream reply(received_message);
                        
                        int node;
                        while(reply >> node) {
                            available_nodes.insert(node);
                        }
                    
                    std::cout << "OK: ";
                    if (available_nodes.empty()) {
                        std::cout << "No available nodes" << std::endl;
                    }
                    else {
                        for (auto v : available_nodes) {
                            std::cout << v << " ";
                        }
                        std::cout << std::endl;
                    }
                    branch.setsockopt(ZMQ_SNDTIMEO, 5000);
                }

                else if (cmd == "exit") {
                
                    
                    send_message(branch, std::to_string(parent_id) + "kill");
                        
                    std::string reply = receive_message(branch);
                    if (reply != "OK") {
                        std::cout << reply << std::endl;
                    }
                    else {
                        unbind(branch, parent_id);
                    }
                    
                    exit(0);
                }
                else {
                    std::cout << "Incorrect cmd" << std::endl;
                }
        
   
   
    }
}

        

}