#!/usr/bin/env python

import os
from stat import *
import re
import sys

count = 0
maximum = 0

dirname="."
do_steghide=False
do_outguess=False

for i in range(1,len(sys.argv)):
    if(sys.argv[i]=="--dir"):
        if(i+1>=len(sys.argv)):
            print "error, --dir needs an argument"
            sys.exit(0)
        dirname = sys.argv[i+1]
        i = i + 1
        print "directory : " + dirname
    if(sys.argv[i]=="--book"):
        pass

dirname = os.path.abspath(dirname)

for root,dirs,files in os.walk(dirname):
#    print root
#    print dirs
#    print files

    for f in files:
        filename = root + "/" + f
        count = count + 1
        if((re.search("JPG$", filename)!=None)
           or (re.search("jpg$", filename)!=None)):
            if(re.search("steg",filename)!=None):
                command="./cookie --file="+root + "/" +f+" --module=rmf --svm=rmf.ds --steg"
            else:
                command="./cookie --file="+root + "/" +f+" --module=rmf --svm=rmf.ds"
            print "%d %s" %(count,command)
            os.system(command)
