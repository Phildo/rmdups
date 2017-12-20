make: rmdups
	

run: rmdupspassword.txt
	

debug:
	gcc -ggdb3 rmdups.cpp && gdb ./rmdups

rmdups: rmdups.cpp
	gcc rmdups.cpp

rmdupspassword.txt: rmdups
	./rmdups
