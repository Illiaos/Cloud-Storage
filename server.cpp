#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
using namespace std;
int main()
{
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if(listening < 0)
    {
        printf("Error can not create socket\n");
        return -1;
    }
    
    struct sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54003);
    inet_pton(AF_INET, "0.0.0.0", (sockaddr*)&hint.sin_addr);
    if(bind(listening, (sockaddr*)&hint, sizeof(hint)) == -1)
    {
        printf("Can not bind to ip port\n");
        return -2;
    }
    if(listen(listening, SOMAXCONN) == -1)
    {
        printf("Can not listen\n");
        return -3;
    }
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    
    int client_socket = accept(listening, (sockaddr*)&client, &clientSize);
    
    if(client_socket == -1)
    {
        printf("Client Problem\n");
        return -4;
    }
    
    close(listening);
    memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
    
    int result = getnameinfo((sockaddr*) &client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0);
    if(result)
    {
        cout<<host<<" connected on "<<service<<endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        cout<<host<<" connected on "<<ntohs(client.sin_port)<<endl;
    }
    
    char buf[4096];
    while (true)
    {
        memset(buf, 0, 4096);
        
        int bytesRecive = recv(client_socket, buf, 4096, 0);
        if(bytesRecive == -1)
        {
            cout<<"Connection issue "<<endl;
            break;
        }
        if(bytesRecive == 0)
        {
            cout<<"Client disconnected"<<endl;
            break;
        }
        cout<<"Received: "<<string(buf, 0, bytesRecive)<<endl;
        send(client_socket, buf, bytesRecive+1, 0);
    }
    close(client_socket);
    
    
    
}

