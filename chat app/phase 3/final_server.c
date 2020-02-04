

#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <dir.h>
#include <process.h>
#include <winsock2.h>
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

void json_get_string(char main[],char s[]){

    int number;
    for(int i=1;;i++){
        if (main[i]==',' && (  (main[i-1]==34 && main[i+1]==34)|| (main[i+1]=='{' && main[i-1]=='}')  )  ){
            number=i;
            break;
        }
    }
    for(int i=number+1;i<strlen(main)-1;i++){
        s[i-number-1]=main[i];
    }
    s[strlen(main)-1-number-1]='\0';

}

void remove_firstANDlast_char(char s[]){
    int size=strlen(s);
    for(int i=0;i<size;i++){
        s[i]=s[i+1];
    }
    s[size-2]='\0';
}

void mystringStrtor(char main[],char s[]){
    int number,size=strlen(main);

    for(int i=0;;i++){

        if (main[i+1]==',' && main[i]==34){
            number=i+1;
            break;
        }
        if (i>size){
            strcpy(s,main);
            return;
        }
    }

    for(int i=0;i<number;i++){
        s[i]=main[i];
    }

    s[number]='\0';

    for(int i=0;i<strlen(main);i++){
        main[i]=main[i+number+1];
    }

    main[size-number-1]='\0';

}

void mystringStrtore2(char main[],char s[]){
    int number,size=strlen(main);
    remove_firstANDlast_char(main);
    for(int i=0;;i++){

        if (main[i+1]==',' && main[i]==34){
            number=i+1;
            break;
        }
        if (i>size){
            strcpy(s,main);
            return;
        }
    }

    for(int i=0;i<number;i++){
        s[i]=main[i];
    }

    s[number]='\0';

    for(int i=0;i<strlen(main);i++){
        main[i]=main[i+number+1];
    }

    main[size-number-1]='\0';

}

int part_number (char main[]){
    int number=1;
    for(int i=1;i<strlen(main)-1;i++){
        if(main[i]==',' && main[i-1]==34 && main[i+1]==34){
            number++;
        }
    }
    return number;

}

int iseaqual(char main[],char s[]){

    int size=strlen(s);

    for(int i=1;i<size+1;i++){
        if(main[i]!=s[i-1]){
            return 0;
        }
    }

    return 1;

}

void my_json_getobject(char main[],char first[],char second[]){

    int size=strlen(first);

    for(int i=size+4;i<strlen(main);i++){
        second[i-size-4]=main[i];
    }

    second[strlen(main)-size-5]='\0';

}

void my_objcet_getItem(char main[],char first[],char second[]){
    remove_firstANDlast_char(main);
    char s[100];
    mystringStrtor(main,s);
    int i=0;
    while(i<=part_number(main)){
        i++;
        if(iseaqual(s,first)==1){
            my_json_getobject(s,first,second);

        }
            mystringStrtor(main,s);
    }

}

int ArrayQuote_number(char main[]){

    if(main[0]==0){
        return 0;
    }

    int number=1;

    for (int i=1;i<strlen(main)-1;i++){
        if ( (main[i]==',' && main[i-1]==34 && main[i+1]==34) ){
            number++;
        }
    }

    return number;
}

int ArrayBruket_number(char main[]){

    if(main[0]==0){
        return 0;
    }

    int number=1;

    for (int i=1;i<strlen(main)-1;i++){
        if ( (main[i]==',' && main[i-1]=='}' && main[i+1]=='{') ){
            number++;
        }
    }

    return number;
}

void ArrayObjectQuote (char main[],char s[],int x){

    int first, second;
    ArrayIndexQuote(main,x,&first,&second);
    for(int i=first+1;i<second;i++){
        s[i-first-1]=main[i];

    }

    s[second-2-first]='\0';

}

void ArrayObjectBracket (char main[],char s[],int x){

    int first, second;
    ArrayIndexBracket(main,x,&first,&second);

    for(int i=first+1;i<second;i++){
        s[i-first-1]=main[i];

    }

    s[second-2-first]='\0';

}

