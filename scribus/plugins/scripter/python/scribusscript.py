"""
Loader and handler for Scribus descriptor files/headers.
They help to integrate scripts into Scribus by definining menu-entries, 
shortcuts, etc. Some additional metadata gives useful information and help.

See doc/TUTORIAL for a detailed explanation including examples.
"""
import sys
import re
import os
from ConfigParser import ConfigParser
from StringIO import StringIO
from PyQt4.QtGui import QKeySequence, QIcon

import excepthook
from scripter_hooks import MenuHooks
from scripter_runtime import run_filename


class ValidationError(Exception):
    pass

class EmptyDescriptor(ValidationError):
    pass


def validate_bool(value):
    lvalue = value.lower()
    if lvalue in ["0", "no", "false", "off"]:
        return False
    elif lvalue in ["1", "yes", "true", "on"]:
        return True


def validate_regex(pattern):
    def check(value):
        found = re.match(pattern, value)
        if found:
            return found.group(0)
        raise ValidationError, \
             "Value %r does not match regular expression pattern %r" % pattern
    return check


validate_ident = validate_regex("[A-Za-z_][A-Za-z_]*")


def validate_list(value):
    return value.split(",")
    
    
def validate_intlist(value):
    try:
        return [int(v) for v in validate_list(value)]
    except ValueError, e:
        raise ValidationError, "Int-validation error: %s" % e


def validate_enum(*args):
    def check(value):
        if value.lower() in args:
            return value.lower()
        raise ValidationError, "%r not in %r" % (value, args)
    return check


def validate_enumlist(*args):
    def check(value):
        l = []
        for v in value.split(","):
            l.append(validate_enum(args)(v))
        return l
    return check


class Item(object):
    
    _counter = 0

    def __init__(self, name, default=None, validate=None,
                       required=False):
        self._item_id = Item._counter
        Item._counter += 1
        self.name = name
        self.default = default
        if isinstance(validate, basestring):
            validate = validate_regex
        self.validate = validate or (lambda v:v)
        self.required = required


    def __call__(self, value, ignore_errors=False):
        try:
            pyvalue = self.validate(value)
        except ValidationError, e:
            if not ignore_errors:
                raise
            pyvalue = self.default
        return pyvalue




