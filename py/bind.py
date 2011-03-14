# Creates the python bindings for libvov-c

import os
import sys
sys.path = ["./py/"] + sys.path
import CppHeaderParser
#CppHeaderParser.debug = True


if __name__ == '__main__':

    headers = []
    for root, dirs, files in os.walk('src/'):
        if root.find('src/gfx') >= 0: continue

        for f in files:
            if f.endswith('.h'):
                headers.append(root+'/'+f)

    print headers
    for hdr in headers:
        #try:
        cppHeader = CppHeaderParser.CppHeader(hdr)

        print cppHeader
        #except CppHeaderParser.CppParseError,  e:
        #    print e
        #    sys.exit(1)

