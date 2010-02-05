#!/usr/bin/env python
"""
This script parses C++ source- and header-files and searches for the following blocks:

  /**
  * fullpath.to.mycall(arg1)
  * args is int > 0
  * returns bool, tree on success
  *
  * this method does something
  */

The comment block has to start with two stars.
There is an empty line between the call-declaration and the description. 
If an entry is a property, the second line should be "Property".
You should mention in the line below which type the property is.
Append (read-only) to this line, if you cannot change the propery.
"""

import os


class Entry(object):


    def __init__(self, signature, is_property, returns, args, description):
        self.name = signature.split("(", 1)[0]
        self.signature = signature
        self.is_property = is_property
        self.returns = returns
        self.args = args
        self.description = description
        if is_property:
            self.typename = "PROPERTY"
        else:
            if returns:
                self.typename = "FUNCTION"
            else:
                self.typename = "PROCEDURE"

    def __cmp__(self, other):
        return cmp(self.name, other.name)

    @classmethod
    def parse(cls, lines):
        signature = lines[0]
        if len(lines) > 1 and lines[1].strip().lower() == "property":
            is_prop = True
            i = 2
        else:
            is_prop = False
            i = 1
        args = []
        returns = ""
        while i < len(lines) and lines[i].strip():
            if lines[i].startswith("returns "):
                returns = lines[i] #[8:]
            else:
                args.append(lines[i])
            i += 1
        i += 1
        if is_prop:
            args = ["\n".join(args)]
        desc = []
        while i < len(lines):
            desc.append(lines[i])
            i += 1
        return cls(signature, is_prop, returns, args, desc)
        


def extract(filename):
    lines = open(filename).read().splitlines()
    i = 0
    length = len(lines)
    while i < length:
        l = lines[i].strip()
        if l.startswith("/**"):
            i += 1
            block = []
            while not l.endswith("*/") and i < length:
                l = lines[i].lstrip()
                if l.startswith("* "):
                    block.append(l[2:])
                elif l == "*":
                    block.append("")
                i += 1
            if len(block):
                yield Entry.parse(block)
        else:
            i += 1


def find(files):
    entries = []
    for filename in files or os.listdir("."):
        if filename.endswith((".cpp", ".h", ".api")): # and filename.startswith("api_")           
            entries.extend(list(extract(filename)))
    entries.sort()
    return entries


html_template = """
<html>
   <head><title></title></head>
<body>
<h2>Scripter API Reference</h2>

<table>
%(entries)s
</table>
</body>
</html>
"""

html_entry_template = """
<tr>
  <td><strong>%(signature)s</strong></td>
  <td><strong>%(typename)s</strong></td>
</tr>
<tr>
  <td colspan="2">
  <em>%(returns)s</em>
  </td>
</tr>
<tr>
  <td colspan="2">
  <ul>%(args)s</ul>
  </td>
</tr>
<tr>
  <td colspan="2">
  %(description)s
  </td>
</tr>
<tr><td colspan="2"><hr></td></tr>
"""


html_entry_args_template = "<li>%s</li>"

def main(files):
    # This is a lot of room for improvement. Grouping, sorting, linking, ...
    from cgi import escape as html_quote
    def html_entry(entry):
        d = dict(entry.__dict__)
        d["args"] = "".join([html_entry_args_template % html_quote(a) for a in entry.args])
        d["description"] = html_quote(" ".join(entry.description))
        return html_entry_template % d
    print html_template % {
        "entries": "".join([html_entry(entry)  for entry in find(files)])
      }        


if __name__ == "__main__":
    import sys
    main(sys.argv[1:])
