import subprocess
import sys
import re


def is_web(name):
    return name[:8] == 'https://'


def main():
    try:
        history = subprocess.check_output(
            "dnf5 history list", shell=True).decode('utf-8')
    except subprocess.CalledProcessError as e:
        sys.stderr.write("Error: Failed to open history stream.\n")
        sys.exit(1)

    package_set = set()
    final_result = []
    num = 0
    suffix_x86_64 = ".x86_64"

    lines = history.split('\n')
    for line in lines:
        install = " install " in line or " reinstall " in line
        remove = " remove " in line

        if install:
            names = line[line.find("install") + 8:]
            names = names[:110].strip()
            for single_name in names.split():
                if suffix_x86_64 in single_name:
                    single_name = single_name[:single_name.find(suffix_x86_64)]
                if single_name[0].isalpha() and not is_web(single_name) and single_name not in package_set:
                    package_set.add(single_name)
                    final_result.append(single_name)
                    num += 1

        elif remove:
            names = line[line.find("remove") + 7:]
            names = names[:110].strip()
            for single_name in names.split():
                if suffix_x86_64 in single_name:
                    single_name = single_name[:single_name.find(suffix_x86_64)]
                if single_name not in package_set:
                    package_set.add(single_name)

    # Determine the maximum length of the package names
    max_len = max(len(name) for name in final_result)

    # Format and print package names in groups of 3, aligning each column
    for i in range(0, len(final_result), 3):
        print(''.join(f'{name:<{max_len+2}}' for name in final_result[i:i+3]))

    print(f"[Total installed: {num} top level packages]")


if __name__ == "__main__":
    main()
