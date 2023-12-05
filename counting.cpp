#include <unordered_map>
#include <unistd.h>
#include <sstream>
#include <unordered_map>
 
#include "zmq_functions.h"
 
int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Wrong args for counting node");
    }
    int cur_id = std::atoi(argv[1]);
    int left_id = -1;
    int right_id = -1;

    zmq::context_t context;
    zmq::socket_t parent_socket(context, ZMQ_REP); 
    connect(parent_socket, cur_id);

    zmq::socket_t left_socket(context, ZMQ_REQ);
    zmq::socket_t right_socket(context, ZMQ_REQ);
    left_socket.setsockopt(ZMQ_SNDTIMEO, 5000);
    right_socket.setsockopt(ZMQ_SNDTIMEO, 5000);
    if (left_id != -1) {
        bind(left_socket, left_id);
    }
    if (right_id != -1) {
        bind(right_socket, right_id);
    }
 
    std::string message;
    while (true) {
        message = receive_message(parent_socket);
        std::istringstream request(message);
        int dest_id;
        request >> dest_id;

        std::string cmd;
        request >> cmd;

        if (dest_id == cur_id) {

            if (cmd == "pid") {
                send_message(parent_socket, "OK: " + std::to_string(getpid()));
            }

            else if (cmd == "create") {
                int new_child_id;
                request >> new_child_id;
                
                if (cur_id > new_child_id && left_id != -1){
                    send_message(left_socket, std::to_string(left_id) + "create " + std::to_string(new_child_id));
                    std::string msg = receive_message(left_socket);
                    if (msg == "OK") {
                        send_message(parent_socket, "OK");
                    }
                    continue;
                }

                else if (cur_id < new_child_id && right_id != -1){
                    send_message(right_socket, std::to_string(right_id) + "create " + std::to_string(new_child_id));
                    std::string msg = receive_message(right_socket);
                    if (msg == "OK") {
                        send_message(parent_socket, "OK");
                    }
                    continue;
                }
                int flag = 0;
                if (cur_id > new_child_id ){
                    left_id = new_child_id;
                    bind(left_socket, left_id);
                    flag = 1;
                }

                else if (cur_id < new_child_id){
                    right_id = new_child_id;
                    bind(right_socket, right_id);
                }
                
                pid_t pid = fork();
                if (pid < 0) {
                    perror("Can't create new process");
                    return -1;
                }
                if (pid == 0) {
                    execl("./counting", "./counting", std::to_string(new_child_id).c_str(),  NULL);
                    perror("Can't execute new process");
                    return -2;
                }
                if (flag){
                    send_message(left_socket, std::to_string(left_id) + "pid");
                    std::string res = receive_message(left_socket);
                    send_message(parent_socket, "OK");
                }
                else{
                    send_message(right_socket, std::to_string(right_id) + "pid");
                    std::string res = receive_message(right_socket);
                    send_message(parent_socket, "OK");
                }
                
            }
            else if (cmd == "exec") {
                int sum = 0;
                std::string number;
                while (request >> number) {
                    sum += std::stoi(number);
                }
                send_message(parent_socket, "OK: " + std::to_string(cur_id) + ": " + std::to_string(sum));
            }

            else if (cmd == "heartbeat") {
                std::string reply;
                if (left_id != -1) {
                    send_message(left_socket, std::to_string(left_id) + " heartbeat");
                    std::string msg = receive_message(left_socket);
                    reply += " " + msg;
                }
                if (right_id != -1) {
                    send_message(right_socket, std::to_string(right_id) + " heartbeat");
                    std::string msg = receive_message(right_socket);
                    reply += " " + msg;
                }
                send_message(parent_socket, std::to_string(cur_id) + reply);
            }
            else if (cmd == "kill") {
                if (left_id != -1) {
                    send_message(left_socket, std::to_string(left_id) + " kill");
                    std::string msg = receive_message(left_socket);
                    if (msg == "OK") {
                        send_message(parent_socket, "OK");
                    }
                  //unbind(left_socket, left_id);
                 //  disconnect(parent_socket, left_id);
                 //  disconnect(parent_socket,cur_id);

                    
                }
                if (right_id != -1) {
                    send_message(right_socket, std::to_string(right_id) + " kill");
                    std::string msg = receive_message(right_socket);
                    if (msg == "OK") {
                        send_message(parent_socket, "OK");
                    }
                       //unbind(right_socket, right_id);
                      // disconnect(parent_socket, right_id);
                      // disconnect(parent_socket, cur_id);
                   
                
                }
                send_message(parent_socket, "OK");
                unbind(parent_socket, cur_id);
                disconnect(parent_socket, cur_id);
               
                exit(0);
                
            }

        }
        else if (right_id != -1 && dest_id > cur_id) {
            send_message(right_socket, message);
            send_message(parent_socket, receive_message(right_socket));
            if (right_id == dest_id && cmd == "kill") {
                unbind(right_socket, right_id);
                right_id = -1;
            }
        }

        else if (left_id != -1 && dest_id < cur_id) {
            send_message(left_socket, message);
            send_message(parent_socket, receive_message(left_socket));
            if (left_id == dest_id && cmd == "kill") {
                unbind(left_socket, left_id);
                left_id = -1;
            }
        }
        else {
            send_message(parent_socket, "Error: node is unavailable [cont]");
        }
    }

   return 0;
}