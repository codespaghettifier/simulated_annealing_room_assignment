
# compilator
CXX = mpicxx

# flags
CXXFLAGS = -Wall -O3 -I./ 

# exec
EXECUTABLE = out

# sources
SOURCES = testMain.cpp src/CostMatrix.cpp src/RoomsAssignment.cpp src/AnnealingWorker.cpp src/AnnealingTask.cpp

# headers
HEADERS = include/CostMatrix.hpp include/RoomsAssignment.hpp include/AnnealingWorker.hpp include/AnnealingTask.hpp include/Constants.hpp

# object
OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(EXECUTABLE)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)

