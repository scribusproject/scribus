import os
import re
import MachO
from distutils.dir_util import copy_tree
from datetime import datetime
	

def findDependencies(exeFiles, exePath):
	"Return a dictionary of MachO.Fixes of all recursive dependencies"
	result = {}
	
	# allow some sloppyness:
	if isinstance(exeFiles, str):
		exeFiles = [MachO.Executable(exeFiles, "executable")]
	elif isinstance(exeFiles, MachO.Executable):
		exeFiles = [exeFiles]
		
	# go through executables and store Fixes		
	todo = [x for x in exeFiles]
	done = [x.Location for x in exeFiles]
	while len(todo) > 0:
		current = todo.pop()
		print "getting dependencies for " + current.Location
		for dep in current.getDependencies():
			if dep.Link not in result:
				if dep.findLocation(exePath):
					result[dep.Link] = dep
					# check if we need to traverse the referenced lib
					if not dep.isSystem() and dep.Location not in done:
						print "- adding " + dep.Location
						done.append(dep.Location)
						todo.append(MachO.Executable(dep.Location, "lib"))
				else:
					print ("couldn't find " + dep.Link + 
							" -> " + dep.Location)

	# forget any system dependencies
	for k,fix in result.items():
		if fix.isSystem():
			del result[k]
						
	return result
	
	
def ingest(bundle, exceptions=[], strippedFrameworks=[]):
	"Moves all needed non-System libraries inside the bundle and fixes links"
	# step 1: find all executables
	executables = MachO.findExecutables(bundle)
	# find the bundle executable
	pat = re.compile("executable")
	exePath = ""
	for exe in executables:
		if pat.match(exe.Kind):
			exePath = os.path.dirname(exe.Location)
			print "using @executable_path=" + exePath
			break
	# step 2: find all dependencies
	fixes = findDependencies(executables, exePath)
	# step 3: move all libraries which are not excepted
	log = []
	frameworks = os.path.join(bundle, "Contents/Frameworks")
	if not os.path.exists(frameworks):
		log.append(">> mkdir " + frameworks)
		os.makedirs(frameworks, 0755)
	for k,fix in fixes.items():
		if fix.Location in exceptions or fix.Link in exceptions:
			del fixes[k]
		else:
			stripFW = fix.Location in strippedFrameworks
			executables.extend(fix.moveLibrary(bundle, stripFW, log))

	# step 3.5: copy aspell dictionaries, hacked for aspell via macports for now, #7371
	aspellsrcpath = "/opt/local/share/aspell"
	if os.path.exists(aspellsrcpath):
		aspelldestpath = os.path.join(bundle, "Contents/share/aspell")
		if not os.path.exists(aspelldestpath):
			log.append(">> mkdir " + aspelldestpath)
			os.makedirs(aspelldestpath, 0755)      
		if os.path.exists(aspelldestpath):
			log.append(">> copying aspell dictionaries")
			print "copying aspell dictionaries"
			copy_tree(aspellsrcpath, aspelldestpath)

	# step 4: fix all executables
	for exe in executables:
		exe.applyFixes(fixes, log)
	# step 5: write log
	logfile = file(os.path.join(bundle, "Contents/osxtools.log"), "a")
	logfile.write("ingest at " + datetime.now().isoformat(" ") + "\n")
	for e in log:
		logfile.write(e + "\n")
	logfile.close()
		

def createSymlinks(bundle, links):
	currDir = os.getcwd()
	for lnk,tar in links:
		print "chdir to " + os.path.join(bundle, os.path.dirname(lnk))
		os.chdir(os.path.join(bundle, os.path.dirname(lnk)))
		print "symlink " + os.path.basename(lnk) + " -> " + tar
		os.symlink(tar, os.path.basename(lnk))
	os.chdir(currDir)
	
	
def relinkOld(FILE, LIBDIR, INSTALLDIR):
	#LIBS=`otool -L $FILE | sed 's/\([^(]*\)(.*)/\1/g'`
	#for LIB in $LIBS ; do
	#	LNAM=`basename $LIB`
	#	if [ $FILE -ef $LIBDIR/$LNAM ] ; then
	#		install_name_tool -id $INSTALLDIR$LNAM $FILE
	#	elif [ -e $LIBDIR/$LNAM ] ; then
	#		install_name_tool -change $LIB $INSTALLDIR$LNAM $FILE
	pass

