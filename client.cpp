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
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1)
    {
        return -1;
    }
    int port = 8888;
    string IpAddress = "127.0.0.1";
    
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, IpAddress.c_str(), &hint.sin_addr);
    int connection_res = connect(sock, (sockaddr*)&hint, sizeof(sockaddr_in));
    if(connection_res == -1)
    {
        return -2;
    }
    char buf[4096];
    string userInput;
    
    do
    {
        cout<<"> ";
        getline(cin, userInput);
        int sendRes = send(sock, userInput.c_str(), userInput.size() + 1, 0);
        
        memset(buf, 0, 4096);
        
        int bytesReceived = recv(sock, buf, 4096, 0);
        if(bytesReceived == -1)
        {
            cout<<"Error"<<endl;
        }
        else
        {
            cout<<"Server: "<<string(buf, bytesReceived)<<endl;
        }
        
    }while(true);
    close(sock);
}
