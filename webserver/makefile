all:
	clean webserver

clean: 
	rm dist/webserver 

webserver: webserver.c
	gcc -Wall -Wextra -O2 -g webserver.c -o dist/webserver