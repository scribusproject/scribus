#!/usr/bin/python

import os
import sys
import shutil
import osxtools

def usage():
	print """
 Usage: mkframework.py bundle [-l libfile] [-v version]
 
	Creates the directory structure for a framework.
	If libfile is given, it will be used as the frameworks executable,
	otherwise the framework will just be an empty shell.
		
	bundle:	the path to the *.framework bundle
	-l lib:	copy lib into the bundle. 
	-v ver:	use "ver" as the version instead of the standard 'A'
	-f:     overwrite existing files if version exists
	"""


if len(sys.argv) <= 1 or sys.argv[1] == "-?" :
	usage()
	sys.exit(0)



version = "A"
overwrite = False
libfile = None
bundle = None

argp = 1

while argp < len(sys.argv) :
	if sys.argv[argp] == '-f':
		overwrite = True;
		argp = argp + 1
	elif sys.argv[argp] == '-l' :
		libfile = (sys.argv[argp + 1])
		argp = argp + 2
	elif sys.argv[argp][0:2] == '-l' :
		libfile = (sys.argv[argp][2:])
		argp = argp + 1
	elif sys.argv[argp] == '-v' :
		version = (sys.argv[argp + 1])
		argp = argp + 2
	elif sys.argv[argp][0:2] == '-v' :
		version = (sys.argv[argp][2:])
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
	
if bundle[-10 : ] != ".framework":
	bundle = bundle + ".framework"
fwName = os.path.basename(bundle)[0: -10]
	
if not os.path.exists(bundle):
	os.makedirs(bundle, 0755)
elif not os.path.isdir(bundle):
	print "Error: '" + bundle + "' is no bundle path!"
	usage()
	sys.exit(1)

versionPath = os.path.join(bundle, "Versions", version)

if os.path.exists(versionPath):
	if overwrite:
		shutil.removetree(versionPath)
	else:
		print "Error: '" + versionPath + "' already exists!"
		usage()
		sys.exit(1)

os.makedirs(versionPath, 0755)

if libfile != None:
	shutil.copy(libfile, os.path.join(versionPath, fwName))
	os.system("install_name_tool -id @executable_path/" + 
				os.path.join("../Frameworks",
							fwName + ".framework",
							"Versions",
							version,
							fwName) +
				" " +
				os.path.join(versionPath, fwName))
			
osxtools.createSymlinks(bundle, [
	("Versions/Current", version),
	(fwName, os.path.join("Versions/Current", fwName)),
	("Headers", "Versions/Current/Headers")
])