void ArrayIndexQuote(char main[],int x,int * first,int * second){

    int number=0;
    bool b1=true;
    bool b2=true;

    if(x==1){
        b1=false;
        *first=0;
    }


    if(ArrayQuote_number(main)==1){
        b2=false;
        *second=strlen(main)-1;
    }

    for(int i=1;i<strlen(main)-1;i++){

        if ((main[i]==',' && main[i-1]==34 && main[i+1]==34)){
            number++;
        }

        if(x-1==number && b1){
            *first=i;
            b1=false;
        }

        if(ArrayQuote_number(main)!=x){
            if(x==number && b2){
                *second=i;
                b2=false;
            }
        }
        else{
            *second=strlen(main);
        }

    }

}

void ArrayIndexBracket(char main[],int x,int * first,int * second){
    int number=0;
    bool b1=true;
    bool b2=true;

    if(x==1){
        b1=false;
        *first=-1;
    }

    if(ArrayBruket_number(main)==1){
        b2=false;
        *second=strlen(main);
    }

    for(int i=1;i<strlen(main)-1;i++){

        if ( (main[i]==',' && main[i-1]=='}' && main[i+1]=='{')){
            number++;
        }

        if(x-1==number && b1){
            *first=i;
            b1=false;
        }

        if(ArrayBruket_number(main)!=x){
            if(x==number && b2){
                *second=i;
                b2=false;
            }
        }
        else{
            *second=strlen(main);
        }

    }

}
/*
struct node{
    char type[100];
    char typeValue[100];
    char content[1000];
    char contentValue[1000];
};
*/
void text_builder(char output[],char type[],char typevalue[],char content[],char contentvalue[] ){

    sprintf(output,"{\"%s\":\"%s\",\"%s\":\"%s\"}",type,typevalue,content,contentvalue);

}

//a function that we user for receiving message
//and token for <<send>> function

