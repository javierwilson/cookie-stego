PROGNAME=test_matrix test_mod_rmf extract_dct

all: matrix.o mod_rmf.o libmod_rmf.so $(PROGNAME)

matrix.o: matrix.c matrix.h
	gcc -fPIC -c $<

test_matrix.o: test_matrix.c
	gcc -fPIC -c $<

mod_rmf.o: mod_rmf.c
	gcc -fPIC -c $<

#mod_rmf.shared.o: mod_rmf.c
#	gcc -Wall -fPIC -o $@ -c $<

#matrix.shared.o: matrix.c
#	gcc -Wall -fPIC -o $@ -c $<

libmod_rmf.so: mod_rmf.o matrix.o
	gcc -shared -Wl,-soname,$@ -o $@ $^ -lc

extract_dct.o: extract_dct.c
	gcc -ljpeg -c $<

extract_dct: extract_dct.o mod_rmf.o matrix.o
	gcc -ljpeg -o $@ $^

test_mod_rmf.o: test_mod_rmf.c
	gcc -c $<

test_mod_rmf: test_mod_rmf.o mod_rmf.o matrix.o
	gcc -o $@ $^

test_matrix: test_matrix.o matrix.o
	gcc -o $@ $^

clean:
	rm -rf *.o *.so *~ $(PROGNAME) \#*
