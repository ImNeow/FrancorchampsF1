in = main.c
out = f1

main: build run

build:
	gcc $(in) -o $(out)
run:
	./$(out)
clean:
	rm $(out)
