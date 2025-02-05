#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>

#define BUFFER_SIZE 1024
#define PORT 12345

std::mutex mtx; // Mutex for thread-safe access to shared resources

// Data structures
std::unordered_map<int, std::string> clients; // Maps client socket to username
std::unordered_map<std::string, std::string> users; // Maps username to password
std::unordered_map<std::string, std::unordered_set<int>> groups; // Maps group name to client sockets

// Function to load users from users.txt
void load_users(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string username = line.substr(0, colon);
            std::string password = line.substr(colon + 1);
            users[username] = password;
        }
    }
}

// Function to broadcast a message to all clients
void broadcast_message(const std::string& message, int exclude_socket = -1) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& client : clients) {
        if (client.first != exclude_socket) {
            send(client.first, message.c_str(), message.size(), 0);
        }
    }
}

// Function to send a private message to a specific user
void send_private_message(const std::string& username, const std::string& message) {
    std::lock_guard<std::mutex> lock(mtx);
    for (const auto& client : clients) {
        if (client.second == username) {
            send(client.first, message.c_str(), message.size(), 0);
            return;
        }
    }
}

// Function to handle client commands
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    std::string username;

    // Authentication
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        send(client_socket, "Enter username: ", 16, 0);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        username = std::string(buffer);

        send(client_socket, "Enter password: ", 16, 0);
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE, 0);
        std::string password = std::string(buffer);

        if (users.find(username) != users.end() && users[username] == password) {
            send(client_socket, "Welcome to the chat server!", 27, 0);
            break;
        } else {
            send(client_socket, "Authentication failed.", 24, 0);
            close(client_socket);
            return;
        }
    }

    // Add client to the list of connected clients
    {
        std::lock_guard<std::mutex> lock(mtx);
        clients[client_socket] = username;
    }

    // Notify all clients about the new user
    broadcast_message(username + " has joined the chat.", client_socket);

    // Handle client messages
    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            break;
        }

        std::string message(buffer);
        std::istringstream iss(message);
        std::string command;
        iss >> command;

        if (command == "/msg") {
            std::string recipient, msg;
            iss >> recipient;
            std::getline(iss, msg);
            send_private_message(recipient, "[" + username + "]: " + msg);
        } else if (command == "/broadcast") {
            std::string msg;
            std::getline(iss, msg);
            broadcast_message("[" + username + "]: " + msg + "", client_socket);
        } else if (command == "/create_group") {
            std::string group_name;
            iss >> group_name;
            {
                std::lock_guard<std::mutex> lock(mtx);
                groups[group_name].insert(client_socket);
            }
            send(client_socket, ("Group " + group_name + " created.").c_str(), BUFFER_SIZE, 0);
        } 
        else if (command == "/join_group") {
            std::string group_name;
            iss >> group_name;
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (groups.find(group_name) != groups.end()) {
                    groups[group_name].insert(client_socket);
                    send(client_socket, ("You joined the group " + group_name + ".").c_str(), BUFFER_SIZE, 0);
                } else {
                    send(client_socket, ("Group " + group_name + " does not exist.").c_str(), BUFFER_SIZE, 0);
                }
            }
        }
        else if (command == "/group_msg") {
            std::string group_name, msg;
            iss >> group_name;
            std::getline(iss, msg);
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (groups.find(group_name) != groups.end()) {
                    for (int socket : groups[group_name]) {
                        send(socket, ("[Group " + group_name + "]: " + msg + "").c_str(), BUFFER_SIZE, 0);
                    }
                } else {
                    send(client_socket, ("Group " + group_name + " does not exist.").c_str(), BUFFER_SIZE, 0);
                }
            }
        } else if (command == "/leave_group") {
            std::string group_name;
            iss >> group_name;
            {
                std::lock_guard<std::mutex> lock(mtx);
                if (groups.find(group_name) != groups.end()) {
                    groups[group_name].erase(client_socket);
                    send(client_socket, ("You left the group " + group_name + ".").c_str(), BUFFER_SIZE, 0);
                } else {
                    send(client_socket, ("Group " + group_name + " does not exist.").c_str(), BUFFER_SIZE, 0);
                }
            }
        } else if (command == "/exit") {
            break;
        } else {
            send(client_socket, "Unknown command.", 17, 0);
        }
    }

    // Remove client from the list of connected clients
    {
        std::lock_guard<std::mutex> lock(mtx);
        clients.erase(client_socket);
    }

    // Notify all clients about the user leaving
    broadcast_message(username + " has left the chat.", client_socket);

    close(client_socket);
}



int main() {
    // Load users from users.txt
    load_users("users.txt");

    // Create server socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    // Bind server socket to port
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error binding socket." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error listening on socket." << std::endl;
        return 1;
    }

    // std::cout << "Server is listening on port " << PORT << "..." << std::endl;

    // Accept incoming connections
    while (true) {
        sockaddr_in client_address{};
        socklen_t client_address_size = sizeof(client_address);
        int client_socket = accept(server_socket, (sockaddr*)&client_address, &client_address_size);
        if (client_socket < 0) {
            std::cerr << "Error accepting connection." << std::endl;
            continue;
        }

        // Handle client in a new thread
        std::thread(handle_client, client_socket).detach();
    }

    close(server_socket);
    return 0;
}