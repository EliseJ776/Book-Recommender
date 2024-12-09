all: main.out

run: main.out
	./main.out

main.out: main.cpp BookRecommender.cpp 
	g++ -Wall main.cpp BookRecommender.cpp -o main.out

clean:
	rm -f main.out