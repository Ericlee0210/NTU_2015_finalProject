CXX = g++-4.8
CXXFLAGS = -O2 -std=c++11 -Wall
TARGET1 = final_project_vector 
TARGET2 = final_project_map
TARGET3 = final_project_un
RM = rm -rf
TARGET = $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): data_vector.cpp data_vector.h system.cpp md5.cpp md5.h
	$(CXX) $(CXXFLAGS) md5.cpp data_vector.cpp system.cpp -o $@

$(TARGET2): data_map.cpp data_map.h system.cpp md5.cpp md5.h
	$(CXX) $(CXXFLAGS) md5.cpp data_map.cpp system.cpp -o $@

$(TARGET3): data_un.cpp data_un.h system.cpp md5.cpp md5.h
	$(CXX) $(CXXFLAGS) md5.cpp data_un.cpp system.cpp -o $@

run1: final_project_vector
	./final_project_vector

run2: final_project_map
	./final_project_map

run3: final_project_un
	./final_project_un

run_best: final_project_un
	./final_project_un

clean:
	$(RM) $(TARGET)
