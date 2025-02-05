# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++2a -Wall -Wextra -pedantic -pthread

# Targets
SERVER_SRC = server_grp.cpp
CLIENT_SRC = client_grp.cpp
SERVER_BIN = server_grp
CLIENT_BIN = client_grp

# Default target
all: $(SERVER_BIN) $(CLIENT_BIN)

# Compile server
$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# Compile client
$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

# Clean build artifacts
clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)



# # Compiler and flags
# CXX = g++
# CXXFLAGS = -std=c++2a -Wall -Wextra -pedantic -pthread

# # Targets
# SERVER_SRC = server.cpp  # Server source file
# CLIENT_SRC = client.cpp  # Client source file
# SERVER_BIN = server_grp  # Server executable name
# CLIENT_BIN = client_grp  # Client executable name

# # Default target
# all: $(SERVER_BIN) $(CLIENT_BIN)

# # Compile server
# $(SERVER_BIN): $(SERVER_SRC)
# 	$(CXX) $(CXXFLAGS) -o $(SERVER_BIN) $(SERVER_SRC)

# # Compile client
# $(CLIENT_BIN): $(CLIENT_SRC)
# 	$(CXX) $(CXXFLAGS) -o $(CLIENT_BIN) $(CLIENT_SRC)

# # Clean build artifacts
# clean:
# 	rm -f $(SERVER_BIN) $(CLIENT_BIN)

# # Run the server (optional helper target)
# run-server: $(SERVER_BIN)
# 	./$(SERVER_BIN)

# # Run a client (optional heexlper target)
# run-client: $(CLIENT_BIN)
# 	./$(CLIENT_BIN)


# # g++ -std=c++17 -pthread -o server server.cpp
# # g++ -std=c++17 -o client client.cpp
# # cd ASSIGNMENT1/cs425-2025/Homeworks/A1