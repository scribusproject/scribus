#!/usr/bin/env python

""" Simple unicode map with comments strip for Scribus
It converts original file taken from:
http://www.unicode.org/Public/5.0.0/ucd/NamesList.txt
into simple hex:comment values map used in UnicodeSearch
class
Petr Vanek <petr@scribus.info>
"""
print 'start stripping'

f = open('NamesList.txt', 'r')

out = {}
for i in f:
    if i[:1] == '@' or i[:1] == '\t':
        continue
    str = i.split('\t')
    out[str[0]] = ' '.join(str[1:]).strip()

f.close()

print 'writing output'

f = open('unicodenameslist.txt', 'w')
for i in out:
    f.write("%s:%s\n" % (i, out[i]))
f.close()

print 'done'

