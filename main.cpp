#include <iostream>
#include <map>
#include <deque>
#include <string>
#include <cstring>

int main() {
    //note: in clion terminal output is incomplete
    FILE *install_stream = popen("dnf history | grep 'install' | awk '{print $4}' | sort -u", "r");
    FILE *history_stream = popen("dnf history", "r");
    if (!install_stream) {
        std::cerr << "Failed to execute command\n";
        return EXIT_FAILURE;
    }

    char install_buffer[256];
    char history_buffer[256];
    std::map<int, std::pair<std::string, int>> package_map;

    int i = 0;
    while (fgets(install_buffer, sizeof(install_buffer), install_stream) != nullptr) {
        install_buffer[strlen(install_buffer) - 1] = '\0';
        package_map.insert(std::pair(i, std::pair<std::string, int>(install_buffer, 0)));
        i++;
    }

    std::deque<std::string> reverse_history;
    while (fgets(history_buffer, sizeof(history_buffer), history_stream) != NULL) {
        reverse_history.emplace_front(history_buffer);
    }

    bool install;
    bool remove;
    for (const auto& line : reverse_history) {
        install = line.find("install") != std::string::npos || line.find("reinstall") != std::string::npos;
        remove = line.find("remove") != std::string::npos ;
        if(!install && !remove){ continue;}
        for (int k = 0; k < package_map.size(); ++k) {
            if (line.find(package_map.at(k).first)!= std::string::npos) {
                if (remove) {
                    package_map.at(k).second--;
                } else {
                    package_map.at(k).second++;
                }
            }
        }
    }

    //print in alphabetical order
    for (auto & it : package_map) {
        if(it.second.second >= 1){
            std::cout << it.second.first << std::endl;
        }
    }

    pclose(history_stream);
    pclose(install_stream);
    return EXIT_SUCCESS;
}

