#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <array>
#include <poll.h>
#include <errno.h>

int main() {
    // set up cout and stuff for this usage, also no printf
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    constexpr size_t maxBufLength = 4096;
    constexpr char socketClosedMessage[] = "[{\"error\": \"connection closed\"}]";

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        std::cerr << "Socket not opened";
        return 1;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_len = sizeof(addr);
    addr.sun_family = PF_UNIX;

    // get tmpdir
    const size_t sunPathLimit = sizeof(addr.sun_path);
    const char* socketName = "focusassist9_nmhostpipe";
    const size_t socketNameLen = strlen(socketName);
    // subtract 1 for null character
    const size_t confstrMaxSize = sunPathLimit - 1 - socketNameLen;
#ifdef __APPLE__
    size_t confstrSize = confstr(_CS_DARWIN_USER_TEMP_DIR, addr.sun_path, confstrMaxSize);
#else
    const char* tmp_p = std::getenv("XDG_RUNTIME_DIR");
    size_t confstrSize = strlen(tmp_p);
#endif
    if (confstrSize == 0) {
        std::cerr << "Unknown tmpdir path." << std::endl;
        return 1;
    } else if (confstrSize > confstrMaxSize) {
        std::cerr << "path to tmpdir is probably too long" << std::endl;
        return 1;
    }
#ifndef __APPLE__
    std::strncpy(addr.sun_path, tmp_p, confstrSize+1)
#endif
    std::strncat(addr.sun_path, socketName, socketNameLen);

    if (connect(sockfd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr)) < 0) {
        std::cerr << "failed to connect: " << strerror(errno) << std::endl;
        return 1;
    }

    constexpr size_t pfds_len = 2;
    struct pollfd pfds[pfds_len] = {
        { .fd = STDIN_FILENO, .events = POLLIN },
        { .fd = sockfd, .events = POLLIN }
    };
    short eventsStdin, eventsSock;

    std::array<char, maxBufLength> buffer;

    while (true) {
        if (poll(pfds, pfds_len, -1) > 0) {
            eventsStdin = pfds[0].revents; eventsSock = pfds[1].revents;

            if (eventsStdin & (POLLHUP | POLLERR | POLLNVAL)) {
                std::cerr << "Connection closed." << std::endl;
                close(sockfd);
                return 0;
            }
            if (eventsSock & (POLLHUP | POLLERR | POLLNVAL)) {
                std::cerr << "Socket closed. Sending closed message." << std::endl;
                std::cout << socketClosedMessage << std::endl;

                close(sockfd);
                return 0;
            }
            if (eventsStdin & POLLIN) {
                std::string response;
                getline(std::cin, response); // will crash if failed
                send(sockfd, response.c_str(), response.length(), 0);
            }
            if (eventsSock & POLLIN) {
                int bytesRecieved = 0;
                do {
                    // DONTWAIT makes sure that if we send exatly maxBufLength chars, it will not block
                    bytesRecieved = recv(sockfd, &buffer[0], buffer.size(), MSG_DONTWAIT);
                    // append string from buffer.
                    if ( bytesRecieved == -1 ) {
                        // error
                    } else {
                        std::copy(buffer.cbegin(), buffer.cbegin() + bytesRecieved, std::ostream_iterator<char>(std::cout));
                    }
                } while (bytesRecieved == maxBufLength);
                std::cout << std::endl; // flush and print new line
            }
        }
    }


    return 0;
}
