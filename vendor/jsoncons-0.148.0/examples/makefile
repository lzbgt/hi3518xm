TARGET = app

INCLUDE=../include
SRCS  = $(shell find ./src     -type f -name "*.cpp")
HEADS = $(shell find ${INCLUDE} -type f -name "*.h")
OBJS = $(SRCS:.cpp=.o)
DEPS = Makefile.depend

INCLUDES = -I${INCLUDE}
CXXFLAGS = -O2 -Wall $(INCLUDES) -std=c++11
LDFLAGS=


all: $(TARGET)

$(TARGET): $(OBJS) $(HEADS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

run: all
	@./$(TARGET)

.PHONY: depend clean
depend:
	$(CXX) $(INCLUDES) -MM $(SRCS) > $(DEPS)
	@sed -i -E "s/^(.+?).o: ([^ ]+?)\1/\2\1.o: \2\1/g" $(DEPS)

clean:
	$(RM) $(OBJS) $(TARGET)

-include $(DEPS)