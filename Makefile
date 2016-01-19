all:
	$(CXX) -o search.exe main.cc anqi.cc ClientSocket.cpp Protocol.cpp  -Wno-invalid-source-encoding
#	$(CXX) -o gen_order_tab.exe gen_order_tab.cpp anqi.cc ClientSocket.cpp Protocol.cpp  -Wno-invalid-source-encoding

LDFLAGS=-static -s -lws2_32  -Wno-invalid-source-encoding
.PHONY:	clean
search.exe:	main.cc anqi.cc ClientSocket.cpp Protocol.cpp
	$(CXX) -o $@ $^ $(LDFLAGS)
clean:
	DEL search.exe 2>NUL
