CXX = g++
CXXFLAGS = -O2 -std=c++11 -Wall
target = system

$(target): data.cpp data.h system.cpp md5.cpp md5.h
	$(CXX) $(CXXFLAGS) md5.cpp data.cpp system.cpp -o $@

run_test: system
	./system < test.txt > testout.txt 
run: system
	./system
clean:
	rm system
