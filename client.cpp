#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>  // 包含 socket, bind, listen, inet_ntoa 等网络函数
#include <unistd.h> // Linux 标准头文件，包含 close,

#define PORT 8080
#define IP "127.0.0.1"

int main() {

    int sock = socket(AF_INET, SOCK_STREAM, 0); // 创建套接字
    if (sock < 0) {
        perror("socket failed");
        return -1;
    }

    struct sockaddr_in serv_addr; // 服务器地址结构体
    memset(&serv_addr, 0, sizeof(serv_addr)); // 初始化地址结构体

    serv_addr.sin_family = AF_INET; // IPv4
    serv_addr.sin_port = htons(PORT); // 设置端口号

    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr) <=0) {  // 将 IP 地址转换为二进制形式
        perror("Invalid address/ Address not supported");
        return -1;
    }

    // 连接服务器
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    // 接受服务器响应
    char buffer[1024] = {0};
    std::string input;

    while (true) {

        std::cout << "输入消息（输入 quit 退出）: ";
        std::getline(std::cin, input);
        if (input == "quit") {
            break; // 退出循环，结束程序
        }
        send(sock, input.c_str(), input.length(), 0); // 发送消息到服务器
        std::cout << "Message sent to server: " << input << std::endl;

        memset(buffer, 0, sizeof(buffer)); // 清空缓冲区
        int valread = read(sock, buffer, 1024); // 读取服务器响应

        if (valread <= 0) {
            if (valread == 0) {
                std::cout << "Server closed the connection." << std::endl; // 输出服务器关闭连接的消息
            } else {
                perror("read failed");
            }
            break; // 退出循环，结束程序
        }

        std::cout << "Message received from server: " << buffer << std::endl;
    }
    
    close(sock); // 关闭套接字
    std::cout << "客户端已断开" << std::endl; // 输出客户端断开连接的消息
    return 0;
}