void AddItemToArray(char array[],char item[]){
    int size=strlen(array);
    array[size-1]=',';
    strcat(array,item);
    size=strlen(array);
    array[size]=']';
    array[size+1]='\0';

}
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
    else printf("Server listening..\n");
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
                text_builder(result,"type","Successful","content","");
                send(client_socket, result, sizeof(result), 0);
                fclose(fuser);
                char output[1000];
                fuser=fopen(address,"w");
                text_builder(output,"username",user,"password",pass);
                fprintf(fuser,"%s",output);
                fclose(fuser);
            }
            else{
                char result[1000];
                text_builder(result,"type","Error","content","username is taken!");
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
                text_builder(result,"type","Error","content","user with this username doesnt exist!");
                send(client_socket, result, sizeof(result), 0);
            }
            else{
                char p[1000];
                fgets(json,1000,fuser);
                my_objcet_getItem(json,"password",p);
                fclose(fuser);
                if (strcmp(p,pass)==0){
                    char random_token[31];
                    srand((unsigned)time(NULL));
                    for(int i=0;i<30;i++){
                        int s=(rand()%26)+97;
                        random_token[i]=s;
                    }
                    random_token[30]='\0';
                    // random_token created
                    char result[1000];
                    text_builder(result,"type","Successful","content",random_token);
                    send(client_socket, result, sizeof(result), 0);
                    printf("%s",result);
                    strcpy(online_people[onpeop].user,user);
                    strcpy(online_people[onpeop].token,random_token);
                    onpeop++;
                }
                else {
                    char result[1000];
                    text_builder(result,"type","Error","content","password is incorrect!");
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
                text_builder(result,"type","Error","content","token is incorrect!");
                send(client_socket, result, sizeof(result), 0);
                fclose(fchannel);
            }
            else if(   (fchannel=fopen(address,"r"))!=NULL ){
                char result[1000];
                text_builder(result,"type","Error","content","channel exists!");
                send(client_socket, result, sizeof(result), 0);
                fclose(fchannel);

            }
            else {
                fclose(fchannel);
                int n=token_check(token);
                char result[1000];
                text_builder(result,"type","Successful","content","");
                send(client_socket, result, sizeof(result), 0);
                strcpy(online_people[n].channel,channel_name);
                char output[1000];

                //adding a message to the cJSON

                char message[100];
                sprintf(message,"channel created by %s",online_people[n].user);

                //printing it to the file
                strcpy(output,"{\"type\":\"\",\"content\":\"[{\"sender\":\"server\",\"message\":\"");
                strcat(output,message);
                strcat(output,"\"");
                strcat(output,"}]}");
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
                text_builder(result,"type","Error","content","token is incorrect!");
                send(client_socket, result, sizeof(result), 0);
            }
            else if(   (fchannel=fopen(address,"r"))==NULL ){
                char result[1000];
                text_builder(result,"type","Error","content","channel doesnt exist!");
                send(client_socket, result, sizeof(result), 0);
                fclose(fchannel);
            }
            else {
                fclose(fchannel);
                char result[1000];
                text_builder(result,"type","Successful","content","");
                send(client_socket, result, sizeof(result), 0);
                char message[100];
                sprintf(message,"%s joined the channel",online_people[user_token_number].user);
                FILE * fch;
                char o[100];
                text_builder(o,"sender","server","message",message);

                char json[10000],content[10000];
                fch=fopen(address,"r");
                fgets(json,10000,fch);
                json_get_string(json,content);
                fclose(fch);
                AddItemToArray(content,o);

                char output[1000];
                strcpy(output,"{\"type\":\"\",");
                strcat(output,content);
                strcat(output,"}");

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
                    text_builder(result,"type","Error","content","user doesnt have a channel!");
                    send(client_socket, result, sizeof(result), 0);
                    }
                    else{
                    memset(address, 0, sizeof(address));
                    sprintf(address,"resources\\channels\\%s.channel.json",online_people[user_token_number].channel);
                    char json[1000];
                    FILE *fchannel;
                    fchannel=fopen(address,"r");
                    fgets(json,1000,fchannel);
                    fclose(fchannel);


                    json[strlen(json)-3]='\0';
                    strcat(json,"},{");
                    strcat(json,"\"");
                    strcat(json,"sender\":");
                    strcat(json,"\"");
                    strcat(json,online_people[user_token_number].user);
                    strcat(json,"\",");
                    strcat(json,"\"message\":\"");
                    strcat(json,message);
                    strcat(json,"\"}]}");


                    //adding the message to it

                    //printing the new cJSON to file

                    fchannel=fopen(address,"w");
                    fprintf(fchannel,"%s",json);
                    fclose(fchannel);
                    char temp_result[1000];
                    text_builder(temp_result,"type","Successful","content","");
                    send(client_socket, temp_result, sizeof(temp_result), 0);

                    }
            }

            else{
                char result[1000];
                text_builder(result,"type","Error","content","token is incorrect!");
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

                        char s[1000];
                        json_get_string(result,s);
                        int length=ArrayBruket_number(s);

                        FILE * FF;
                        FF=fopen(s_adr,"w");
                        fprintf(FF,"%d",length);
                        printf("the length %d",length);
                        fclose(FF);
                    }

                    //the case that they have already refreshed the channel
                    else{
                        char output[1000];

                        char s[1000],array[1000];
                        strcpy(array,"[]");
                        json_get_string(result,s);
                        //receiving the number of messages that they've already seen
                        fscanf(FP,"%d",&seen);

                        int length=ArrayBruket_number(s);

                        for(int i=10;i<strlen(s);i++){
                            s[i-10]=s[i];
                        }

                        s[strlen(s)-10]='\0';
                        printf("%s",s);
                        remove_firstANDlast_char(s);
                        //start from the message with index seen

                        for(int i=1+seen;i<=length;i++){
                            char auxilary[1000];
                            ArrayObjectBracket(s,auxilary,i);
                            strcat(auxilary,"}");
                            AddItemToArray(array,auxilary);


                        }

                        for(int i=1;i<strlen(array)-1;i++){
                            result[i]=array[i+1];
                        }
                        printf("\n the array is: %s \n",array);

                        result[strlen(array)-1]='\0';
                        printf("\n%s\n",result);
                        strcpy(output,"{\"type\":\"\",\"content\":");
                        strcat(output,array);

                        strcat(output,"}");
                        printf("\n the output is: %s\n",output);

                        send(client_socket, output, sizeof(output), 0);
                        fclose(FP);
                        FP=fopen(s_adr,"w");
                        fprintf(FP,"%d",length);
                        fclose(FP);

                    }

                    }

                else{
                    char result[1000];
                    text_builder(result,"type","Error","content","user has no channel");
                    send(client_socket, result, sizeof(result), 0);

                }
            }
            else{
                char result[100];
                text_builder(result,"type","Error","content","token is incorrect");
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
                    text_builder(result,"type","Error","content","user has no channel");
                    send(client_socket, result, sizeof(result), 0);
                }
                else{
                    char result[1000];
                    strcpy(result,"[]");

                    //checking each of the users that are in some channel
                    //whether they're in the same channel of the user
                    //and adding them to the list if they were

                    for(int i=0;i<999;i++){
                        if (strcmp(online_people[i].channel,online_people[user_token_number].channel)==0){
                            char c[100];
                            sprintf(c,"\"%s\"",online_people[i].user);
                            AddItemToArray(result,c);
                        }
                    }

                    for(int i=1;i<strlen(result)-1;i++){
                        result[i]=result[i+1];
                    }
                    result[strlen(result)-1]='\0';

                    //sending the cJSON to client
                    char res[10000];
                    strcpy(res,"{\"type\":\"list\",\"content\":");
                    strcat(res,result);
                    strcat(res,"}");
                    printf("%s\n",res);
                    send(client_socket, res, sizeof(res), 0);
                }
            }
            else{
                char result[100];
                text_builder(result,"type","Error","content","token is incorrectl");
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
                    text_builder(result,"type","Error","content","the user is in no channel");
                    send(client_socket, result, sizeof(result), 0);
                }
                else{
                    char result[1000];
                    text_builder(result,"type","Successful","content","");
                    send(client_socket, result, sizeof(result), 0);
                    char channel_name[100];
                    strcpy(channel_name,online_people[user_token_number].channel);
                    char message[100];
                    memset(address,0,sizeof(address));
                    sprintf(address,"resources\\channels\\%s.channel.json",channel_name);
                    sprintf(message,"%s left the channel",online_people[user_token_number].user);
                    FILE * fchannel;
                    char json[1000];
                    fchannel=fopen(address,"r");
                    fgets(json,1000,fchannel);

                    json[strlen(json)-3]='\0';
                    strcat(json,"},{");
                    strcat(json,"\"");
                    strcat(json,"sender\":\"server");
                    strcat(json,"\",\"");
                    strcat(json,"message\":\"");
                    strcat(json,message);
                    strcat(json,"\"}]}");


                    fclose(fchannel);

                    //printing the new cJSON to the file

                    fchannel=fopen(address,"w");
                    fprintf(fchannel,"%s",json);
                    fclose(fchannel);
                    strcpy(online_people[user_token_number].channel,"\0");
                }
            }
            else{
                char result[1000];
                text_builder(result,"type","Error","content","token is incorrectl");
                send(client_socket, result, sizeof(result), 0);
            }
       }
       else if(strcmp(first,"logout")==0){
            char token[100];
            sscanf(buffer,"logout %s",token);
            int user_token_number=token_check(token);
            if(user_token_number!=-1){
                char result[1000];
                text_builder(result,"type","Successful","content","");
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
                text_builder(result,"type","Error","content","token is incorrectl");
                send(client_socket, result, sizeof(result), 0);
            }
       }
       closesocket(server_socket);
    }
}
