TARGETS := test
HEADERS := hashtableAwesome.h
OBJS := hashtableAwesome.o testCPPHash.o

BUILDFLAGS := -std=c++11

all: $(TARGETS)

%.o: %.cpp $(HEADERS)
	g++ -c $(BUILDFLAGS) -o $@ $<

test: $(OBJS)
	g++ -o $@ $^

clean:
	rm -rf *~ $(OBJS) $(TARGETS)
