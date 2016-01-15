all:
	g++-4.8 main.c board.cpp  -o test.exe

LDFLAGS=-static -s 
.PHONY:	clean
search.exe:	main.cc anqi.cc ClientSocket.cpp Protocol.cpp
	$(CXX) -o $@ $^ $(LDFLAGS)
clean:
	rm *.exe
	DEL search.exe 2>NUL

