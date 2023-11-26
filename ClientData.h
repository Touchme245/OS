#pragma once
#include <iostream>

typedef struct ClientData
{
     char login[20];
     char  reciever[20];
     char  message[100];
    ClientData( char* login,  char * reciever,  char * message){
        
        int i = 0;
        while (i < 20 && login[i] != '\0'){
            this->login[i] = login[i];
            i++;
        }
        this->login[i] = '\0';
        i = 0;

        while (i < 100 && message[i] != '\0'){
            this->message[i] = message[i];
            i++;
        }
        this->message[i] = '\0';
        i = 0;
        while (i < 100 && reciever[i] != '\0'){
            this->reciever[i] = reciever[i];
            i++;
        }
        this->reciever[i] = '\0';
        
    }

    ClientData(const char* login, const char * reciever, const char * message){
        
        int i = 0;
        while (i < 20 && login[i] != '\0'){
            this->login[i] = login[i];
            i++;
        }
        this->login[i] = '\0';
        i = 0;

        while (i < 100 && message[i] != '\0'){
            this->message[i] = message[i];
            i++;
        }
        this->message[i] = '\0';
        i = 0;
        while (i < 20 && reciever[i] != '\0'){
            this->reciever[i] = reciever[i];
            i++;
        }
        this->reciever[i] = '\0';
        
    }

    void setLogin(char* login){
        int i = 0;
        while (i < 20 && login[i] != '\0'){
            this->login[i] = login[i];
            i++;
        }
        this->login[i] = '\0';
        
    }

    void setLogin(const char* login){
        int i = 0;
        while (i < 20 && login[i] != '\0'){
            this->login[i] = login[i];
            i++;
        }
        this->login[i] = '\0';
    }
        

    void setMessage(char* messsage){
        int i = 0;

        while (i < 100 && message[i] != '\0'){
            this->message[i] = message[i];
            i++;
        }
        this->message[i] = '\0';
    }

    void setMessage(const char* messsage){
        int i = 0;

        while (i < 100 && message[i] != '\0'){
            this->message[i] = message[i];
            i++;
        }
        this->message[i] = '\0';
    }

    void setReciever(char* reciever){
        int i = 0;

        while (i < 20 && reciever[i] != '\0'){
            this->reciever[i] = reciever[i];
            i++;
        }
        this->reciever[i] = '\0';
    }


    void setReciever(const char* reciever){
        int i = 0;

        while (i < 20 && reciever[i] != '\0'){
            this->reciever[i] = reciever[i];
            i++;
        }
        this->reciever[i] = '\0';
    }

    char * getLogin(){
        char login[20];
        int i = 0;
        while (i < 20 && this->login[i]!='\0'){
            login[i] = this->login[i];
            i++;
        }
        login[i] = '\0';


        return login;
    }

    char * getReciever(){
        char reciever[20];
        int i = 0;
        while (i < 20 && this->reciever[i]!='\0'){
            reciever[i] = this->reciever[i];
            i++;
        }
        reciever[i] = '\0';
        return reciever;
    }

    char * getMessage(){
        char message[100];
        int i = 0;
        while (i < 100 && this->message[i]!='\0'){
            message[i] = this->message[i];
            i++;
        }
        message[i] = '\0';
        return message;
    }


    

    ClientData(){
       // std::cout << "empty const\n"; 
    }
}ClientData;