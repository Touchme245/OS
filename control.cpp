#include <iostream>
#include <sstream>
#include <set>
#include "zmq_functions.h"
#include "topology.h"


int main(){
    Node* root_network = createNode(1);
    root_network->data = 1;
    std::vector<zmq::socket_t> branches;
    zmq::context_t context; 
    int parent_id = 1;
    // создание мастер ноды
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

        branches.emplace_back(context, ZMQ_REQ); 
        branches[branches.size() - 1].setsockopt(ZMQ_SNDTIMEO, 5000);
        bind(branches[branches.size() - 1], parent_id);
        send_message(branches[branches.size() - 1], std::to_string(parent_id) + "pid");
                
        std::string reply = receive_message(branches[branches.size() - 1]);
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
                
                send_message(branches[0], std::to_string(parent_id) + "create " + std::to_string(node_id));

                std::string reply = receive_message(branches[0]);
                std::cout << reply << std::endl;
                insertNode(root_network, node_id);
            }
        }
        else if (cmd == "exec") {
            int dest_id;
            std::string numbers;
            std::cin >> dest_id;
            std::getline(std::cin, numbers);


            send_message(branches[0], std::to_string(dest_id) + "exec " + numbers);
            std::string reply = receive_message(branches[0]);
            std::cout << reply << std::endl;
            
        }

        else if (cmd == "kill") {
            int id;
            std::cin >> id;
            //inOrderTraversal(root_network);
            //std::cout << "\n" << find(root_network, id) << " " << id << "\n";
            int t = find(root_network, id);
            if (t == -1) {
                std::cout << "ERROR: incorrect node id" << std::endl;
            }
            else {
                send_message(branches[0], std::to_string(id) + "kill");
                
                std::string reply = receive_message(branches[0]);
                std::cout << reply << std::endl;
                // network.erase(id);
                if (parent_id == 1) {
                    unbind(branches[0], id);
                    exit(0);
                }
            }
        }

        else if (cmd == "heartbeat") {
            std::set<int> available_nodes;
            
                
                send_message(branches[0], std::to_string(parent_id) + " heartbeat");
                
                std::string received_message = receive_message(branches[0]);
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
        }

        else if (cmd == "exit") {
           
            int first_node_id = 1;
            send_message(branches[0], std::to_string(first_node_id) + "kill");
                
            std::string reply = receive_message(branches[0]);
            if (reply != "OK") {
                std::cout << reply << std::endl;
            }
            else {
                unbind(branches[0], first_node_id);
            }
            
            exit(0);
        }
        else {
            std::cout << "Incorrect cmd" << std::endl;
        }
   
   
   
    }












}