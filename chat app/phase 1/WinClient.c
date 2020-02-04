#include <cJSON.h>
#include "cJSON.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 8000
#define PORT 8080
#define SA struct sockaddr
struct sockaddr_in servaddr;
void chat(int server_socket)
{
	char buffer[MAX];
	int n;
	char user[50],pass[50];
	while (true) {
            login_menu:{
        printf("\nentering menu:\n 1: register \n 2:login\n ***\please choose one option\***\n");
        int x;
        scanf("%d",&x);
        if(x==1) {
                printf("please enter your username\n");
                scanf("%s",user);
                printf("please enter your password\n");
                scanf("%s",pass);
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer,"register %s, %s\n",user,pass);
                server_socket = socket(AF_INET, SOCK_STREAM, 0);
                connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
               // printf("salam\n");
                send(server_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(server_socket, buffer, sizeof(buffer), 0);
                cJSON *rec= cJSON_Parse(buffer);
                cJSON *type = cJSON_GetObjectItemCaseSensitive(rec, "type");
                printf("welcome, your operation is \"%s\"\n", type->valuestring);
                closesocket(server_socket);
        }
        else if(x==2) {
                printf("please enter your username\n");
                scanf("%s",user);
                printf("please enter your password\n");
                scanf("%s",pass);
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer,"login %s, %s\n",user,pass);
                server_socket = socket(AF_INET, SOCK_STREAM, 0);
                connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                send(server_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(server_socket, buffer, sizeof(buffer), 0);
                cJSON *rec= cJSON_Parse(buffer);
                cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "content");
                printf("\nyour token is \"%s\" and you succesfuly loged in\n", content->valuestring);
                char token[100];
                strcpy(token,content->valuestring);
                closesocket(server_socket);
                while (1){
                    channels_menu:{
                    printf("\nchose what you want to do:\n 1:creat channel\n 2:join channel\n 3:log out\n 4: go to login menu\n");
                    int x123;
                    scanf("%d",&x123);
                    if(x123==1){
                        printf("\nplease enter the channel name:\n");
                        char channelname[100];
                        scanf("%s",channelname);
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer,"create channel %s, %s\n",channelname,token);
                        server_socket = socket(AF_INET, SOCK_STREAM, 0);
                        connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                        send(server_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        recv(server_socket, buffer, sizeof(buffer), 0);
                        cJSON *rec= cJSON_Parse(buffer);
                        cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "type");
                        printf("result is %s\n", content->valuestring);
                        closesocket(server_socket);
                    }
                    else if(x123==2){
                        printf("\n enter the channel name you want ton join\n");
                        char channelname[100];
                        scanf("%s",channelname);
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer,"join channel %s, %s\n",channelname,token);
                        server_socket = socket(AF_INET, SOCK_STREAM, 0);
                        connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                        send(server_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        recv(server_socket, buffer, sizeof(buffer), 0);
                        cJSON *rec= cJSON_Parse(buffer);
                        cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "type");
                        printf("\n result is %s\n", content->valuestring);
                        closesocket(server_socket);
                        while(1){
                                send_menu:
                                printf("\n you have 4 options here:\n 1:send massage\n 2: refresh\n 3: channel members\n 4: leave channel\n 5: go to channels menu \n 6: go to send menu\n");
                                int resp;
                                scanf("%d",&resp);
                                if(resp==1){
                                    char sendmas[100];
                                    scanf("%s",sendmas);
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"send %s, %s\n",sendmas,token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    cJSON *rec= cJSON_Parse(buffer);
                                    cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "type");
                                    printf("\n result is %s\n", content->valuestring);
                                    closesocket(server_socket);
                                }
                                else if(resp==2){
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"refresh %s\n",token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    //printf("%s",buffer);
                                    cJSON *rec= cJSON_Parse(buffer);
                                    cJSON *array= NULL;
                                    cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "content");
                                    cJSON_ArrayForEach(array, content)
                                    {
                                        cJSON *sender = cJSON_GetObjectItemCaseSensitive(array, "sender");
                                        cJSON *content = cJSON_GetObjectItemCaseSensitive(array, "message");
                                        printf("\n sender : %s \n content: %s \n ***",sender->valuestring,content->valuestring);
                                    }
                                    closesocket(server_socket);
                                }
                                else if(resp==3){
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"channel members %s\n",token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    cJSON *rec= cJSON_Parse(buffer);
                                    cJSON *array= NULL;
                                    cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "content");
                                    int i=1;
                                    cJSON_ArrayForEach(array, content)
                                    {
                                        printf("\n %d: %s\n",i,array->valuestring);i++;
                                    }
                                    closesocket(server_socket);
                                }
                                else if(resp==4){
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"leave %s\n",token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    cJSON *rec= cJSON_Parse(buffer);
                                    cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "type");
                                    printf("\n result is %s\n", content->valuestring);
                                    closesocket(server_socket);
                                    break;
                                }
                                else if(resp==5){
                                    goto channels_menu;
                                }
                        }
                    }
                    else if(x123==3){
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer,"logout %s\n",token);
                        server_socket = socket(AF_INET, SOCK_STREAM, 0);
                        connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                        send(server_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        recv(server_socket, buffer, sizeof(buffer), 0);
                        cJSON *rec= cJSON_Parse(buffer);
                        cJSON *content = cJSON_GetObjectItemCaseSensitive(rec, "type");
                        printf("\n result is %s\n", content->valuestring);
                        closesocket(server_socket);
                        break;
                    }
                    else if(x123==4){
                        goto login_menu;
                    }
                    else if(x123==5){
                        goto send_menu;
                    }
         }       }
        }
        else printf("\n input is not correct \n");
        }
	}
}

int main()
{
	int client_socket,
	 server_socket;
	struct sockaddr_in cli;
	WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }
	client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (client_socket == -1) {
		printf("Socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");*/
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);/*
	if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection to the server failed...\n");
		exit(0);
	}
	else
		printf("Successfully connected to the server..\n");*/
	chat(client_socket);
	closesocket(client_socket);
}
