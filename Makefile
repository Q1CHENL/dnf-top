all: dnleaf

dnleaf: dnleaf.py
	# Adding shebang to dnleaf.py and creating the executable
	echo "#!/usr/bin/env python3" > dnleaf
	cat dnleaf.py >> dnleaf
	chmod +x dnleaf

clean:
	rm -f dnleaf

.PHONY: all clean install uninstall
