#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 8000
#define PORT 8080
#define SA struct sockaddr
struct sockaddr_in servaddr;
char token[100];
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

void chat(int server_socket)
{
	char buffer[MAX];
	int n;
	char user[50],pass[50];
	while (true) {
            login_menu:{

        printf("please chose using arrow keys\n");
        printf("login <-- \n");
        printf("register\n");

        //memset(buffer,0,sizeof(buffer));
       // memset(serv_response,0,sizeof(serv_response));

        //because we want to use arrows then
        //we define ch2

        int order=0;
        int ch;

        while(true){
            int check;
            check=order;
            ch= getch();  //to read arrow key

            //when the user presses enter means they've selected their option
            //and the ASCII code of enter is 13 so

            if(ch==13){
                break;
            }

            //then we've got four cases for the arrows

            if(ch==72 && order==1) order--;
            if(ch==80 && order==0) order++;
            if(ch==75 && order==0) order++;
            if(ch==77 && order==1) order--;

            //the user might click on other buttons so we clear the screen and print
            //it again only if the order(index of ***) has changed

            if (order!=check){

                if(order==0){
                    printf("please chose using arrow keys\n");
                    printf("login <-- \n");
                    printf("register\n");
                    }
                else if(order==1){
                    printf("please chose using arrow keys\n");
                    printf("login \n");
                    printf("register <-- \n");
                }
            }
        }
        if(order==1) {//register
                printf("please enter your username\n");
                scanf("%s",user);
                printf("please enter your password\n");
                scanf("%s",pass);
                memset(buffer, 0, sizeof(buffer));
                sprintf(buffer,"register %s, %s\n",user,pass);
                server_socket = socket(AF_INET, SOCK_STREAM, 0);
                connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                send(server_socket, buffer, sizeof(buffer), 0);
                memset(buffer, 0, sizeof(buffer));
                recv(server_socket, buffer, sizeof(buffer), 0);
                char result[1000];
                my_objcet_getItem(buffer,"type",result);
                if(strcmp(result,"Error")!=0){
                    printf("you registered successfully!\n");
                }
                else {
                    printf("user name was taken\n");
                }
                closesocket(server_socket);
        }
        else if(order==0) {//login
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
                recv(server_socket, buffer, sizeof(buffer),0);
                closesocket(server_socket);
                char result[1000],t_buff[1000];
                strcpy(t_buff,buffer);
                my_objcet_getItem(t_buff,"type",result);



                if(strcmp(result,"Error")!=0){
                    memset(result,0,sizeof(result));
                    strcpy(t_buff,buffer);

                    my_objcet_getItem(t_buff,"content",result);
                    strcpy(token,result);
                    printf("login was successfull!\n");
                    while (true){
                    channels_menu:{
                    printf("please chose using arrow keys!\n");
                    printf("1-create channel <--\n");
                    printf("2-join channel\n");
                    printf("3-logout\n");
                    int order=0;
                    int ch;

                    while(true){
                        int check;
                        check=order;
                        ch = getch();  //to read arrow key

                        //when the user presses enter means they've selected their option
                        //and the ASCII code of enter is 13 so

                        if(ch==13){
                            break;
                        }
                        if(ch==72 && order>0) order--;
                        if(ch==80 && order<2) order++;
                        if(ch==75 && order<2) order++;
                        if(ch==77 && order>0) order--;

                        if (order!=check){
                            system("@cls||clear");

                            if(order==0){
                                    printf("please chose using arrow keys!\n");
                                    printf("1-create channel <--\n");
                                    printf("2-join channel\n");
                                    printf("3-logout\n");
                            }
                            else if(order==1){
                                    printf("please chose using arrow keys!\n");
                                    printf("1-create channel \n");
                                    printf("2-join channel <--\n");
                                    printf("3-logout\n");
                            }

                            else if(order==2){
                                    printf("please chose using arrow keys!\n");
                                    printf("1-create channel \n");
                                    printf("2-join channel\n");
                                    printf("3-logout <--\n");
                            }
                        }
                    }
                    if(order==0){
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
                        closesocket(server_socket);
                        char result[1000],t_buff[1000];
                        strcpy(t_buff,buffer);

                        my_objcet_getItem(t_buff,"type",result);

                        //if there was no error goes the message menu

                        if(strcmp(result,"Error")!=0){

                            system("@cls||clear");
                            printf("channel created successfully!\n");
                        }

                        //but if there was some error print the error type and go back to the beginning of the while

                        else {
                            memset(result,0,sizeof(result));
                            strcpy(t_buff,buffer);
                            my_objcet_getItem(t_buff,"content",result);

                            system("@cls||clear");
                            printf("%s\n",result);

                        }
                    }
                    else if(order==1){
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
                        closesocket(server_socket);
                        char result[1000],t_buff[1000];
                        strcpy(t_buff,buffer);

                        my_objcet_getItem(t_buff,"type",result);

                        if(strcmp(result,"Error")!=0){
                            system("@cls||clear");
                            printf("you joined successfully!\n");
                            while(1){
                                send_menu:
                                printf("chose one by using arrow keys!\n");
                                printf("1-send message <-- \n");
                                printf("2-refresh\n");
                                printf("3-channel members\n");
                                printf("4-leave channel\n");

                                int order=0;

                                //ch2 will be filled with the ASCII code of button clicked

                                int ch2;

                                while(true){

                                    int ord;
                                    ord=order;
                                    ch2 = getch();  //to read arrow key

                                    //when the user presses enter means they've selected their option
                                    //and the ASCII code of enter is 13 so

                                    if(ch2==13){
                                        break;
                                    }

                                    if(ch2==72 && order>0) order--;
                                    if(ch2==80 && order<3) order++;
                                    if(ch2==75 && order<3) order++;
                                    if(ch2==77 && order>0) order--;

                                    if (order!=ord){
                                        system("@cls||clear");

                                        if(order==0){
                                            printf("chose one by using arrow keys!\n");
                                            printf("1-send message <-- \n");
                                            printf("2-refresh\n");
                                            printf("3-channel members\n");
                                            printf("4-leave channel\n");
                                        }
                                        else if(order==1){
                                            printf("chose one by using arrow keys!\n");
                                            printf("1-send message \n");
                                            printf("2-refresh <-- \n");
                                            printf("3-channel members\n");
                                            printf("4-leave channel\n");
                                        }

                                        else if(order==2){
                                            printf("chose one by using arrow keys!\n");
                                            printf("1-send message \n");
                                            printf("2-refresh\n");
                                            printf("3-channel members <-- \n");
                                            printf("4-leave channel\n");
                                        }

                                        else if(order==3){
                                            printf("chose one by using arrow keys!\n");
                                            printf("1-send message \n");
                                            printf("2-refresh\n");
                                            printf("3-channel members\n");
                                            printf("4-leave channel <-- \n");
                                        }

                                    }

                                }
                                if(order==0){
                                    char sendmas[100];
                                    scanf("%s",sendmas);
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"send %s, %s\n",sendmas,token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    closesocket(server_socket);
                                    char result[1000],t2_buff[10000];

                                    strcpy(t2_buff,buffer);

                                    //getting the object

                                    my_objcet_getItem(t2_buff,"type",result);

                                    system("@cls||clear");

                                    //if there was no error don't print anything but if there was some error
                                    //print the error message

                                    if(strcmp(result,"Error")==0){

                                        memset(t2_buff,0,sizeof(t2_buff));
                                        memset(result,0,sizeof(result));
                                        my_objcet_getItem(t2_buff,"content",result);

                                        //print the content if there was error

                                        printf("content : %s\n",result);

                                    }
                                    else{
                                        printf("message sent succesfully!");
                                    }
                                }
                                else if(order==1){
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"refresh %s\n",token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    closesocket(server_socket);
                                    char result[10000],t2_buff[10000];
                                    strcpy(t2_buff,buffer);

                                    my_objcet_getItem(t2_buff,"type",result);

                                    if(strcmp(result,"Error")!=0){

                                        //we receive the string but we changed it somehow to get information we want

                                        system("@cls||clear");
                                        strcpy(t2_buff,buffer);
                                        memset(result,0,sizeof(result));

                                        char s[10000];
                                        char compare[10000];
                                        //sprintf(compare,""type":"","content""""")
                                        if(strcmp(t2_buff,"{\"type\":\"\",\"content\":[]}")){
                                        mystringStrtore2(t2_buff,s);
                                            for(int i=10;i<strlen(t2_buff);i++){
                                                t2_buff[i-10]=t2_buff[i];

                                            }
                                            //printf("%s\n",buf);

                                            t2_buff[strlen(t2_buff)-10]='\0';
                                            //printf("%s\n",buf);

                                            remove_firstANDlast_char(t2_buff);

                                            for(int i=0;i<strlen(t2_buff)-1;i++){
                                                t2_buff[i]=t2_buff[i+1];
                                            }
                                            t2_buff[strlen(t2_buff)-1]='\0';
                                            //printf("%s\n",buf);
                                        }

                                        else{
                                            continue;
                                        }

                                        //printf("%s\n",buf);
                                        int length=ArrayBruket_number(t2_buff);

                                        for(int i=1;i<=length;i++){

                                            char sender[100],content[10000],unit[10000];
                                            memset(sender,0,sizeof(sender));
                                            memset(sender,0,sizeof(sender));
                                            memset(content,0,sizeof(content));

                                            ArrayObjectBracket(t2_buff,unit,i);
                                            strcat(unit,"}");
                                            char unit1[1000],unit2[1000];
                                            memset(unit1,0,sizeof(unit1));
                                            memset(unit2,0,sizeof(unit2));
                                            strcpy(unit1,unit);
                                            strcpy(unit2,unit);

                                            my_objcet_getItem(unit1,"sender",sender);
                                            my_objcet_getItem(unit2,"message",content);

                                            printf("sender is: %s\n message: %s\n",sender,content);
                                        }

                                    }

                                    //if the server response was error print the type of error
                                    //for example the token is wrong etc.

                                    else{
                                        memset(result,0,sizeof(result));
                                        strcpy(t2_buff,buffer);
                                        my_objcet_getItem(t2_buff,"content",result);

                                        system("@cls||clear");
                                        printf("%s\n",result);

                                    }

                                }
                                else if(order==2){
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"channel members %s\n",token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    closesocket(server_socket);
                                    char result[10000],t2_buff[1000];
                                    strcpy(t2_buff,buffer);

                                    my_objcet_getItem(t2_buff,"type",result);

                                    //if there was no error

                                    if(strcmp(result,"Error")!=0){

                                        //get the array the contains the members of channel and the length
                                        memset(result,0,sizeof(result));
                                        memset(t2_buff,0,sizeof(t2_buff));
                                        strcpy(t2_buff,buffer);

                                        char s[10000];
                                        mystringStrtore2(t2_buff,s);


                                        for(int i=10;i<strlen(t2_buff);i++){
                                            t2_buff[i-10]=t2_buff[i];

                                        }

                                        t2_buff[strlen(t2_buff)-10]='\0';

                                        remove_firstANDlast_char(t2_buff);


                                        int length=ArrayQuote_number(t2_buff);
                                        printf("members: \n");

                                        for(int i=1;i<=length;i++){
                                            if(i==1){
                                                char member[100];
                                                memset(member,0,sizeof(member));
                                                ArrayObjectQuote(t2_buff,member,i);

                                                printf("%s \n",member);
                                            }
                                            else{
                                                char member[100];
                                                memset(member,0,sizeof(member));
                                                ArrayObjectQuote(t2_buff,member,i);

                                                for(int i=1;i<strlen(member);i++){
                                                    printf("%c",member[i]);
                                                }
                                                printf("\n");

                                            }
                                        }
                                    }

                                    //if there was any error print the error type

                                    else{
                                        system("@cls||clear");

                                        memset(result,0,sizeof(result));
                                        memset(t2_buff,0,sizeof(t2_buff));
                                        strcpy(t2_buff,buffer);
                                        my_objcet_getItem(t2_buff,"content",result);

                                        printf("%s\n",result);

                                    }

                                }
                                else if(order==3){
                                    memset(buffer, 0, sizeof(buffer));
                                    sprintf(buffer,"leave %s\n",token);
                                    server_socket = socket(AF_INET, SOCK_STREAM, 0);
                                    connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                                    send(server_socket, buffer, sizeof(buffer), 0);
                                    memset(buffer, 0, sizeof(buffer));
                                    recv(server_socket, buffer, sizeof(buffer), 0);
                                    closesocket(server_socket);
                                    char result[1000],t2_buff[1000];
                                    strcpy(t2_buff,buffer);

                                    my_objcet_getItem(t2_buff,"type",result);

                                    if(strcmp(result,"Error")!=0){
                                        break;
                                    }

                                    //if the was some error print the error message and then go to beginning of the while

                                    else{
                                        memset(result,0,sizeof(result));
                                        strcpy(t2_buff,buffer);
                                        my_objcet_getItem(t2_buff,"content",result);

                                        system("@cls||clear");
                                        printf("%s\n",result);
                                    }


                                }
                            }
                        }

                        //but if there was some error print the error message and go back to the beginning the while

                        else {
                            strcpy(t_buff,buffer);
                            memset(result,0,sizeof(result));
                            my_objcet_getItem(t_buff,"content",result);

                            system("@cls||clear");
                            printf("%s\n",result);

                        }
                    }
                    else if(order==2){
                        memset(buffer, 0, sizeof(buffer));
                        sprintf(buffer,"logout %s\n",token);
                        server_socket = socket(AF_INET, SOCK_STREAM, 0);
                        connect(server_socket, (SA*)&servaddr, sizeof(servaddr));
                        send(server_socket, buffer, sizeof(buffer), 0);
                        memset(buffer, 0, sizeof(buffer));
                        recv(server_socket, buffer, sizeof(buffer), 0);
                        closesocket(server_socket);
                        char result[1000];
                        my_objcet_getItem(buffer,"type",result);

                        //if there wasn't any problem

                        if(strcmp(result,"Error")!=0){
                            printf("%s\n",result);
                            break;
                        }
                        else {
                            system("@cls||clear");

                            memset(result,0,sizeof(result));
                            my_objcet_getItem(buffer,"content",result);
                            printf("%s\n",result);

                        }
                    }
         }       }

                }

                //if there was some error, print the error type and go back to beginning of while

                else{
                    memset(result,0,sizeof(result));
                    strcpy(t_buff,buffer);
                    my_objcet_getItem(t_buff,"content",result);

                    printf("%s\n",result);

                }
        }

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
    }/*
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
