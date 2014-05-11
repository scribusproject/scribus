# author: Andreas Vox, May 2012

from xml.etree.ElementTree import ElementTree

# our namespaces:
_namespaces = {}
_namespaces[''] = "http://relaxng.org/ns/structure/1.0"
_namespaces['rng'] = "http://relaxng.org/ns/structure/1.0"
_namespaces['a'] = "http://relaxng.org/ns/compatibility/annotations/1.0"
_namespaces['impl'] = "http://www.scribus.info/RelaxNG/implementation"

def qn(name):
    if name.find(":") >= 0:
        prefix,n = name.split(":")
        return "{%s}%s" % (_namespaces[prefix], n)
    else:
        # use the default namespace
        return "{%s}%s" % (_namespaces[''], name)

def an(name):
    if name.find(":") >= 0:
        prefix,n = name.split(":")
        ns = _namespaces[prefix]
        # special case: for attributes we don't include the default namespace
        if ns == _namespaces['']:
            return n
        else:
            return "{%s}%s" % (ns, n)
    else:
        return name

# helper
def pick(a, b):
    if a is None:
        return b
    else:
        return a


# implementation details for a ScribusIO class
class Class(object):
    
    def __init__(self, xml):
        self.xmlName = xml.get(an('name'))
        self.name = xml.get(an('impl:name'), self.xmlName)
        self.template = xml.get(an('impl:template'))
        self.datatype = xml.get(an('impl:datatype'), self.name)
        self.refname = None
        self.properties = []
        self.includes = []
        self.classreferences = []
        self.classincludes = []
    
    def addIncludes(self, datatypes, refnames):
        #print datatypes, refnames
        for p in self.properties:
            if p.datatype == None and p.refname in refnames:
                                p.datatype = refnames[p.refname].datatype
            if p.datatype == None:
                print "X>>>", p.name, p.refname, refnames
            
            if p.datatype.find("<") > 0:
                wrapper,lt,args = p.datatype[:-1].partition("<")
                dts = [wrapper]
                for arg in args.split(","):
                    dts.append(arg.strip());
            else:
                dts = [p.datatype]
            #print "DTS:", p.name, p.datatype, dts
            for dt in dts:
                reference = None
                include = None
                
                if dt[-1] == "&" or dt[-1] == "*":
                    # pointer only needs reference in header file
                    dt = dt[:-1]
                    if dt not in ['int', 'long', 'uint', 'bool', 'double', 'float']:
                        reference = dt
                        if dt in datatypes:
                            include = datatypes[dt]
                elif dt in datatypes:
                    # full type needs include
                    include = datatypes[dt]
                elif dt[0:4] == "Qt::":
                    # Qt global enums
                    include = "<Qt>"
                elif dt[0] == "Q":
                    # Qt types are easy
                    include = "<" + dt + ">"
                elif dt not in ['int', 'long', 'uint', 'bool', 'double', 'float']:
                    # this might not be enough:
                    reference = dt
                
                if include != None and include[0] != "<" and include[0] != '"':
                    include = '"' + include + '"'

                if reference != None:
                    if reference not in self.classreferences:
                        self.classreferences.append(reference)
                        if include != None and include not in self.classincludes and include not in self.includes:
                            self.classincludes.append(include)
                else:
                    if include != None and include not in self.includes:
                        self.includes.append(include)
                        if include in self.classincludes:
                            self.classincludes.remove(include)
            
                
        
#implementation details for a property of a ScribusIO class
class Property(object):

    dataTypes = {'integer': "int", 'boolean': "bool", 'decimal': "double" , 'string': "QString" }
    
    def __init__(self, xml, optional, isCollection = False):
        self.xmlName = xml.get(an('name'))
        self.name = xml.get(an('impl:name'), self.xmlName)
        self.optional = xml.get(an('impl:optional'), optional)
        self.type = xml.get(an('impl:type'), "coll" if isCollection else "value") # value, struct, collection
        self.datatype = xml.get(an('impl:datatype'))
        self.refname = None
        self.fromString = xml.get(an('impl:fromstring'))
        self.toString = xml.get(an('impl:tostring'))
        self.getter = xml.get(an('impl:getter'), "get" +self.name)
        self.setter = xml.get(an('impl:setter'), "set" + self.name[0:1].capitalize() + self.name[1:])
        self.resetter = xml.get(an('impl:resetter'), "reset" + self.name[0:1].capitalize() + self.name[1:])
        self.initial = xml.get(an('impl:initial'))
        
        self.builder = xml.get(an('impl:builder'))
        if self.builder is not None:
            args = self.builder.split(";")
            self.builder = args[0]
            self.builderargs = []
            for i in range(1, len(args)):
                arg = args[i]
                p = arg.find('|')
                comma = "," if i < len(args)-1 else ""
                if p >= 0:
                    p2 = arg.rfind('|')
                    triple = (arg[:p] + '(', arg[p+1: p2], ')' + arg[p2+1:] + comma)
                else:
                    triple = ("", arg, comma)
                self.builderargs.append(triple)
        
        if self.datatype is None:
            dt = xml.find(qn('rng:data'))
            if dt != None:
                xsltype = dt.get(an('type'))
                self.datatype =  xsltype if xsltype not in self.dataTypes else self.dataTypes[xsltype]
                
                
