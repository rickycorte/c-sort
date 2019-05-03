build: clean
	cc -g -o sort src/sort.c src/sort.h src/main.c
	chmod +x sort 
	mkdir build
	mv sort build/sort
	cp data/test_data.txt build/test_data.txt

run: build
	cd build && ./sort

clean:
	rm -rf build