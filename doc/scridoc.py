""" Scribus Scripter C++ docstrings vs. HTML documentation checker

It works for functions and procedures only yet.

Disclaimer: this code is very ugly. Don't read it if you want to
keep your brain healthy.

Petr Vanek <petr@scribus.info>
"""

import re
import os
import pydoc
import difflib
import traceback

from pysqlite2 import dbapi2 as sqlite
from scribus import *



class HTMLDocs:
    """ Parse HTML documentation into dictionary
    { 'procedureName' : 'its docstring' }
    """
    def __init__(self):
        self.path = 'en/'
        self.docs = {}
        self.ignore = ['scripterapi-ImageExport.html',
                       'scripterapi-PDFfile.html',
                       'scripterapi-Printer.html',
                       'scripterapi-pydoc.html',
                       'scripterapi-constants.html']

        for i in os.listdir(self.path):
            if i.startswith('scripterapi-') and i not in self.ignore:
                self.parseFile(i)


    def strip(self, txt):
        tmp = re.sub('<.*?>', '', txt)#.replace('&gt;', '>').replace('&lt;', '<').replace('\n', ' ')
        ws = tmp.split()
        return ' '.join(ws)

    def parseFile(self, fname):
        print fname
        f = open(os.path.join(self.path, fname), 'r')
        key = ''
        doc = []
        stateAppend = False
        for line in f:
            if line.find('<dt><a name=') != -1:
                # procedure name found
                key = self.strip(line).replace('(...)', '')
                stateAppend = True
                continue
            if line.find('<dd>') != -1:
                doc.append(self.strip(line))
                stateAppend = True
                continue
            if line.find('</dd>') != -1:
                doc.append(self.strip(line))
                self.docs[key] = ' '.join(doc)
                doc = []
                stateAppend = False
                continue
            if stateAppend:
                doc.append(self.strip(line))
        f.close()



class CPPDocs:
    """ Parse C++ code into dictionary
    { 'procedureName' : 'its docstring' }
    """

    def __init__(self):
        self.path = os.path.join('..', 'plugins', 'scriptplugin')
        self.allProcedures = []
        self.undocumented = []
        self.mapping = []
        self.currentDocs = {}

        self.getAllProcedures()

        for i in os.listdir(self.path):
            if i.endswith('.h'):
                self.mapping += self.getFileProcedures(i)
        # checking
        for i in self.allProcedures:
            if i[2] == 'PROCEDURE':
                find = False
                for j in self.mapping:
                    if j == i[1]:
                        find = True
                        break
                if not find:
                    self.undocumented.append(i)

        for i in self.allProcedures:
            try:
                # TODO: still only procs.
                if i[2] != 'PROCEDURE':
                    continue
                # remove nultiple whitespaces - use only ' '
                tmp = eval('pydoc.getdoc(' + i[0] + ')').replace('>', '&gt;').replace('<', '&lt;')
                ws = tmp.split()
                tmp = ' '.join(ws)
                self.currentDocs[i[0]] = tmp
            except NameError, e:
                self.currentDocs[i[0]] = e
            except:
                print 'Unhandled exception for: ', i[0]
                traceback.print_exc(file=sys.stdout)


    def getAllProcedures(self):
        f = open(os.path.join(self.path, 'scriptplugin.cpp'), 'r')
        for line in f:
            # procedures
            if line.find('{const_cast<char*>(') != -1:
                tmp = line.strip().split(',')
                # clean the components
                tmp[0] = tmp[0].replace('{const_cast<char*>("', '').replace('")', '')
                tmp[3] = tmp[3].replace('tr(', '').replace(')}', '').strip()
                self.allProcedures.append([tmp[0], tmp[3], 'PROCEDURE'])
            # objects
            if line.find('PyModule_AddObject') != -1:
                tmp = line.strip().replace('PyModule_AddObject(m, (char*)"', '').replace('"', '').replace('(PyObject *) &', '').split(',')
                self.allProcedures.append([tmp[0], tmp[1], 'OBJECT'])
            # constants
            if line.find('PyDict_SetItemString') != -1:
                tmp = line.strip().replace('PyDict_SetItemString(d, const_cast<char*>("', '').split('"')
                self.allProcedures.append([tmp[0], '', 'CONSTANT'])
        f.close()

    def getFileProcedures(self, fname):
        d = []
        f = open(os.path.join(self.path, fname), 'r')
        for line in f:
            if line.find('PyDoc_STRVAR') != -1:
                d.append(line.replace('PyDoc_STRVAR(', '').split(',')[0])
        f.close()
        return d



