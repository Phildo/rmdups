make: rmdups
	

run: rmdupspassword.txt
	

debug:
	gcc -ggdb3 rmdups.cpp -o rmdups && gdb ./rmdups

rmdups: rmdups.cpp
	gcc rmdups.cpp -o rmdups

rmdupspassword.txt: rmdups
	./rmdups
