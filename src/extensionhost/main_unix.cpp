#include <iostream>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <array>
#include <poll.h>
#include <errno.h>
#include <fstream>





/* REMINDER REMINDER REMINDER REMINDER REMINDER REMINDER REMINDER REMINDER


         do not forget to copy extensionhost file to /Applications

    trying to debug is impossible if i dont see anything change


   REMINDER REMINDER REMINDER REMINDER REMINDER REMINDER REMINDER REMINDER */






int main() {
    // set up cout and stuff for this usage, also no printf
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::ofstream log;
    log.open("/tmp/focacciat_extensionhost_errlog", std::ios::app | std::ios::out);
    log << "start" << std::endl;

    constexpr size_t maxBufLength = 4096;
    constexpr char socketClosedMessage[] = "{\"type\":\"connection\",\"description\":\"closed\"}";
    constexpr char socketOpenMessage[] = "{\"type\":\"connection\",\"description\":\"opened\"}";

    int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd == -1) {
        log << "Socket not opened: " << strerror(errno) << std::endl;
        return 1;
    }
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_len = sizeof(addr);
    addr.sun_family = PF_UNIX;

    // get tmpdir
    const size_t sunPathLimit = sizeof(addr.sun_path);
    constexpr char socketName[] = "focacciat_nmhostpipe";
    constexpr size_t socketNameLen = sizeof(socketName)/sizeof(char)-1;
    // subtract 1 for null character
    const size_t confstrMaxSize = sunPathLimit - 1 - socketNameLen;
#ifdef __APPLE__
    size_t confstrSize = confstr(_CS_DARWIN_USER_TEMP_DIR, addr.sun_path, confstrMaxSize);
#else
    const char* tmp_p = std::getenv("XDG_RUNTIME_DIR");
    size_t confstrSize = strlen(tmp_p) + 1;
#endif
    if (confstrSize == 0) {
        log << "Unknown tmpdir path." << std::endl;
        return 1;
    } else if (confstrSize > confstrMaxSize) {
        log << "path to tmpdir is probably too long" << std::endl;
        return 1;
    }
#ifndef __APPLE__
    std::strncpy(addr.sun_path, tmp_p, confstrSize)
#endif
    std::strncat(addr.sun_path, socketName, socketNameLen);

    do {
        // TODO: refactor loop out
        while (connect(sockfd, reinterpret_cast<const struct sockaddr *>(&addr), sizeof(addr)) < 0) {
            log << "failed to connect: " << strerror(errno) << std::endl;
            log << "Retrying in 10s..." << std::endl;
            // TODO: check if stdin still exists at this point/
            sleep(10);
        }
        constexpr uint32_t size = sizeof(socketClosedMessage)/sizeof(char);
        std::cout.write(reinterpret_cast<const char*>(&size), sizeof(char));
        std::cout << socketOpenMessage << std::flush;

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
                    log << "Stdin connection closed." << std::endl;
                    close(sockfd);
                    return 0;
                }
                if (eventsSock & (POLLHUP | POLLERR | POLLNVAL)) {
                    log << "Socket closed. Sending closed message." << std::endl;
                    constexpr uint32_t size = sizeof(socketClosedMessage)/sizeof(char);
                    std::cout.write(reinterpret_cast<const char*>(&size), sizeof(char));
                    std::cout << socketClosedMessage << std::flush;

                    close(sockfd);
                    break;
                }
                if (eventsStdin & POLLIN) {
                    uint32_t respSize{0};
                    std::cin.read(reinterpret_cast<char*>(&respSize), sizeof(respSize));
                    char* response = static_cast<char*>(malloc(sizeof(char)*respSize));
                    std::cin.read(response, respSize);
                    send(sockfd, response, respSize, 0);
                    free(response);
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
                    std::cout.flush();
                }
            }
        }
        log << "Creating new socket..." << std::endl;
        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (sockfd == -1) {
            log << "Socket not opened: " << strerror(errno) << std::endl;
            return 1;
        }
    } while (true);
    return 0;
}
