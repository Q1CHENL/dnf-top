all: dnf-top dnf5-top

dnf-top: dnf_top.py
	echo "#!/usr/bin/env python3" > dnf-top
	cat dnf_top.py >> dnf-top
	chmod +x dnf-top

dnf5-top: dnf5_top.py
	echo "#!/usr/bin/env python3" > dnf5-top
	cat dnf5_top.py >> dnf5-top
	chmod +x dnf5-top

clean:
	rm -f dnf-top dnf5-top

install:
	echo "#!/usr/bin/env python3" > dnf-top
	cat dnf_top.py >> dnf-top
	chmod +x dnf-top
	sudo cp dnf-top /usr/local/bin/dnf-top

	echo "#!/usr/bin/env python3" > dnf5-top
	cat dnf5_top.py >> dnf5-top
	chmod +x dnf5-top
	sudo cp dnf5-top /usr/local/bin/dnf5-top

uninstall:
	sudo rm /usr/local/bin/dnf-top
	sudo rm /usr/local/bin/dnf5-top
	

.PHONY: all clean install uninstall
