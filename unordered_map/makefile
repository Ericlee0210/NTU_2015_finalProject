CXX = g++-4.8
CXXFLAGS = -O2 -std=c++11 -Wall
target = final_project

$(target): data.cpp data.h system.cpp md5.cpp md5.h
	$(CXX) $(CXXFLAGS) md5.cpp data.cpp system.cpp -o $@

run_test: final_project
	./final_project < 200000.in > 200000.out
run: final_project
	./final_project
clean:
	rm final_project
