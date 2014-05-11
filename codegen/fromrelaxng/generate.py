# author: Andreas Vox, May 2012

import os
import sys

from xml.etree.ElementTree import ElementTree
from Cheetah.Template import Template

from impldescription import *
from parserng import*

class Generator(object):
    
    def __init__(self, templdir, odir):
        self._templatedir = templdir
        self.outdir = odir
        self._headerfiles = []
        self._sourcefiles = []
        self._templateSets = {}
        
    def getfilename(self, fname):
        if fname[-2:] == ".h" or fname[-2:] == ".H" or fname[-3:] == ".hh" or fname[-4:] == ".hxx":
            self._headerfiles.append(fname)
        elif fname[-2:] == ".c" or fname[-2:] == ".C" or fname[-4:] == ".cpp" or fname[-4:] == ".cxx":
            self._sourcefiles.append(fname)
        return os.path.join(self.outdir, fname)
        
    def writefilelist(self, prefix, outfile):
        fname = os.path.join(self.outdir, outfile);
        
        oldlist = []
        if os.path.exists(fname):
            of = open(fname, "r")
            oldlist = of.readlines();
            of.close()
            
        newlist = ["# Generated file, do not edit!\n"]
        newlist.append("SET(%s_HEADERS\n" % prefix)
        for h in self._headerfiles:
            newlist.append("\t%s\n" % h)
        newlist.append(")\n");
        newlist.append("SET(%s_SOURCES\n" % prefix)
        for s in self._sourcefiles:
            newlist.append("\t%s\n" % s)
        newlist.append(")\n")
        
        if (oldlist != newlist):
            nf = open(fname, "w")
            nf.writelines(newlist)
            nf.close()
        
    def generate(self, classDesc):
        print classDesc.xmlName, ":", classDesc.datatype, classDesc.name, "=", classDesc.template
        name = classDesc.template
        if name not in self._templateSets:
            self._templateSets[name] = TemplateSet(self._templatedir, name)
            
        self._templateSets[name].generate(classDesc, self)
        
        
class TemplateSet(object):
    def __init__(self, dir, name):
        self.name = name
        self.templates = []
        for f in os.listdir(dir):
            if f[:len(name)] == name and f[-5:] == ".tmpl":
                fpath = os.path.join(dir, f)
                pair = (Template(file=fpath), f[len(name):-5])
                self.templates.append(pair)
                
    def generate(self, classDesc, generator):
        if len(self.templates) == 0:
            raise AssertionError("No '%s' templates found" % self.name)
        if (self.name != classDesc.template):
            raise NameError("Template names don't match: %s vs. %s" % (self.name, classDesc.template))
        for (t,n) in self.templates:
            t.c = classDesc
            fname = generator.getfilename(classDesc.name + n)
            oldcontent = "";
            if os.path.exists(fname):
                oldf = open(fname, "r")
                oldcontent = oldf.read();
                oldf.close()
            newcontent = str(t)
            if oldcontent != newcontent:
                print "\t>>", fname
                s = open(fname, "w")
                s.write(newcontent)
                s.close()
            else:
                print "\tskipping:", fname
                
        
def Main(file, templdir, outdir):
    spec = ElementTree();
    spec.parse(file);
    
    parser = RNG_Parser(os.path.dirname(file))
    parser.parsePattern(spec.getroot(), False, False)
    
    generator = Generator(templdir, outdir)
    
    for c in parser.classes:
        parser.imports[c.name] = c.name + ".h"
    for c in parser.classes:
        c.addIncludes(parser.imports, parser.refnames)
        generator.generate(c)
        
    generator.writefilelist("SCRIBUS_RELAXNG", "generatedfiles.cmake")
        
rngfile = sys.argv[1]
if rngfile[-4:] == ".rnc":
    rncfile = rngfile
    rngfile = rngfile[:-4] + ".rng"
    cwd = os.path.dirname(sys.argv[0])
    err = os.system("java -jar %s/../trang/trang.jar %s %s" % (cwd, rncfile, rngfile))
    if err != 0:
        print "aborting"
        exit(err)
        
templatedir = os.path.dirname(rngfile)
outdir = templatedir
if len(sys.argv) > 2:
    templatedir = sys.argv[2]
if len(sys.argv) > 3:
    outdir = sys.argv[3]
Main(rngfile, templatedir, outdir);

