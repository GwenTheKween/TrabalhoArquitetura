COMP = g++
COMP_FLAGS = -c -g
LINK_FLAGS = -lpanel -lncurses -o
EXE = scoreboarding.out
SRC = $(wildcard *.cpp)
OBJS = $(SRC:.cpp=.o)
DEPS = $(SRC:.cpp=.d)

$(EXE): $(OBJS) $(DEPS)
	$(COMP) $(OBJS) $(LINK_FLAGS) $(EXE) 

clean:
	rm $(OBJS) $(DEPS) $(EXE)

%.d:%.cpp
	$(COMP) -MM $< -o $@

%.o:%.cpp
	$(COMP) $< $(COMP_FLAGS) -o $@

include $(DEPS)

run: $(EXE)
	@./$(EXE)

debug: clean all
	gdb ./$(EXE)
