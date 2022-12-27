# to compile - gcc main.c -lpcap -lconfig

# to run - ./a.out


valgrind ./a.out --track-origins=yes --leak-check=full -s -g 

gcc main.c -lpcap -lconfig -fsanitize=address -g 