class ScribusScript(object):

    # Some items are disabled for now. 
    # They will be supported in a future release.
    items = [
        Item("name"),
        Item("title"),
        Item("description"),
        Item("icon"),
        #Item("category"),
        Item("menu", "Scripter"),
        #Item("context_menu"),
        Item("shortcut"),
        Item("filename"),
        Item("subroutine"),
        Item("author"),
        Item("contact"),
        Item("homepage"),
        Item("version"),
        Item("copyright", "Licensed under GPLl 2 or later"),
        Item("scribus_version"),
        #Item("tags", [], validate_list),
        #Item("depends", [], validate_list),        
        #Item("requires", [], validate_enumlist(
        #                        "document", "selection", "text", "image")),
        Item("redraw", True, validate_bool),
        Item("mode", "interactive", validate_enum("batch", "interactive", "extension")),
        #Item("before_action"),
        #Item("after_action"),
        #Item("on_event"),
        Item("language", "python", validate_enum("python", "qtscript")),
        Item("separator_before", False, validate_bool),
        Item("separator_after", False, validate_bool),
        Item("background_mode", False, validate_bool),
    ]


    def __init__(self, _data=None, **kwargs):
        self.data = {}
        d = dict(_data or {}, **kwargs)
        for item in self.__class__.items:
            self.data[item.name] = d.pop(item.name, item.default)
        if d:
            raise TypeError, "Unknown items: %s" % ", ".join(d.keys())


    def __repr__(self):
        return "<%s %r>" % (self.__class__.__name__, self.data)


    def __getattr__(self, name):
        return self.data[name]
        
        
    def __getitem__(self, name):
        return self.data[name]

    
    def get(self, name, default=None):
        return self.data.get(name, default)

    
    def __setitem__(self, name, value):
        self.data[name] = value


    def install(self):
        """
        currently only can create menu entries and sets shortcuts
        """
        if self.menu:
            mh = MenuHooks()
            menu = mh.findMenu(self.menu)
            if not menu:
                menu = mh.createMenu(self.menu)
                mh.appendMenu(menu)
            if self.separator_before:
                mh.appendSeparator(menu)
            self.action = mh.appendItem(menu, self.title, lambda :self.run())
            if self.separator_after:
                mh.appendSeparator(menu)
            if self.icon:
                icon_filename = os.path.join(
                   os.path.dirname(self.filename), self.icon)
                if os.path.exists(icon_filename):
                    self.action.setIcon(QIcon(icon_filename))
                else:
                    print >> sys.stderr, "Icon %r not found" % icon_filename
            if self.shortcut:
                print >> sys.stdout, "Shortcut %r." % self.shortcut 
                self.action.setShortcut(QKeySequence(self.shortcut))


    def run(self, catch_errors=True):
        """
        uses scripter_runtime to call a script
        """
        try:
            win = Scripter.activeWindow
            if win:
                win.redraw = self.redraw
            run_filename(self.filename, self.subroutine,
               extension=(self.mode == "extension"),
               background=self.background_mode)
            if win:
                win.redraw = True
                if not self.redraw:
                    win.update()
        except:
            if not catch_errors:
                raise
            excepthook.show_current_error("Error running %r" % os.path.basename(self.filename))


    @classmethod
    def parse_filename(cls, filename):
        s = open(filename).read(8192)
        name, ext = os.path.splitext(os.path.basename(filename))
        parse = cls.filetypes[ext]
        try:
            obj = parse(s)
        except EmptyDescriptor:
            if ext in [".spy"]:
                language = "python"
            elif ext in [".sjs", ".sqts"]:
                language = "qtscript"
            obj = cls(name=name, title=name.capitalize(),
                       language=language)
        if not obj.get("filename"):
            obj["filename"] = filename
        return obj


    @classmethod
    def parse_python(cls, source):
        s = "[ScribusScript]\nlanguage=python\n"
        for line in source.splitlines():
            if not line.startswith("#"):
                break
            if line.startswith("##"):
                s += line[2:].strip() + "\n"
        return cls.parse(s)


    @classmethod
    def parse_qtscript(cls, source):
        s = "[ScribusScript]\nlanguage=qtscript\n"
        for line in source.splitlines():
            if  not line.startswith("//"):
                break
            if line.startswith("///"):
                s += line[3:].strip() + "\n"
        return cls.parse(s)


    @classmethod
    def parse(cls, s):
        data = {}
        cfg = ConfigParser()
        s = "[ScribusScript]\n" + s
        cfg.readfp(StringIO(s))
        options = cfg.options("ScribusScript")
        if not len(options):
            raise EmptyDescriptor
        for item in cls.items:
            if not item.name in options:
                if item.required:
                    raise ValidationError, "Option %r required but not set" % item.name
                else:
                    continue
            options.remove(item.name)
            value = cfg.get("ScribusScript", item.name)
            data[item.name] = item(value)
        if options:
            raise ValidationError, "Invalid options found: %s" % ", ".join(options)
        
        return cls(**data)



ScribusScript.filetypes = {
  # XXX support zip archives
  ".spy": ScribusScript.parse_python,
  ".sjs": ScribusScript.parse_qtscript,
  ".sqts": ScribusScript.parse_qtscript,
  ".scs": ScribusScript.parse
}



def load_filename(filename):
    return ScribusScript.parse_filename(filename)



def load_scripts(path_or_filename):
    if os.path.isdir(path_or_filename):
        path = os.path.abspath(os.path.expanduser(path_or_filename))
        extensions = ScribusScript.filetypes.keys()
        files = [ os.path.join(path, name) for name in os.listdir(path) \
                              if os.path.splitext(name)[1] in extensions ]
    else:
        files = [ os.path.abspath(os.path.expanduser(path_or_filename)) ]
    scripts = []
    for filename in files:
        try:
            sd = load_filename(filename)
            scripts.append(sd)
        except:
            excepthook.show_current_error("Error loading %r" % os.path.basename(filename))
    return scripts




if __name__ == "__main__":
    # Show script descriptor for given files
    for filename in sys.argv[1:]:
        script = ScribusScript.parse_filename(filename)
        print filename
        print script
    if not sys.argv[1:]:
        for sd in load_scripts("."):
            print sd
