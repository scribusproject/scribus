#!/usr/bin/python

import os
import sys
import shutil
import osxtools

def usage():
	print """
 Usage: mkappbundle.py bundle [-b binary] [-i infofile|-v version]
 
	Creates the directory structure for an application bundle.
	If binary is given, it will be used as the binaries executable,
	otherwise the binary will just be an empty shell.
		
	bundle:	the path to the *.app bundle
	-b binary:	copy lib into the bundle. 
	-i infofile:	use "ver" as the version instead of the standard 'A'
	"""


if len(sys.argv) <= 1 or sys.argv[1] == "-?" :
	usage()
	sys.exit(0)



infofile = None
binfile = None
bundle = None

argp = 1

while argp < len(sys.argv) :
	if sys.argv[argp] == '-b' :
		binfile = (sys.argv[argp + 1])
		argp = argp + 2
	elif sys.argv[argp][0:2] == '-b' :
		binfile = (sys.argv[argp][2:])
		argp = argp + 1
	elif sys.argv[argp] == '-i' :
		infofile = (sys.argv[argp + 1])
		argp = argp + 2
	elif sys.argv[argp][0:2] == '-i' :
		infofile = (sys.argv[argp][2:])
		argp = argp + 1
	elif  sys.argv[argp][0:1] == '-' :
		print "Error: unknown option: " + sys.argv[argp]
		usage()
		sys.exit(1)
	elif bundle == None:
		bundle = sys.argv[argp]
		argp = argp + 1
	else:
		print "Error: more than one bundle path specified!"
		usage()
		sys.exit(1)

if bundle == None:	
	print "Error: no bundle path specified!"
	usage()
	sys.exit(1)

if not os.path.isabs(bundle):
	bundle = os.path.join(os.getenv("PWD"), bundle)
	
if bundle[-4 : ] != ".app":
	bundle = bundle + ".app"
appName = os.path.basename(bundle)[0: -4]
	
if not os.path.exists(bundle):
	os.makedirs(bundle, 0755)
elif not os.path.isdir(bundle):
	print "Error: '" + bundle + "' is no bundle path!"
	usage()
	sys.exit(1)

binPath = os.path.join(bundle, "Contents/MacOS")

if not os.path.exists(binPath):
	os.makedirs(binPath, 0755)

if binfile != None:
	shutil.copy(binfile, os.path.join(binPath, appName))
			
shutil.copy(infofile, os.path.join(bundle, "Contents/Info.plist")

