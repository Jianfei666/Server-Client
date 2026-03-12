#include <iostream>
#include <sys/socket.h> // 包含 socket, bind, listen, accept 等网络函数
#include <unistd.h> // Linux 标准头文件，包含 close, read, write 等函数
#include <string.h>
#include <arpa/inet.h>  // 包含 socket, bind, listen, inet_ntoa 等网络函数

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    // 创建套接字
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1) {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in address; // IPv4 地址结构体
    memset(&address, 0, sizeof(address)); // 初始化地址结构体

    // 设置地址结构体
    address.sin_family = AF_INET; // IPv4
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有接口
    address.sin_port = htons(PORT); // 设置端口号

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {    // 绑定套接字到地址
        perror("bind failed");
        return -1;
    }

    // 监听连接
    if (listen(server_fd, 3) < 0) { // 监听连接，参数 3 是连接队列的最大长度
        perror("listen failed");
        return -1;
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    // 接受客户端连接
    int new_socket;
    int addrlen = sizeof(address);
    
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen); // 接受连接
    if (new_socket < 0) {
        perror("accept failed");
        return -1;
    }

    std::cout << "Connection accepted from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port) << std::endl; // 输出客户端的 IP 地址和端口号

    // 接受客户端消息
    char buffer[BUFFER_SIZE] = {0};

    int valread = read(new_socket, buffer, BUFFER_SIZE); // 读取客户端消息
    if (valread < 0) {
        perror("read failed");
        return -1;
    }
    std::cout << "Received message: " << buffer << std::endl; // 输出接收到的消息

    // 发送响应给客户端
    const char* hello = "Hello from server!";
    send(new_socket, hello, strlen(hello), 0); // 发送响应消息
    std::cout << "Response sent" << std::endl;

    // 关闭连接
    close(new_socket); // 关闭客户端套接字
    close(server_fd); // 关闭服务器套接字

    return 0;
}
