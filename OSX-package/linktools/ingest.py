#!/usr/bin/python

import os
import sys
import osxtools

def usage():
	print """
 Usage: ingest.py bundle [-x lib] [-s fw]
 
	Copies all dependent libraries and frameworks into the app bundle.
	System libraries (/usr/lib*, /System/Library) are not copied.
	Fixes the dependencies in all executabels contained in bundle.
	
	bundle:	the path to the *.app bundle
	-x lib:	dont move lib into the bundle. 
	-s fw:	only move the referenced libarry file from framework fw 
			into the bundle, not the complete framework
	"""


if len(sys.argv) <= 1 or sys.argv[1] == "-?" :
	usage()
	sys.exit(0)



exceptions = []
strippedfws = []
bundle = None

argp = 1

while argp < len(sys.argv) :
	if sys.argv[argp] == '-x' :
		exceptions.append(sys.argv[argp + 1])
		argp = argp + 2
	elif sys.argv[argp][0:2] == '-x' :
		exceptions.append(sys.argv[argp][2:])
		argp = argp + 1
	elif sys.argv[argp] == '-s' :
		strippedfws.append(sys.argv[argp + 1])
		argp = argp + 2
	elif  sys.argv[argp][0:2] == '-s' :
		strippedfws.append(sys.argv[argp][2:])
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
	
if not os.path.isdir(bundle):
	print "Error: '" + bundle + "' is no bundle path!"
	usage()
	sys.exit(1)

osxtools.ingest(bundle, exceptions, strippedfws)
