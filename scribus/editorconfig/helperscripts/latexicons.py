#!/usr/bin/env python
import sys
import string
import subprocess

def conv_filename(name):
	"""Remove all bad characters and convert to all lowercase."""
	badchars = "\\/!\"$%&{}=?[]* "
	for char in badchars:
		name = name.replace(char, '')
	for char in string.ascii_uppercase:
		name = name.replace(char, char.lower()+'_')
	return name
	
	
def run_command(command):
	global retcode, cmd_output
	process = subprocess.Popen(command.split(' '), stderr=subprocess.PIPE, stdout=subprocess.PIPE)
	cmd_output = process.communicate()
	retcode = process.returncode
	return retcode

if len(sys.argv) < 3:
	print "usage: icons.py iconlist template"
	sys.exit(1)
	
symbols = open(sys.argv[1]).readlines()
template = open(sys.argv[2]).read()
xmlsample = open("xmlsample", 'w')

i = 0
for symbol in symbols:
	i += 1
	symbol = symbol.strip()
	if symbol.startswith('#') or symbol == "":
		continue
	print "%5.1f%%   %-30s ." % (float(i)/len(symbols)*100, symbol),
	source = template % symbol
	#print source
	f = open("tmp", 'w')
	f.write(source)
	f.close()
	
	if run_command("latex --interaction batchmode tmp") != 0:
		print "Error", cmd_output
		continue
	else:
		print ".",
		
	filename = conv_filename(symbol)
	if run_command("dvipng -D 200 -T tight -pp 1 -bg Transparent -o out/%s.png tmp.dvi" % filename) != 0:
		print "Error", cmd_output
		continue
	else:
		print "."
	xmlsample.write(r'<item value="%s"  image="%s.png" />' %(symbol, filename)+"\n")
	
