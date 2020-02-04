#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dir.h>
#include <process.h>
#include <winsock2.h>
#include "cJSON.h"
#include <cJSON.h>
#define MAX 8000
#define PORT 8080
#define SA struct sockaddr

int server_socket, client_socket,onpeop=0;// onpeop is number of online people
struct sockaddr_in server, client;
char address[50];
// Function designed for chat between client and server.

struct detail{
    char user[100],token[100],channel[100],channel_content[1000];
};
struct detail online_people[1000];
void remove_last_char(char string[]){
    const int mysize=strlen(string);
    string[mysize-1]='\0';
}
int token_check(char token[]){

    for(int i=0;i<=onpeop;i++){
        if(strcmp(online_people[i].token,token)==0){
            return i;
        }
    }
    return -1;
}
void acc(){

    int len = sizeof(client);
    client_socket = accept(server_socket, (SA *)&client, &len);
    if (client_socket < 0)
    {
        printf("Server accceptance failed...\n");
        exit(0);
    }
    else
        printf("+Server acccepted the client..\n");


}
int main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
	// Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        // Tell the user that we could not find a usable Winsock DLL.
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
    // Create and verify socket
    /*
    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");*/
    // Assign IP and port
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(PORT);
    // Bind newly created socket to given IP and verify
    /*
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
    printf("Socket successfully bound..\n");
    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
*/
    ///begening of main code

    char first[30],user[50],pass[50],buffer[MAX];
    memset(buffer, 0, sizeof(buffer));
    // Accept the data packet from client and verify
    mkdir("resources");
    mkdir("resources\\channels");
    mkdir("resources\\users");
    mkdir("resources\\refresh");
    for(int i=0;i<1000;i++){
       strcpy(online_people[i].user,"\0");
       strcpy(online_people[i].token,"\0");
       strcpy(online_people[i].channel,"\0");
       strcpy(online_people[i].channel_content,"\0");
    }
    while(1){
        memset(buffer, 0, sizeof(buffer));

    server_socket = socket(AF_INET, SOCK_STREAM, 6);
	if (server_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError() );
    else
        printf("Socket successfully created..\n");
    if ((bind(server_socket, (SA *)&server, sizeof(server))) != 0)
    {
        printf("Socket binding failed...\n");
        exit(0);
    }
    else
    printf("Socket successfully bound..\n");
    // Now server is ready to listen and verify
    if ((listen(server_socket, 5)) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
       acc();
       recv(client_socket,buffer,sizeof(buffer),0);
       sscanf(buffer,"%s %*s",first);
      // printf("%s\n",first);
       if(strcmp(first,"register")==0){

            sscanf(buffer,"register %s %s",user,pass);
            //printf("%s\n%s",name,pass);
            remove_last_char(user);
            FILE *fuser;
            sprintf(address,"resources\\users\\%s.user.json",user);
            if ((fuser=fopen(address,"r"))==NULL){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                fclose(fuser);
                char output[1000];
                cJSON * write_to_txt;
                write_to_txt = cJSON_CreateObject();
                cJSON_AddItemToObject(write_to_txt,"username",cJSON_CreateString(user));
                cJSON_AddItemToObject(write_to_txt,"password",cJSON_CreateString(pass));
                strcpy(output,cJSON_PrintUnformatted(write_to_txt));
                fuser=fopen(address,"w");
                fprintf(fuser,"%s",output);
                fclose(fuser);
            }
            else{
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("The username was taken"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }

            //fopen("recources\\users.")
       }
       else if(strcmp(first,"login")==0){

            char json[1000];
            sscanf(buffer,"login %s %s",user,pass);
            remove_last_char(user);
            sprintf(address,"resources\\users\\%s.user.json",user);
            FILE * fuser;
            if ((fuser=fopen(address,"r"))==NULL){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("user with this username doesnt exist!"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }
            else{
                fgets(json,1000,fuser);
                fclose(fuser);
                cJSON * rec = cJSON_Parse(json);
                if (strcmp(cJSON_GetObjectItem(rec,"password")->valuestring,pass)==0){
                    char random_token[31];
                    srand((unsigned)time(NULL));
                    for(int i=0;i<30;i++){
                        int s=(rand()%26)+97;
                        random_token[i]=s;
                    }
                    random_token[30]='\0';
                    // random_token created
                    char result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString(random_token));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);

                    strcpy(online_people[onpeop].user,user);
                    strcpy(online_people[onpeop].token,random_token);
                    onpeop++;
                }
                else {
                    char result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString("password is incorrect"));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);
                }
            }
       }
       else if(strcmp(first,"create")==0){
            char token[100],channel_name[100];
            sscanf(buffer,"create channel %s %s",channel_name,token);
            remove_last_char(channel_name);
            memset(address, 0, sizeof(address));
            sprintf(address,"resources\\channels\\%s.channel.json",channel_name);
            FILE * fchannel;
            if (token_check(token)==-1){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("token is incorrect"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                fclose(fchannel);
            }
            else if(   (fchannel=fopen(address,"r"))!=NULL ){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("channel exists"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                fclose(fchannel);

            }
            else {
                fclose(fchannel);
                int n=token_check(token);
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                strcpy(online_people[n].channel,channel_name);
                char output[1000];
                cJSON *chap, *content_array, *content;
                chap = cJSON_CreateObject();
                content_array = cJSON_CreateArray();
                cJSON_AddItemToObject(chap, "type", cJSON_CreateString("List"));
                cJSON_AddItemToObject(chap, "content", content_array);

                char message[100];
                sprintf(message,"channel created by %s",online_people[n].user);
                cJSON_AddItemToArray(content_array, content = cJSON_CreateObject());
                cJSON_AddItemToObject(content, "sender", cJSON_CreateString("server"));
                cJSON_AddItemToObject(content, "message", cJSON_CreateString(message));

                strcpy(output,cJSON_PrintUnformatted(chap));
                //strcpy(online_people[n].channel_content,output);
                FILE * fch;
                fch=fopen(address,"w");
                fprintf(fch,"%s",output);
                fclose(fch);

            }


       }
       else if(strcmp(first,"join")==0){
            FILE * fchannel;
            char token[100],channel_name[100];
            sscanf(buffer,"join channel %s %s",channel_name,token);
            remove_last_char(channel_name);
            memset(address, 0, sizeof(address));
            sprintf(address,"resources\\channels\\%s.channel.json",channel_name);
            int user_token_number=token_check(token);
            if (user_token_number==-1){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("token is incorrect"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }
            else if(   (fchannel=fopen(address,"r"))==NULL ){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("channel doesnt exist"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                fclose(fchannel);
            }
            else {
                fclose(fchannel);
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                char message[100];
                sprintf(message,"%s joined the channel",online_people[user_token_number].user);
                FILE * fch;
                cJSON * file_content, * content, * content_message;
                char file_json[1000];
                fch=fopen(address,"r");
                fgets(file_json,1000,fch);
                file_content=cJSON_Parse(file_json);
                fclose(fch);
                content=cJSON_GetObjectItem(file_content,"content");
                cJSON_AddItemToArray(content, content_message = cJSON_CreateObject());
                cJSON_AddItemToObject(content_message, "sender", cJSON_CreateString("server"));
                cJSON_AddItemToObject(content_message, "message", cJSON_CreateString(message));

                char output[1000];
                strcpy(output,cJSON_PrintUnformatted(file_content));
                fch=fopen(address,"w");
                strcpy(online_people[user_token_number].channel,channel_name);
              //  strcpy(online_people[user_token_number].channel_content,output);
                fprintf(fch,"%s",output);
                fclose(fch);

            }

       }
       else if(strcmp(first,"send")==0){
            char token[100],message[1000];
            sscanf(buffer,"send %s %s",message,token);
            remove_last_char(message);
            //scan(message,client_response,token);
            //token[20]='\0';
            int user_token_number=token_check(token);
            if(user_token_number!=-1){
                    if(strcmp(online_people[user_token_number].channel,"\0")==0){
                    char result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString("user doesnt have a channel"));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);
                    }
                    else{
                    memset(address, 0, sizeof(address));
                    sprintf(address,"resources\\channels\\%s.channel.json",online_people[user_token_number].channel);
                    char result[1000],json[1000];
                    FILE * fchannel;
                    cJSON * file_content, * content, * last_content;
                    fchannel=fopen(address,"r");
                    fgets(json,1000,fchannel);
                    file_content=cJSON_Parse(json);
                    fclose(fchannel);

                    content=cJSON_GetObjectItem(file_content,"content");
                    cJSON_AddItemToArray(content, last_content = cJSON_CreateObject());
                    cJSON_AddItemToObject(last_content, "sender", cJSON_CreateString(online_people[user_token_number].user));
                    cJSON_AddItemToObject(last_content, "message", cJSON_CreateString(message));

                    char output[1000];
                    strcpy(output,cJSON_PrintUnformatted(file_content));
                    fchannel=fopen(address,"w");
                    fprintf(fchannel,"%s",output);
                    fclose(fchannel);
                    char temp_result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
                    strcpy(temp_result,cJSON_PrintUnformatted(root));
                    send(client_socket, temp_result, sizeof(temp_result), 0);

                    }
            }

            else{
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("token is incorrect"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }

       }
       else if(strcmp(first,"refresh")==0){

            char token[100],channel_name[1000];
            sscanf(buffer,"refresh %s",token);
            int user_token_number=token_check(token);
            if(user_token_number!=-1){
                if(strcmp(online_people[user_token_number].channel,"\0")!=0){
                    char result[1000],name[1000],adr[1000];
                    strcpy(channel_name,online_people[user_token_number].channel);
                    int seen=0;
                    sprintf(adr,"resources\\refresh\\%s",channel_name);
                    mkdir(adr);
                    sprintf(adr,"resources\\channels\\%s.channel.json",channel_name);
                    FILE * fch;
                    fch=fopen(adr,"r");
                    fgets(result,1000,fch);
                    fclose(fch);
                    FILE * FP;
                    char s_adr[1000];
                    sprintf(adr,"resources\\refresh\\%s",channel_name);
                    sprintf(s_adr,"%s\\%s.txt",adr,online_people[user_token_number].user);
                    FP=fopen(s_adr,"r");
                    //the case of refreshing for the first time

                    if(FP==NULL){
                        send(client_socket, result, sizeof(result), 0);

                        cJSON * rec = cJSON_Parse(result);
                        cJSON * array = cJSON_GetObjectItem(rec,"content");
                        int length=cJSON_GetArraySize(array);

                        FILE * FF;
                        FF=fopen(s_adr,"w");
                        fprintf(FF,"%d",length);
                        printf("the length %d",length);
                        fclose(FF);
                        printf("resid");
                    }

                    //the case that they have already refreshed the channel
                    else{

                        //making the cJSON that we'll send to our client
                        char output[1000];
                        cJSON *chap, *content, *car;
                        chap = cJSON_CreateObject();
                        content = cJSON_CreateArray();
                        cJSON_AddItemToObject(chap, "type", cJSON_CreateString(""));
                        cJSON_AddItemToObject(chap, "content", content);

                        //receiving the number of messages that they've already seen0
                        fscanf(FP,"%d",&seen);

                        cJSON * rec = cJSON_Parse(result);

                        cJSON * array = cJSON_GetObjectItem(rec,"content");

                        int length=cJSON_GetArraySize(array);
                        printf("the length is %d, however the seen is %d\n",length,seen);

                        //start from the message with index seen

                        for(int i=seen;i<length;i++){
                            cJSON * array1 = cJSON_GetArrayItem(array,i);
                            char sender[100],sms[1000];
                            strcpy(sender,cJSON_GetObjectItem(array1,"sender")->valuestring);
                            strcpy(sms,cJSON_GetObjectItem(array1,"message")->valuestring);

                            //adding a message to the cJSON

                            cJSON_AddItemToArray(content, car = cJSON_CreateObject());
                            cJSON_AddItemToObject(car, "sender", cJSON_CreateString(sender));
                            cJSON_AddItemToObject(car, "message", cJSON_CreateString(sms));
                        }
                        strcpy(output,cJSON_PrintUnformatted(chap));
                        send(client_socket, output, sizeof(output), 0);
                        fclose(FP);
                        FP=fopen(s_adr,"w");
                        fprintf(FP,"%d",length);
                        fclose(FP);

                    }
                }
                else{
                    char result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString("user has no channel"));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);

                }
            }
            else{
                char result[100];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("token is incorrect"));
                strcpy(result, cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }

        }


       else if(strcmp(first,"channel")==0){
            char token[100];
            sscanf(buffer,"channel members %s",token);
            int user_token_number=token_check(token);
            if(user_token_number!=-1){

                if (strcmp(online_people[user_token_number].channel,"\0")==0){
                    char result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString("user has no channel"));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);
                }
                else{
                    char result[1000];
                    cJSON *root, *channel_members;
                    root = cJSON_CreateObject();
                    channel_members = cJSON_CreateArray();
                    cJSON_AddItemToObject(root, "type", cJSON_CreateString("list"));
                    cJSON_AddItemToObject(root, "content", channel_members);
                    for(int i=0;i<onpeop;i++){
                        if (strcmp(online_people[i].channel,online_people[user_token_number].channel)==0){
                            cJSON_AddItemToObject(channel_members, "content", cJSON_CreateString(online_people[i].user));
                        }
                    }
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);

                }
            }
            else{
                char result[100];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("token is incorrect"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }

       }
       else if(strcmp(first,"leave")==0){
            char token[100];
            sscanf(buffer,"leave %s",token);
            int user_token_number=token_check(token);
            if(user_token_number!=-1){
                if(strcmp(online_people[user_token_number].channel,"\0")==0){
                    char result[100];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString("the user in no channel"));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);
                }
                else{
                    char result[1000];
                    cJSON * root;
                    root = cJSON_CreateObject();
                    cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                    cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
                    strcpy(result,cJSON_PrintUnformatted(root));
                    send(client_socket, result, sizeof(result), 0);
                    char channel_name[100];
                    strcpy(channel_name,online_people[user_token_number].channel);
                    char message[100];
                    memset(address,0,sizeof(address));
                    sprintf(address,"resources\\channels\\%s.channel.json",channel_name);
                    sprintf(message,"%s left the channel",online_people[user_token_number].user);
                    FILE * fchannel;
                    cJSON * file_content, * content, * last_content;
                    char json[1000];
                    fchannel=fopen(address,"r");
                    fgets(json,1000,fchannel);
                    file_content=cJSON_Parse(json);
                    fclose(fchannel);
                    content=cJSON_GetObjectItem(file_content,"content");
                    cJSON_AddItemToArray(content, last_content = cJSON_CreateObject());
                    cJSON_AddItemToObject(last_content, "sender", cJSON_CreateString("server"));
                    cJSON_AddItemToObject(last_content, "message", cJSON_CreateString(message));

                    char output[1000];
                    strcpy(output,cJSON_PrintUnformatted(file_content));
                    fchannel=fopen(address,"w");
                    fprintf(fchannel,"%s",output);
                    fclose(fchannel);
                    strcpy(online_people[user_token_number].channel,"\0");
                }
            }
            else{
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("the token is wrong"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }
       }
       else if(strcmp(first,"logout")==0){
            char token[100];
            sscanf(buffer,"logout %s",token);
            int user_token_number=token_check(token);
            if(user_token_number!=-1){
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Successful"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString(""));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
                for(int i=user_token_number;i<onpeop-1;i++){
                    strcpy(online_people[i].user,online_people[i+1].user);
                    strcpy(online_people[i].channel,online_people[i+1].channel);
                    strcpy(online_people[i].token,online_people[i+1].token);
                    strcpy(online_people[i].channel_content,online_people[i+1].channel_content);
                }
                onpeop--;
            }
            else{
                char result[1000];
                cJSON * root;
                root = cJSON_CreateObject();
                cJSON_AddItemToObject(root,"type",cJSON_CreateString("Error"));
                cJSON_AddItemToObject(root,"content",cJSON_CreateString("token is incorrect"));
                strcpy(result,cJSON_PrintUnformatted(root));
                send(client_socket, result, sizeof(result), 0);
            }
       }
       closesocket(server_socket);
    }
}
