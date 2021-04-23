
all: gpio
	
%: %.cpp 
	g++-9.1 -std=c++17 -Wall -pedantic $^ -o $@ -lwiringPi

clean:
	rm -rf gpio