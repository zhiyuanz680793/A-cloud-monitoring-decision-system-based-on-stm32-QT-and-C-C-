#include <iostream>
#include <cstring>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <thread>
#include <unordered_map>

#define PORT 8080
#define MAX_EVENTS 10
#define BUFFER_SIZE 1024

void setNonBlocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}

// 线程管理（避免同一个客户端重复开启线程）
std::unordered_map<int, std::thread> clientThreads;

void handleClient(int client_fd) {
    char buffer[BUFFER_SIZE];
    std::cout << "处理客户端 " << client_fd << " 线程启动。" << std::endl;

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);

        if (bytes_read > 0) {
            std::cout << "客户端 " << client_fd << " 发送: " << buffer << std::endl;

            if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
                std::cout << "客户端 " << client_fd << " 发送终止请求，断开连接。" << std::endl;
                break;
            }

            int temperature = atoi(buffer);
            std::string response = (temperature > 30) ? "FAN_ON" : "FAN_OFF";
            send(client_fd, response.c_str(), response.size(), 0);
        } 
        else if (bytes_read == 0) {  
            std::cout << "客户端 " << client_fd << " 关闭了连接。" << std::endl;
            break;
        } 
        else if (bytes_read < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                continue;
            } else {
                std::cerr << "读取数据错误，客户端 " << client_fd << " 断开。" << std::endl;
                break;
            }
        }
    }

    close(client_fd);
    std::cout << "客户端 " << client_fd << " 连接已关闭。" << std::endl;

    // 线程退出后，从线程管理表中删除
    clientThreads.erase(client_fd);
}

int main() {
    int server_fd, epoll_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    bool first_connection = true;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    setNonBlocking(server_fd);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    listen(server_fd, 5);
    std::cout << "服务器启动成功，监听端口 " << PORT << std::endl;

    epoll_fd = epoll_create1(0);
    struct epoll_event event, events[MAX_EVENTS];
    event.data.fd = server_fd;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    while (true) {
        int event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < event_count; i++) {
            if (events[i].data.fd == server_fd) {
                int client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);
                setNonBlocking(client_fd);

                if (first_connection) { 
                    std::cout << "服务器已启动并成功接受第一个客户端连接。" << std::endl;
                    first_connection = false;
                }

                event.data.fd = client_fd;
                event.events = EPOLLIN;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);

                // 只创建一次线程
                clientThreads[client_fd] = std::thread(handleClient, client_fd);
                clientThreads[client_fd].detach();
            }
        }
    }

    close(server_fd);
    return 0;
}
