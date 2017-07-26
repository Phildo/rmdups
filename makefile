make: a.out
	

run: rmdupspassword.txt
	

debug:
	gcc -ggdb3 rmdups.cpp && gdb ./a.out

a.out: rmdups.cpp
	gcc rmdups.cpp

rmdupspassword.txt: a.out
	./a.out
