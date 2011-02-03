#!/usr/bin/env python

# launch this program like that:
# LD_LIBRARY_PATH=. ./test_proba_diff.py


from ctypes import *

proba_diff = cdll.LoadLibrary("libproba_diff.so")
#libc = cdll.LoadLibrary("libc.so.6")

dct = [ 
    [ 45, 8, 6, 4, 2, 0, 0, 0 ],
    [ 4, 4, 2, 1, 0, 0, 0, 0 ],
    [ 3, 3, 1, 1, 0, 0, 0, 0 ],
    [ 2, 2, 0, 0, 0, 0, 0, 0 ],
    [ 1, 1, 0, 0, 0, 0, 0, 0 ],
    [ 0, 0, 0, 0, 0, 0, 0, 0 ],
    [ 0, 0, 0, 0, 0, 0, 0, 0 ],
    [ 0, 0, 0, 0, 0, 0, 0, 0 ]]


array_int_64 = c_int * 64  
array_float_324 = c_float * 324

dct_lin = array_int_64()

for j in range(0,8):
    for i in range(0,8):
        dct_lin[j*8+i] = c_int(dct[j][i])


nb_features = 9*9*4
features = array_float_324()

proba_diff.hello_module()
#proba_diff.test_param(pointer(dct_lin))
proba_diff.do_extraction(pointer(dct_lin), 8,8, pointer(features), nb_features)
