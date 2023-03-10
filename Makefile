#$@  表示目标文件
#$^  表示所有的依赖文件
#$<  表示第一个依赖文件
#$?  表示比目标还要新的依赖文件列表
CXX = g++
LD = ld 
SRC= $(wildcard source/*.cpp)
OBJS=$(patsubst %.cpp, %.o,$(SRC))
target= main
.PHONY:clean all
all: $(OBJS)
	$(CXX) $^ -o $(target) -g -Wall -lpthread
clean:
	rm  source/*.o $(target) 
%.o:%.cpp
	$(CXX) -c $< -o $@ -g -Wall
#Thread.o: Thread.cpp
#	$(CXX) -c source/$^ -o $@
#ThreadPool.o: ThreadPool.cpp
#	$(CXX) -c source/$^ -o $@
#main.o: test.cpp
#	$(CXX) -c source/$^ -o $@