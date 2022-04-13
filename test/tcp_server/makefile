SRC_DIR = ../../src
TEST_DIR  = ../../test/tcp_server
INC_DIR = ../../inc

CXX = g++
CC = $(CXX)

WARNINGS = -pedantic
WARNINGS += -Wall -Werror

DEBUG = -g3
CXX_STD = c++17

LDLIBS   = -lpthread

CXXFLAGS = $(WARNINGS) $(DEBUG) -std=$(CXX_STD) $(LDLIBS)


CPPFLAGS = -I$(INC_DIR)

TARGET = server_test
OBJS = server_test.o $(SRC_DIR)/aggregator.o $(SRC_DIR)/data_base.o  $(SRC_DIR)/processor.o $(SRC_DIR)/protocol.o $(SRC_DIR)/tcp_server.o $(SRC_DIR)/server_manager.o  

$(TARGET):$(OBJS)

server_test.o : $(INC_DIR)/aggregator.hpp $(INC_DIR)/aggregate_data.hpp $(INC_DIR)/cdr_struct.hpp $(INC_DIR)/data_base.hpp $(INC_DIR)/reader.hpp  $(INC_DIR)/processor.hpp  $(INC_DIR)/protocol.hpp $(INC_DIR)/server_manager.hpp  $(INC_DIR)/http_server.hpp
aggregator.o: $(INC_DIR)/aggregator.hpp
data_base.o: $(INC_DIR)/data_base.hpp 
tcp_server.o: $(INC_DIR)/tcp_server.hpp 
processor.o: $(INC_DIR)/processor.hpp
protocol.o: $(INC_DIR)/protocol.hpp 
server_manager.o: $(INC_DIR)/server_manager.hpp  $(INC_DIR)/http_server.hpp



check: $(TARGET)
	./$(TARGET) -v

vala: $(TARGET)
	valgrind --leak-check=full --track-origins=yes --log-file=logValgrind01.txt ./$(TARGET)
	valgrind --tool=helgrind --log-file=loghelgrind01.txt ./$(TARGET)
	valgrind --tool=drd --log-file=logdrd01.txt ./$(TARGET)

prod: CXXFLAGS += -O2 -DNDEBUG
prod: $(TARGET)
	./$(TARGET) -v

clean:
	@$(RM) $(TARGET) $(OBJS)

.PHONY: check clean
