all: dnf-top

dnf-top: dnf_top.py
	# Adding shebang to dnleaf.py and creating the executable
	echo "#!/usr/bin/env python3" > dnf-top
	cat dnleaf.py >> dnf-top
	chmod +x dnf-top

clean:
	rm -f dnf-top

install:
	echo "#!/usr/bin/env python3" > dnf-top
	cat dnleaf.py >> dnf-top
	chmod +x dnf-top
	sudo cp dnf-top /usr/local/bin/dnf-top

uninstall:
	sudo rm /usr/local/bin/dnf-top
	

.PHONY: all clean install uninstall
