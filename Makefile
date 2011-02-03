PROGNAME=test_matrix test_proba_diff extract_dct

all: matrix.o proba_diff.o libproba_diff.so $(PROGNAME)

matrix.o: matrix.c matrix.h
	gcc -fPIC -c $<

test_matrix.o: test_matrix.c
	gcc -fPIC -c $<

proba_diff.o: proba_diff.c
	gcc -fPIC -c $<

#proba_diff.shared.o: proba_diff.c
#	gcc -Wall -fPIC -o $@ -c $<

#matrix.shared.o: matrix.c
#	gcc -Wall -fPIC -o $@ -c $<

libproba_diff.so: proba_diff.o matrix.o
	gcc -shared -Wl,-soname,$@ -o $@ $^ -lc

extract_dct.o: extract_dct.c
	gcc -ljpeg -c $<

extract_dct: extract_dct.o proba_diff.o matrix.o
	gcc -ljpeg -o $@ $^

test_proba_diff.o: test_proba_diff.c
	gcc -c $<

test_proba_diff: test_proba_diff.o proba_diff.o matrix.o
	gcc -o $@ $^

test_matrix: test_matrix.o matrix.o
	gcc -o $@ $^

clean:
	rm -rf *.o *.so *~ $(PROGNAME) \#*
