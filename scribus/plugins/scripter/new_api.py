#!/usr/bin/env python
"""
This little scaffolding script helps you creating new api classes.
"""
import sys, re, os

cc_re = re.compile("([A-Z][a-z]+)")


def is_camelcase(s):
    return cc_re.match(s) != None


def to_camelcase(s):
    return "".join([p.capitalize() for p in s.split("_")])


def from_camelcase(s):
    return "_".join(cc_re.split(s)[1:-1]).lower()


def replace_variations(s, name):
    lower = ("example", name.lower())
    upper = ("EXAMPLE", name.upper())
    camel = ("Example", to_camelcase(name))
    for old, new in (lower, upper, camel):
        s = s.replace(old, new)
    return s


def modify(filename, old, new):
    data = open(filename).read()
    data = data.replace(old, new)
    open(filename, "w").write(data)


def main(args):
    if "-f" in args:
        args.remove("-f")
        force = True
    else: 
        force = False
    if len(args) != 2:
        print >>sys.stderr, "Please give a name for a new api module"
        sys.exit(1)
    for name in args[1:]:
        if is_camelcase(name):
            name = from_camelcase(name)
        for filename in ["api_example.h", "api_example.cpp"]:
            target = replace_variations(filename, name)
            if os.path.exists(target):
                print "%r already exists, aborting. Force overwrite with -f" % target
                sys.exit(1)
            data = open(filename).read()
            targetdata = replace_variations(data, name)
            open(target, "w").write(targetdata)
            prefs_fn = filename.replace("example", "prefs")
            modify("CMakeLists.txt", prefs_fn, "%s\n%s" % (prefs_fn, target))
            if prefs_fn.endswith(".h"):
                modify("scripterimpl.h", 
                  '#include "%s"' % prefs_fn,
                  '#include "%s"\n#include "%s"' % (prefs_fn, target))
            else:
                modify("scripterimpl.cpp",
                  "new PreferencesAPI();",
                  "new PreferencesAPI();\n    new %sAPI();" % to_camelcase(name))
            print target, "created and added to CMakeLists.txt and scripterimpl.{h,cpp}"

    
        
if __name__ == "__main__":
    main(sys.argv)
