#!/usr/bin/env python

# Copyright (c) 2013 Google Inc. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
import os
import sys
import time
import shutil
#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_BUILD 0
#define VERSION_QFE 0
#define VERSION_BUILD_DATE "24/05/2005"
#define VERSION_BUILD_TIME "08:37:55UTC"
def copytree(src, dst, symlinks=False, ignore=None):
    for item in os.listdir(src):
        s = os.path.join(src, item)
        d = os.path.join(dst, item)
        if os.path.isdir(s):
            shutil.copytree(s, d, symlinks, ignore)
        else:
            shutil.copy2(s, d)
            
            
outfile = sys.argv[1]
# Why is the " exiting at tail?
outDir = os.path.abspath(sys.argv[2])[0:-1]
print 'The result directory is: ', outDir

versionFile = open(outfile, 'w')
versionFile.write('#define VERSION_MAJOR 1\n')
versionFile.write('#define VERSION_MINOR 0\n')
versionFile.write('#define VERSION_BUILD 0\n')
versionFile.write('#define VERSION_QFE 0\n')
versionFile.write('#define VERSION_BUILD_DATE \"%s\"\n' % time.strftime("%Y/%m/%d"))
versionFile.write('#define VERSION_BUILD_TIME \"%s\"\n' % time.strftime("%H:%M:%S"))
versionFile.write('#define VERSION_REVISION \"%s\"\n' % os.popen("git log --pretty=format:'%h' -n 1").read()[1:-1])

# copy file to $(OutDir)
resDir = os.path.join(outDir, 'Resources')
if os.path.isdir(resDir):
    print 'Removing resource directory', resDir
    shutil.rmtree(resDir)

print 'Copying Cef resources files to ', resDir
shutil.copytree('../cef_binary_3.2078.1780_windows32/Resources',  resDir)
shutil.copy('../cef_binary_3.2078.1780_windows32/Resources/icudtl.dat', resDir)

print 'Copying Cef dlls to ', outDir
if outDir.endswith('Debug'):
    copytree('../cef_binary_3.2078.1780_windows32/Debug/', outDir)
else:
    copytree('../cef_binary_3.2078.1780_windows32/Release/', outDir)

# emotion    
emotionDir = os.path.join(outDir, 'Resources\\emotion')
if os.path.isdir(emotionDir):
    shutil.rmtree(emotionDir)
print 'Copying emotion files to ', emotionDir
shutil.copytree('../Messenger/skin/emotion', emotionDir)
# css 
cssDir = os.path.join(outDir, 'Resources\\css')
if os.path.isdir(cssDir):
    shutil.rmtree(cssDir)
print 'Copying css files to ', cssDir
shutil.copytree('../Messenger/skin/css/', cssDir)
# media
mediaDir = os.path.join(outDir, 'Resources\\media')
if os.path.isdir(mediaDir):
    shutil.rmtree(mediaDir)
shutil.copytree('../Messenger/media/', mediaDir)