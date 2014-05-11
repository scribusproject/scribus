# author: Andreas Vox, May 2012

from xml.etree.ElementTree import ElementTree
import os

from impldescription import *

class RNG_Parser(object):
    
    def __init__(self, dir):
        self.basedir = dir
        self.definitions = {}
        self.refnames = {}
        self.imports = {}
        self.classes = []
        self.currentClass = None
        
    def parsePattern(self, pattern, isOptional, isCollection):
        if pattern.tag == qn('element'):
            if pattern.get(an('impl:type')) == "ignore":
                pass;
            elif pattern.get(an('impl:template')) != None:
                c = self.currentClass
                self.currentClass = Class(pattern)
                self.classes.append(self.currentClass)
                self.parsePatterns(pattern.getchildren(), False, False)
                self.currentClass = c
            elif self.currentClass != None:
                newProp = Property(pattern, isOptional, isCollection)
                if newProp.datatype == None:
                    ref = pattern.getchildren()[0]
                    if ref.tag == qn('rng:ref'):
                        newProp.refname = ref.get(an('name'))
                self.currentClass.properties.append(newProp)
                
            else:
                self.parsePatterns(pattern.getchildren(), isOptional, isCollection)

        elif pattern.tag == qn('attribute'):
            if pattern.get(an('impl:type')) == "ignore":
                pass;
            elif self.currentClass != None:
                self.currentClass.properties.append(Property(pattern, isOptional, isCollection))
    
        elif pattern.tag == qn('group'):
            self.parsePatterns(pattern.getchildren(), isOptional, isCollection)
        elif pattern.tag == qn('interleave'):
            self.parsePatterns(pattern.getchildren(), isOptional, isCollection)
        elif pattern.tag == qn('choice'):
            self.parsePatterns(pattern.getchildren(), True, isCollection)
        elif pattern.tag == qn('optional'):
            self.parsePatterns(pattern.getchildren(), True, isCollection)
        elif pattern.tag == qn('zeroOrMore'):
            self.parsePatterns(pattern.getchildren(), isOptional, True)
        elif pattern.tag == qn('oneOrMore'):
            self.parsePatterns(pattern.getchildren(), isOptional, True)
        elif pattern.tag == qn('list'):
            raise NotImplementedError
        elif pattern.tag == qn('mixed'):
            self.parsePatterns(pattern.getchildren(), isOptional, isCollection)
        elif pattern.tag == qn('ref'):
            if pattern.get(an('impl:type')) != "ignore":
                self.parsePatterns(self.definitions[pattern.get(an('name'))], isOptional, isCollection)
        elif pattern.tag == qn('parentRef'):
            if pattern.get(an('impl:type')) != "ignore":
                self.parsePatterns(self.definitions[pattern.get(an('name'))], isOptional, isCollection)
        elif pattern.tag == qn('empty'):
            pass
        elif pattern.tag == qn('text'):
            pass
        elif pattern.tag == qn('value'):
            raise NotImplementedError
        elif pattern.tag == qn('data'):
            raise NotImplementedError
        elif pattern.tag == qn('notAllowed'):
            pass
        elif pattern.tag == qn('externalRef'):
            raise NotImplementedError
        elif pattern.tag == qn('grammar'):
            self.parseGrammar(pattern.getchildren())
        elif pattern.tag == qn('impl:import'):
            self.parseImport(pattern)
        else:
            pass

    def parseImport(self, pattern):
        self.imports[pattern.get(an('impl:datatype'))] = pattern.get(an('impl:headerfile'))

    def parsePatterns(self, patterns, isOptional, isCollection):
        for p in patterns:
            self.parsePattern(p, isOptional, isCollection)
            
    def parseGrammar(self, grammar):
        for rule in grammar:
            if rule.tag == qn('define'):
                self.parseDefine(rule)
            elif rule.tag == qn('start'):
                self.parsePatterns(rule.getchildren(), False, False)
            elif rule.tag == qn('div'):
                self.parseGrammar(rule)
            elif rule.tag == qn('include'):
                xgrammar = ElementTree()
                file = os.path.join(self.basedir, rule.get(an('href')))
                xgrammar.parse(file)                
                self.parseGrammar(xgrammar.getroot())
            elif rule.tag == qn('impl:import'):
                self.parseImport(rule)
            else:
                raise NotImplementedError
                
    def parseDefine(self, rule):
        self.definitions[rule.get(an('name'))] = rule.getchildren()
        if rule.get(an('impl:type')) == "ignore":
            pass
        elif rule.get(an('impl:template')) != None:
            c = self.currentClass
            self.currentClass = Class(rule)
            self.refnames[rule.get(an('name'))] = self.currentClass
            self.classes.append(self.currentClass)
            self.parsePatterns(rule.getchildren(), False, False)
            self.currentClass = c
        else:
            self.parsePatterns(rule.getchildren(), False, False)



def dumpTree(i, tree):
    print i*"  " + tree.tag + "  " + str(tree.attrib) + "  " + repr(tree.text)
    for e in tree.getchildren():
        dumpTree(i+1, e)
    
    
def testParser(file):
    self.spec = ElementTree()
    spec.parse(file)                
    x = RNG_Parser(os.path.dirname(file))
    x.parsePattern(spec.getroot(), False, False)
    for c in x.classes:
        x.imports[c.datatype] = c.name + ".h"
    for c in x.classes:
        c.addIncludes(x.imports, x.refnames)

    for c in x.classes:
        print "%s (%s) TEMPL %s" % (c.name, c.datatype, c.template)
        print "\t#imports", c.includes
        print "\tclass", c.classreferences, ";"
        print "  Properties:"
        for p in c.properties:
            print "\t%s (%s) %s\t|%s|\t INIT %s" % (p.name, p.datatype, "OPT" if p.optional else "", p.type, p.initial)

def testmain():
    import sys
    import os
    rncfile = sys.argv[1]
    rngfile = rncfile[:-4] + ".rng"
    cwd = os.path.dirname(sys.argv[0])
    err = os.system("java -jar %s/../trang/trang.jar %s %s" % (cwd, rncfile, rngfile))
    if err != 0:
        print "aborting"
        exit(err)
        
    testParser(rngfile)
    
#testmain()