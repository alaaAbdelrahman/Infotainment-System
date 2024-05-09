#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <regex>

#define PORT 8080

std::string read_temp_data() {
    std::ifstream file("/sys/kernel/mpu6050_sysfs/mpu6050_data");
    std::string line;
    if (file.is_open()) {
        if (getline(file, line)) {  // Assuming all data is on one line
            std::size_t pos = line.find("Temp:");
            if (pos != std::string::npos) {
                std::size_t start = pos + 5; // Start after "Temp:"
                std::size_t end = line.find("°", start); // Find end at ° sign
                std::string tempStr = line.substr(start, end - start);
                try {
                    float temp = std::stof(tempStr); // Convert string to float
                    int tempInt = static_cast<int>(temp); // Convert float to int
                    return std::to_string(tempInt); // Return integer part as string
                } catch (const std::exception& e) {
                    return "Error parsing temperature.";
                }
            }
        }
        file.close();
    }
    return "Temperature data not found.";
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    const char* gpio_file = "/sys/class/GPIO_CLASS/GPIO_16/value";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        valread = read(new_socket, buffer, 1024);  // Read data from the socket

        if (buffer[0] == 'D') {
            std::ifstream gpio(gpio_file);
            if (!gpio.is_open()) {
                std::cerr << "Failed to open GPIO file: " << gpio_file << std::endl;
            } else {
                std::string gpio_value;
                std::getline(gpio, gpio_value);
                gpio.close();
                send(new_socket, gpio_value.c_str(), gpio_value.length(), 0);
                std::cout << "GPIO value sent to client: " << gpio_value << std::endl;
            }
        } else if (buffer[0] == 'T') {
            std::string temp_data = read_temp_data();
             std::string temp="T:";
           temp=temp+temp_data;
            send(new_socket, temp_data.c_str(), temp_data.length(), 0);
            std::cout << "Temperature data sent to client: " << temp_data << std::endl;
        }

        close(new_socket);
    }

    return 0;
}
