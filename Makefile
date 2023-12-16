CFLAGS = -std=c++17 # release: -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi
VulkanTest: main.cpp
	g++ $(CFLAGS) -o Spock main.cpp $(LDFLAGS)

.PHONY: test clean

test: Spock
	./Spock

clean:
	rm Spock
