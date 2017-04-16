test1: hw2-1.c
	gcc hw2-1.c -o test1

test2: hw2-2.c
	gcc hw2-2.c -o test2

test3: hw2-3.c
	gcc hw2-3.c -o test3

test4: hw2-4.c
	gcc hw2-4.c -o test4   

test5: hw2-5.c
	gcc hw2-5.c -o test5

run1: test1
	./test1 >> output.txt
	diff src.dummy dest1.dummy
	rm -rf dest1.dummy

run2: test2
	./test2 >> output.txt
	diff src.dummy dest2.dummy
	rm -rf dest2.dummy

run3: test3
	./test3 >> output.txt
	diff src.dummy dest3.dummy
	rm -rf dest3.dummy

run4: test4
	./test4 >> output.txt
	diff src.dummy dest4.dummy
	rm -rf dest4.dummy

run5: test5
	./test5 >> output.txt
	diff src.dummy dest5.dummy
	rm -rf dest5.dummy



