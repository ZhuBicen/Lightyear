#!/usr/bin/env python

# Copyright (c) 2013 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
import os
import sys
import time
import shutil
import zipfile
from time import localtime, strftime

            
# Why is the " exiting at tail?
outDir = os.path.abspath(sys.argv[1])[0:-1]
print 'The result directory is: ', outDir


# to archive Resource/, *.dll, *.dat, and messenger.exe
zipName = "messenger_%s.zip" % strftime("%Y-%m-%d-%H-%M-%S", localtime())
print 'Creating ', zipName, ' !!!'
zf = zipfile.ZipFile(zipName, "w")

# zip Resource
resDir = os.path.join(outDir, "Resources")
absResDir = os.path.abspath(resDir)
for dirname, subdirs, files in os.walk(resDir):
    for filename in files:
        absname = os.path.abspath(os.path.join(dirname, filename))
        arcname = "Resources/" + absname[len(absResDir) + 1:]
        print 'zipping %s => %s' % (os.path.join(dirname, filename), arcname)
        zf.write(absname, arcname)
        
for file in os.listdir(outDir):
    if file == "Messenger.exe" or file == "icudtl.dat" or file == "libcef.dll": #file.endswith(".dll"):
        print 'zipping %s => %s' % (os.path.abspath(os.path.join(outDir, file)), file)
        zf.write(os.path.abspath(os.path.join(outDir, file)), file)
        

zf.close()
print zipName, " done."    
    
