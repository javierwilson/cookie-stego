#!/usr/bin/env python

import os
from stat import *
import re
import sys


import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

count = 0
maximum = 0



dirname = sys.argv[1]
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
            if(not re.search("steg",filename)):
                #print f
                command="steghide embed -cf "+root + "/" +f+" -sf "+root + "/" +"stehide."+f+" -ef secret.txt -p toto -f"
                print "%d %s" %(count,command)
                os.system(command)

            else:
                pass

                   

