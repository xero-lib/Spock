CFLAGS = -std=c++17 # release: -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
Spock: *.cpp *.hpp
	g++ $(CFLAGS) -o Spock *.cpp $(LDFLAGS)

.PHONY: test clean

test: Spock
	./Spock

clean:
	rm Spock
