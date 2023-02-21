#include <iostream>
#include <map>
#include <cstring>
#include <vector>
#include <sstream>

int main() {
    //note: in clion terminal output is incomplete
    FILE *history_stream = popen("dnf history", "r");

    char history_buffer[256];
    std::map<std::string, bool> package_map;

    bool install;
    bool remove;

    std::string line;
    std::string names_all;
    std::string name;

    //if appear second time just sip because we only cares about the last time of remove/install
    while (fgets(history_buffer, sizeof(history_buffer), history_stream) != NULL) {
        line = history_buffer;
        install = line.find(" install ") != std::string::npos || line.find(" reinstall ") != std::string::npos;
        remove = line.find(" remove ") != std::string::npos;
        if (install) {
            names_all = line.substr(line.find("install") + 8);
            names_all.erase(names_all.find_first_of('|'));
            std::stringstream ss(names_all);
            while(ss >> name) {
                if(name.find(".x86_64") != std::string::npos){
                    name.erase(name.find_first_of(".x86_64"));
                }
                if (name[0] != '-' && package_map.find(name) == package_map.end()) {
                    package_map.insert({name, false});
                }
            }
        } else if (remove){
            names_all = line.substr(line.find("remove") + 7);
            names_all.erase(names_all.find_first_of('|'));
            std::stringstream ss(names_all);
            while(ss >> name) {
                if(name.find(".x86_64") != std::string::npos){
                    name.erase(name.find_first_of(".x86_64"));
                }
                if (package_map.find(name) == package_map.end()) {
                    package_map.insert({name, true});
                }
            }
        }
    }
    pclose(history_stream);
    int num = 0;
    // print in alphabetical order
    for (auto &it: package_map) {
        if (!it.second) {
            std::cout << it.first << std::endl;
            num++;
        }
    }
    std::cout << "[Total installed: " << num <<" top level packages]" << std::endl;
    return EXIT_SUCCESS;
}





