//
// Created by pairs on 4/21/17.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cstring>
#include <thread>

int main()
{
    std::thread threads[65536];
    for (int i = 0; i < 1; i++) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(5));
        threads[i] = std::thread([]()
        {
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(23333);
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");

            auto cli_sock = socket(AF_INET, SOCK_STREAM, 0);

            if (connect(cli_sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
                std::cout << "connect failed" << std::endl;
                return false;
            }
            for (int j = 0; j < 1000; j++) {
                ssize_t len = send(cli_sock, "timemessage", strlen("timemessage"), 0);
                if (len < 0) {
                    break;
                }
                std::cout << "send get time messaage success" << std::endl;
                char buf[256];
                ssize_t rl = recv(cli_sock, buf, 256, 0);
                buf[rl] = '\0';
                std::cout << buf << std::endl;
              //  std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        });
    }

    for (int i = 0; i < 1; i++) {
        threads[i].join();
    }
}