def order():
    """ dummy sql clausule """
    return ' order by 1'


def getHTMLDocs(name, doc):
    """ create a html template for scribus documentation.
    It looks like it's written in PERL ;) """
    return """<pre>&lt;dt&gt;&lt;a name="-%s"&gt;&lt;strong&gt;%s&lt;/strong&gt;(...)&lt;/a&gt;&lt;/dt&gt;
&lt;dd&gt;&lt;code&gt;&lt;/code&gt;
&lt;p&gt;%s&lt;/p&gt;&lt;/dd&gt;</pre>""" % (name, name, doc.replace('&', '&amp;'))



print 'START'

h = HTMLDocs()
c = CPPDocs()

out = []
out.append('<p>Remember: You have to "make install" after code change to promote the changes here.</p>')

out.append('<h1>Undocumented in C++ code</h1><p>Code with no docstrings in *.h files</p><ul>')
for i in c.undocumented:
    out.append('<li>' + i[0] + ' - ' + i[1] + '</li>')
out.append('</ul>')

conn = sqlite.connect(':memory:')
#conn = sqlite.connect('scdoc.db')
cur = conn.cursor()

try:
    cur.execute('drop table cpp')
    cur.execute('drop table html')
except:
    pass

cur.execute('create table cpp (name varchar(30), doc text)')
cur.execute('create table html (name varchar(30), doc text)')

for i in c.currentDocs:
    cur.execute('insert into cpp values (?, ?)', (unicode(i), unicode(c.currentDocs[i])))
for i in h.docs:
    cur.execute('insert into html values (?, ?)', (unicode(i), unicode(h.docs[i])))

conn.commit()

# undocumented in HTML vs. Code
out.append('<h1>HTML vs. Code</h1><p>Is in HTML but not in the code</p><ul>')
for i in cur.execute('select h.name from html as h left outer join cpp as c on (c.name = h.name) where c.name is null' + order()):
    out.append('<li>' + i[0] + '</li>')
out.append('</ul>')

# undocumented in Code vs HTML
out.append('<h1>Code vs HTML</h1><p>Is in the code but not in HTML</p><table border="1"><tr><th>Name</th><th>docstring</th></tr>')
for i in cur.execute('select c.name, c.doc from cpp as c left outer join html as h on (c.name = h.name) where h.name is null' + order()):
    out.append('<tr><td valign="top">' + i[0] + '</td><td>' + getHTMLDocs(i[0], i[1]) + "</td></tr>")
out.append('</table>')

# documented in the both places. Checking for diffs in docstrings
out.append('<h1>Doc diffs</h1><table border="1"><tr><th>code</th><th>html</th><th>diff</th></tr>')
# I don't care about whitespaces
diff = difflib.Differ(charjunk=difflib.IS_CHARACTER_JUNK)
for i in cur.execute('select c.doc, h.doc, c.name, h.name from cpp as c, html as h where c.name = h.name and c.doc != h.doc' + order()):
    out.append('<tr><th>' + i[2] + '</th><th>' + i[3] + '</th>')
    out.append('<td rowspan="2"><pre>' + '\n'.join(list(diff.compare([i[0]], [i[1]]))) + '</pre></td></tr>\n')
    out.append('<tr><td>' + i[0] + '</td><td>' + i[1] + '</td>\n')
out.append('</table>')

out.append('<h1>Correct docs</h1><table border="1"><tr><th>code</th><th>html</th></tr>')
for i in cur.execute('select c.name, h.name from cpp as c, html as h where c.name = h.name and c.doc = h.doc' + order()):
    out.append('<tr><td>' + i[0] + '</td><td>' + i[1] + '</td></tr>\n')
out.append('</table>')

print 'FINISHING...'

conn.close()
f = open('scribus-docs-check.html', 'w')
f.write(''.join(out))
f.close()

print 'END'
