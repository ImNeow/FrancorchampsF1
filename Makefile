in = main.c
out = f1

main: build run

build:
	gcc -O3 $(in) -o $(out)
run:
	./$(out)
clean:
	rm $(out)
