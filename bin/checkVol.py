#!/usr/bin/python

import sys

Volume = sys.argv[1]
Volume = float(Volume)
if Volume  > .12:
    print "1"
else:
    print "0"
