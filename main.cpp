#include <iostream>
#include <map>
#include <string>
#include <cstring>

int main() {
    //note: in clion terminal output is incomplete
    FILE *history_stream = popen("dnf history", "r");

    char history_buffer[256];
    std::map<std::string, bool> package_map;

    bool install;
    bool remove;

    std::string line;
    std::string name_t;

    //if appear second time just sip because we only cares about the last time of remove/install
    while (fgets(history_buffer, sizeof(history_buffer), history_stream) != NULL) {
        line = history_buffer;
        install = line.find(" install ") != std::string::npos || line.find(" reinstall ") != std::string::npos;
        remove = line.find(" remove ") != std::string::npos;
        if (install) {
            name_t = line.substr(line.find("install") + 8);
            name_t.erase(name_t.find_first_of(' '));
            if (package_map.find(name_t) == package_map.end()) {
                package_map.insert({name_t, false});
            }
        } else if (remove){
            name_t = line.substr(line.find("remove") + 7);
            name_t.erase(name_t.find_first_of(' '));
            if (package_map.find(name_t) == package_map.end()) {
                package_map.insert({name_t, true});
            }
        }
    }
    // print in alphabetical order
    for (auto &it: package_map) {
        if (!it.second) {
            std::cout << it.first << std::endl;
        }
    }

    pclose(history_stream);
    return EXIT_SUCCESS;
}



