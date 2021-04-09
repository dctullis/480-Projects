#
# PROGRAM: Assignment 4
# AUTHOR:  David Tullis
#

CXX = g++
CPPFLAGS = -g -pthread -ansi -pedantic -Wall -Wextra -Werror -std=c++14

main: main.cpp
	$(CXX) $(CPPFLAGS) -c -o main.o main.cpp
	$(CXX) $(CPPFLAGS) -o z1860353_project4 main.o 

clean:
	rm -f *.o main