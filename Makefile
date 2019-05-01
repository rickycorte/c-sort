build: clean
	cc -g -o sort src/sort.c src/sort.h src/main.c
	chmod +x sort 
	mkdir build
	mv sort build/sort

run: build
	./build/sort

clean:
	rm -rf build