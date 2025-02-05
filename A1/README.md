### **README.md**
---

[Github link of the Assignment1](https://github.com/lohitpt252003/CS425-2025.git)


## **Group members**
- LOHIT P TALAVAR (210564)
- ANJALI MALOTH (210146)

---

## **Chat Server with Private Messages and Group Messaging**

This project implements a multi-threaded chat server in C++ that supports private messaging, group communication, and user authentication. The server allows multiple clients to connect simultaneously, authenticate themselves, and communicate with each other using various commands like `/msg <username> <message>`, `/create_group <group_name>`, `etc`.

---

## **Features**
1. **User Authentication**:
   - Clients must authenticate using a username and password stored in `users.txt`.
   - It will throw an error if you provide invalid credentials.

2. **Private Messaging**:
   - Users can send private messages to specific users using the `/msg` command.

3. **Group Messaging**:
   - Users can create, join, and leave the groups.
   - Group messages can be sent to all members of a group using the `/group_msg` command.

4. **Broadcast Messaging**:
   - User can broadcast messages to all connected clients using the `/broadcast` command.

5. **Thread Safety**:
   - The server uses mutexes to ensure thread-safe access to shared resources like the list of connected clients and groups.

---

## **How the Code Works**

### **Server Code (`server_grp.cpp`)**:
1. **Data Structures**:
   - `clients`: Maps client sockets to usernames.
   - `users`: Stores usernames and passwords loaded from `users.txt`.
   - `groups`: Maps group names to sets of client sockets (members).

2. **Authentication**:
   - The server prompts clients for their username and password.
   - Credentials are validated against the `users.txt` file.

3. **Command Handling**:
   - The server listens for client commands and processes them:
     - `/msg <username> <message>`: Sends a private message to a specific user.
     - `/broadcast <message>`: Sends a message to all connected clients.
     - `/create_group <group_name>`: Creates a new group.
     - `/join_group <group_name>`: Joins an existing group.
     - `/group_msg <group_name> <message>`: Sends a message to all members of a group.
     - `/leave_group <group_name>`: Leaves a group.
     - `/exit`: Disconnects the client from the server.

4. **Threading**:
   - Each client connection is handled in a separate thread to allow multiple clients to connect simultaneously.

5. **Broadcasting**:
   - The server broadcasts messages to all clients or specific groups as required.

---

### **Client Code (`client_grp.cpp`)**:
1. **Connection**:
   - The client connects to the server using the specified IP address and port.

2. **Authentication**:
   - The client sends its username and password to the server for authentication.

3. **Message Handling**:
   - The client sends messages to the server and receives responses.
   - A separate thread handles incoming messages from the server.

4. **Commands**:
   - The client supports the same commands as the server for sending messages and managing groups.

---

## **How to Run the Server and Client**

### **Steps to Run**

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/lohitpt252003/CS425-2025.git
   cd A1/A1
   ```

2. **Compile the Server and Client**:
   - Use the provided `Makefile` to compile the code:
     ```bash
     make
     ```
   - Or simply type the following:
     ```bash
     g++ -std=c++17 -pthread -o server_grp server_grp.cpp
     g++ -std=c++17 -o client_grp client_grp.cpp
     ```

3. **Run the Server**:
   - Start the server on port `12345`:
     ```bash
     ./server_grp
     ```

4. **Run the Client**:
   - Open a new terminal and run the client:
     ```bash
     ./client_grp
     ```
   - Follow the prompts to authenticate and start chatting.

5. **Test Commands**:
   - Use the following commands in the client:
     - `/msg <username> <message>`: Send a private message.
     - `/broadcast <message>`: Broadcast a message to all users.
     - `/create_group <group_name>`: Create a new group.
     - `/join_group <group_name>`: Join an existing group.
     - `/group_msg <group_name> <message>`: Send a message to a group.
     - `/leave_group <group_name>`: Leave a group.
     - `/exit`: Disconnect from the server.

---

## **File Descriptions**
1. **`server_grp.cpp`**:
   - The main server code that handles client connections, authentication, and message processing.

2. **`client_grp.cpp`**:
   - The client code that connects to the server and sends/receives messages.

3. **`users.txt`**:
   - A file containing usernames and passwords in the format `username:password`.

4. **`Makefile`**:
   - A file to compile the server and client code.

5. **`README.md`**:
   - This file, providing an overview of the project and instructions to run it.

---

## **Example Workflow**

1. **Start the Server**:
   ```bash
   ./server_grp
   ```

2. **Run the First Client**:
   ```bash
   ./client_grp
   ```
   - Enter username: `alice`
   - Enter password: `password123`
   - Use commands to chat and manage groups.

3. **Run the Second Client**:
   ```bash
   ./client_grp
   ```
   - Enter username: `bob`
   - Enter password: `qwerty456`
   - Join the same group as `alice` and start chatting.

4. **Broadcast a Message**:
   - In `alice`'s terminal:
     ```
     /broadcast Hello everyone!
     ```

5. **Send a Private Message**:
   - In `bob`'s terminal:
     ```
     /msg alice Hi Alice!
     ```

6. **Create and Join a Group**:
   - In `alice`'s terminal:
     ```
     /create_group CS425
     ```
   - In `bob`'s terminal:
     ```
     /join_group CS425
     ```

7. **Send a Group Message**:
   - In `alice`'s terminal:
     ```
     /group_msg CS425 Let's start the assignment!
     ```

---
