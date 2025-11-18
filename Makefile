TARGET = cache_simulator

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

SRCS = main.cpp \
       memory_level.cpp \
       main_memory.cpp \
       cache.cpp \
       cache_system.cpp \
       processor.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

rebuild: clean all