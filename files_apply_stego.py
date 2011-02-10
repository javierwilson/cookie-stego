#!/usr/bin/env python

import os
from stat import *
import re
import sys


#import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

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
    if(sys.argv[i]=="--steghide"):
        do_steghide = True
        print "do_steghide"
    if(sys.argv[i]=="--outguess"):
        do_outguess = True
        print "do_outguess"
    if(sys.argv[i]=="--book"):
        pass

dirname = os.path.abspath(dirname)

for root,dirs,files in os.walk(dirname):
    #print root
    #print dirs
    #print files

    for f in files:
        filename = root + "/" + f
        count = count + 1
        if((re.search("JPG$", filename)!=None)
           or (re.search("jpg$", filename)!=None)):
            if(not re.search("steg",filename)):
                if(do_steghide):
                    command="steghide embed -cf "+root + "/" +f+" -sf "+root + "/" +"steg.steghide."+f+" -ef secret.txt -p toto -f"
                    print "%d %s" %(count,command)
                    os.system(command)

                if(do_outguess):
                    command="outguess -k \"toto\" -d secret.txt "+root + "/" +f+" -sf "+root + "/" +"steg.outguess."+f
                    print "%d %s" %(count,command)
                    os.system(command)

            else:
                pass

                   

