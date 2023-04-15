#include <iostream>
#include <vector>
#include <sstream>
#include <set>

int main(int argc, char* argv[]) {
    bool reverse_order = false;
    if (argc == 2 && std::string(argv[1]) == "--reverse") {
        reverse_order = true;
    }
    //note: in clion terminal output is incomplete
    FILE *history = popen("dnf history", "r");
    if (history == nullptr) {
        std::cerr << "Error: Failed to open history stream.\n";
        return EXIT_FAILURE;
    }

    char line_buf[256];
    std::set<std::string> package_set;
    std::vector<std::string> final_result;
    int num = 0;
    std::string suffix_x86_64 = ".x86_64";

    //if appear second time just sip because we only cares about the last time of remove/install
    while (fgets(line_buf, sizeof(line_buf), history) != nullptr) {
        std::string line = line_buf;
        bool install = line.find(" install ") != std::string::npos || line.find(" reinstall ") != std::string::npos;
        bool remove = line.find(" remove ") != std::string::npos;

        std::string names;
        std::string single_name;

        if (install) {
            names = line.substr(line.find("install") + 8);
            names.erase(names.find_first_of('|'));
            std::stringstream ss(names);
            while (ss >> single_name) {
                if (single_name.find(suffix_x86_64) != std::string::npos) {
                    single_name.erase(single_name.find_first_of(suffix_x86_64));
                }
                if (single_name[0] != '-' && package_set.count(single_name) == 0) {
                    package_set.insert(single_name);
                    final_result.emplace_back(single_name);
                    //std::cout << single_name << std::endl;
                    num++;
                }
            }
        } else if (remove) {
            names = line.substr(line.find("remove") + 7);
            names.erase(names.find_first_of('|'));
            std::stringstream ss(names);
            while (ss >> single_name) {
                if (single_name.find(suffix_x86_64) != std::string::npos) {
                    single_name.erase(single_name.find_first_of(suffix_x86_64));
                }
                if (package_set.count(single_name) == 0) {
                    package_set.insert(single_name);
                }
            }
        }
    }
    pclose(history);
//     print in alphabetical order
//    for (auto &it: package_set) {
//        if (!it.second) {
//            std::cout << it.first << std::endl;
//            num++;
//        }
//    }

    for(auto &str: final_result){
        std::cout << str << std::endl;
    }
    std::cout << "[Total installed: " << num << " top level packages]" << std::endl;
    return EXIT_SUCCESS;
}
