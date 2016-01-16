all:
	$(CXX) -o search.exe main.cc anqi.cc ClientSocket.cpp Protocol.cpp
LDFLAGS=-static -s -lws_32
.PHONY:	clean
search.exe:	main.cc anqi.cc ClientSocket.cpp Protocol.cpp
	$(CXX) -o $@ $^ $(LDFLAGS)
clean:
	DEL search.exe 2>NUL
