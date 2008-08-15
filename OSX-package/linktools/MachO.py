import os
import re
import shutil


def findFramework(path, name = None):
	"find the framework folder for FW name"
	if path == "" or path == "@executable_path" or path == "/":
		return None
	elif name == None:
		return findFramework(os.path.dirname(path), 
							 os.path.basename(path))
	elif os.path.basename(path) == name + ".framework":
		return path
	elif len(os.path.dirname(path)) >= len(path):
		print "MachO.findFramework: Oops '" + path + "', '" + name + "'"
		return None
	else:
		return findFramework(os.path.dirname(path), name)
	


def stripPrefix(prefix, path):
	"Returns the relative path r such that os.path.join(prefix, r) == path"
	prefix = os.path.normpath(prefix)
	prefixLen = len(prefix)
	path = os.path.normpath(path)
	if path[0 : prefixLen] == prefix:
		if path[prefixLen] == os.sep:
			return path[prefixLen+1 : ]
		else:
			return path[prefixLen : ]
	else:
		return path
		
		
class Executable:
	"Represents an Mach-O executable."	
	
	def __init__(self, path, kind):
		self.Location = path
		self.Kind = kind
		
		
	def __repr__(self):
		return self.Location + " (" + self.Kind + ")"
		
		
	def getDependencies(self):
		"Return a list of MachO.Fixes describing the dependencies."
		"Uses otool -L"
		f = os.popen("otool -L " + self.Location, "r")
		result = []
		pat = re.compile("\s*([^(]*)\s\((.+)\)")
		for line in f:
			m = pat.match(line)
			if m != None:
				result.append(Fix(m.group(1), m.group(2)))
		status = f.close()
		return result
		
	def applyFixes(self, changes, log):
		"Uses install_name_tool to change the links to dependencies."
		"changes is a dictionary mapping links (as strings) to Fixes."
		args = ""
		for dep in self.getDependencies():
			if dep.Link in changes:
				args = args + changes[dep.Link].getChange()
		log.append(">> " + "install_name_tool " + args + self.Location)
		if len(args) > 0:
			os.system("install_name_tool " + args + self.Location)
		pat = re.compile("(library)|(universal binary)")
		if pat.search(self.Kind):
			relName = os.path.basename(self.Location) # FIXME: rel to fw
			log.append(">> " + "install_name_tool -id " + relName + 
						" " + self.Location)
			os.system("install_name_tool -id " + relName + 
						" " + self.Location)


def findExecutables(bundleDir):
	"Return a list of MachO.Executables found in bundleDir"
	result = []
	pat = re.compile("Mach-O (.+)")
	for root, dirs, files in os.walk(bundleDir):
		for n in files:
			p = os.path.join(root, n)
			f = os.popen("file -b " + p, "r")
			m = pat.match(f.readline())
			if m != None:
				result.append(Executable(p, m.group(1)))
				print "found " + m.group(1) + ": " + n
			f.close()
	return result



class Fix:
	"Represents a fix for a library link."
	
	def __init__(self, dependency, versionString="?"):
		self.Link = dependency
		self.Location = dependency
		self.NewLink = dependency
		self.NewLocation = dependency
		self.versionString = versionString # not used yet
		self.fwPath = None
		self.relPath = None
		
	def __repr__(self):
		return (self.Link + " (" + self.versionString + ")")
		
	def isAbsolute(self):
		return os.path.isabs(self.Link)
		
	def isBundleRelative(self):
		return self.Link[0:17] == "@executable_path/"
		
	def isSystem(self):
		return (self.Location[0:8] == "/usr/lib"     # also matches libexec
				or self.Location[0:8] == "/usr/X11"  # also matches X11R6
				or self.Location[0:8] == "/System/")
				
	def getChange(self):
		"Returns argument for install_name_tool."
		if self.Link == self.NewLink:
			return ""
		else:
			return "-change " + self.Link + " " + self.NewLink + " "
		
	def findLocation(self, exePath=None):
		if self.isBundleRelative():
			if exePath != None:
				self.Location = os.path.normpath(
								os.path.join(exePath, self.Link[17:]))
			else:
				self.Location = self.Link[17:]
		else:
			self.Location = self.Link

		# check if done
		if (os.path.isabs(self.Location) and
			os.path.isfile(self.Location)):
			self.NewLocation = self.Location
			return True
		
		# search for frameworks in /System/Library and /Library
		fwPath = findFramework(self.Location)
		if fwPath:
			fwdir = os.path.dirname(fwPath)
			self.relPath = stripPrefix(fwdir, self.Location)
			for d in	["/Library/Frameworks", 
						"/System/Library/Frameworks"]:
				if os.path.isfile(os.path.join(d, self.relPath)):
#					self.Location = os.path.join(d, self.relPath)
					self.Location = os.path.join(d, self.relPath)
					self.NewLocation = self.Location
					self.fwPath = os.path.join(d, os.path.basename(fwPath))
					self.relPath = stripPrefix(self.fwPath, self.Location)
					return True
		
		# ok, try libs
		lib = os.path.basename(self.Location)
		self.relPath = None
		for d in	["/usr/local/lib", "/opt/local/lib", 
					"/usr/lib", "/opt/lib"]:
			if os.path.isfile(os.path.join(d, lib)):
				self.Location = os.path.join(d, lib)
				self.NewLocation = self.Location
				return True

		# not found
		return False
		
		
	def moveLibrary(self, destBundlePath, stripFW, log):
		"Copies the library or fw to destBundlePath."
		"Also sets NewLink and NewLocation properties"
		"Returns a list of copied executables"
		
		# dont do this if we are already inside the bundle:
		if stripPrefix(destBundlePath, self.Location) != self.Location:
			log.append("-- ignoring " + self.Location)
			return []
		
		if self.relPath != None and not stripFW:
			# copy framework
			newFwPath = os.path.join(destBundlePath, 
										"Contents/Frameworks", 
										os.path.basename(self.fwPath))
			log.append(">> " + self.fwPath + " ===> " + newFwPath)
			if (os.path.exists(destBundlePath) and
				not os.path.exists(newFwPath)):
				shutil.copytree(self.fwPath, newFwPath, True)
			self.NewLocation = os.path.join(newFwPath, self.relPath)
			self.NewLink = ("@executable_path/" 
							+ os.path.join("../Frameworks", 
											os.path.basename(self.fwPath),
											self.relPath))
			return findExecutables(newFwPath)
		else:
			# copy lib to bundle.app/Contents/Frameworks/
			self.NewLocation = os.path.join(destBundlePath, 
											"Contents/Frameworks",
											os.path.basename(self.Location))
			self.NewLink = ("@executable_path/" 
							+ os.path.join("../Frameworks", 
											os.path.basename(self.Location)))
			log.append(">> " + self.Location + " ---> " + self.NewLocation)
			if (os.path.exists(destBundlePath) and
				not os.path.exists(self.NewLocation)):
				shutil.copy(self.Location, self.NewLocation)
			return [Executable(self.NewLocation, "lib")]
			
	