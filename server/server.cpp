#include "server.h"

namespace server {

server::server() {
    try {
        start_server();
        listen_server();
    } catch (const std::runtime_error& e) {
        e.what();
    }
}

server::~server() {
    std::cout << "Close server\n";
    close_socket();
}

void server::close_socket() {
    shutdown(_socket_server, 2); // обрываем соединения

    if (_socket_server != -1) {
        close(_socket_server);
        _socket_server = -1;
    }

    shutdown(_socket_client, 2);

    if (_socket_client != -1) {
        close(_socket_client);
        _socket_client = -1;
    }
}

void server::start_server() {
    _socket_server = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket_server < 0) {
        throw std::runtime_error("socket initialization error");
    }

    std::cout << "Set socket: Success\n";

    _server_addr.sin_family         = AF_INET;
    _server_addr.sin_port           = PORT_SERVER;
    _server_addr.sin_addr.s_addr    = inet_addr(IP_ADRESS_SERVER);

    if (bind(_socket_server, (struct sockaddr*) &_server_addr, sizeof(_server_addr)) <0) {
        throw std::runtime_error("ip address or port is not connect");
    }

    std::cout << "Set IP address and port: Success\n";

    if (listen(_socket_server, SOMAXCONN)) {
        throw std::runtime_error("server can't listen");
    }

    std::cout << "Server listen\n";
}

bool server::add_client() {
    socklen_t len = sizeof(_client_addr);

    _socket_client = accept(_socket_server, (struct sockaddr *)&_client_addr, &len);

    if (_socket_client < 0) {
        std::cerr << "Cant to add new client\n";
        return false;
    }
    return true;
}

void server::listen_server() {
    while (1) {
        if (!add_client()) {
            throw std::runtime_error("Connection from the client was not established");
        }
    }
}

}
