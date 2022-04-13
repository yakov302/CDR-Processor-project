SRC_DIR = ../../src
TEST_DIR  = ../../test/tcp_client
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

TARGET = client_test
OBJS = client_test.o  $(SRC_DIR)/reader.o  $(SRC_DIR)/protocol.o  $(SRC_DIR)/tcp_client.o  $(SRC_DIR)/client_manager.o

$(TARGET):$(OBJS)

client_test.o : $(INC_DIR)/cdr_struct.hpp $(INC_DIR)/reader.hpp  $(INC_DIR)/tcp_client.hpp  $(INC_DIR)/protocol.hpp $(INC_DIR)/client_manager.hpp
reader.o: $(INC_DIR)/reader.hpp 
protocol.o: $(INC_DIR)/protocol.hpp 
tcp_client.o: $(INC_DIR)/tcp_client.hpp 
client_manager.o: $(INC_DIR)/client_manager.hpp


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
