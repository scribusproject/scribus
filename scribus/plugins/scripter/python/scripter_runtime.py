"""
This runtime module contains everything about running 
Python and QtScript scripts inside Scribus.

Look at run_filename for details.
"""
import os
import hashlib
from ConfigParser import ConfigParser

import sip
from PyQt4.QtCore import QThread, QObject, QVariant
from PyQt4.QtGui import qApp,  QMessageBox
from PyQt4.QtScript import QScriptEngine, QScriptValue

from safe_eval import checkCode
import permitdlg

import __main__

from inspect import getargspec

class RuntimeConfig(ConfigParser):
    
    # I cannot use Scripter.preferences because a safe script could
    # mark other scripts as safe (=allowed) although they use import and
    # other (possible) dangerous stuff..
    # Perhaps I will find a better solution later.

    def __init__(self):
        ConfigParser.__init__(self)
        # XXX better use ScPaths->...
        path = os.path.expanduser("~/.scribus/scripter")
        if not os.path.exists(path):
            os.makedirs(path)
        self.filename = os.path.join(path, "runtime.cfg")
        self.read([self.filename])


    def save(self):
        fp = open(self.filename, "w")
        self.write(fp)
        fp.close()


    def set(self, section, key, value):
        if not self.has_section(section):
            self.add_section(section)
        ConfigParser.set(self, section, key, value)
        self.save()
    

    def getbool(self, section, key):
        value = self.get(section, key).strip().lower()
        if value and value in ["true", "on", "yes", "1"]:
            return True
        elif value and value in ["false", "off", "no", "0"]:
            return False
        else:
            raise ValueError, "Invalid boolean value %r" % value


runtime_config = RuntimeConfig()

extension_namespace = __main__.__dict__


qts_engine = None

# XXX share namespaces of Python and QtScript

class QtSRuntimeError(Exception):
    pass


def qts_func_decorator(func):
    def wrapper(context, engine):
        args = []
        (fargs, fvarargs, fvarkw, fdefaults) = getargspec(func)
        if len(fargs) and fargs[0] == "self":
            args.append(context.thisObject())
        for i in xrange(context.argumentCount()):
            args.append(context.argument(i))
        try:
            result = func(*args)
        except Exception, e:
            # XXX correct behaviour?
            # http://lists.trolltech.com/qt-interest/2007-06/thread00892-0.html
            return context.throwValue(QScriptValue(engine, str(e)))
        if result:
            return QScriptValue(engine, result)
        else:
            return QScriptValue()
    return wrapper


@qts_func_decorator
def alert(msg_qsv):
    msg = msg_qsv.toString()
    QMessageBox.information(Scripter.dialogs.mainWindow.qt, "Alert", msg)


def update_qs_namespace(engine, ns):
    go = engine.globalObject()
    for name, value in ns.items():
        if isinstance(value, QObject):
            value = engine.newQObject(value)
        elif callable(value):
            value = engine.newFunction(value)
        #elif not isinstance(value, QScriptValue):
        #    value = QScriptValue(engine, value)
        go.setProperty(name, value)

        
def newQScriptEngine():
    engine = QScriptEngine()
    update_qs_namespace(engine, 
       { 
          "Application": qApp,
          "Scripter": Scripter.qt,
          "alert": alert
       })
    return engine


def run_qtscript(filename, subroutine=None, extension=False):
    global qts_engine
    if not extension:
        engine = newQScriptEngine()
    else:
        engine = qts_engine = qts_engine or newQScriptEngine()
    code = open(filename).read()
    engine.clearExceptions()
    result = engine.evaluate(code)
    engine.collectGarbage()
    if not engine.hasUncaughtException() and subroutine:
        sub = engine.globalObject().property(subroutine)
        sub.call()
    if engine.hasUncaughtException():
        bt = engine.uncaughtExceptionBacktrace()
        raise QtSRuntimeError("%s\nTraceback:\%s" % (
              str(engine.uncaughtException().toString()),
              "\n".join(["  %s" % l for l in list(bt)])))


def hash_source(filename, source=None):
    # I gueses sha256 is safe enough without collisions?
    source = source or open(filename).read()
    return "%s:%s:%s" % (
        os.path.basename(filename), len(filename), hashlib.sha256(source).hexdigest())


def check_python(filename):
    filename = os.path.abspath(os.path.expanduser(filename))
    path = os.path.dirname(filename)
    # Allow files from global autoload folder by default. 
    # XXX Good idea?
    if path == os.path.join(Scripter.path, "autoload"):
        return True
    code = open(filename).read()
    h = hash_source(filename, code)
    if runtime_config.has_option("permissions", h):
        return runtime_config.getbool("permissions", h)

    problems = checkCode(code)
    if problems and len(problems) == 1 and isinstance(problems[0], SyntaxError):
        return True # let's ignore it and let excepthook hande the error later
    elif problems:
        ok = permitdlg.ask(filename, problems)
        if ok == -2: # deny and remember
            runtime_config.set("permissions", h, False)
            return False
        elif ok == 2: # deny
            return False
        elif ok == -1: # allow and remember
            runtime_config.set("permissions", h, True)
        elif ok == 1: # allow but now remember
            pass
        else:
            raise ValueError, "Inknown return code for permission dialog: %r" % ok
    return True


def run_python(filename, subroutine=None, extension=False):
    if not extension:
        namespace = {
        __name__: "__scribus__",
        __file__: filename
        }
    else:
        namespace = extension_namespace
    if not check_python(filename):
        return
    execfile(filename, namespace)
    if subroutine:
        sub = namespace[subroutine]
        sub()
    if not extension:
        del namespace


threads = []

class RunThread(QThread):

    
    def __init__(self, func, *args):
        QThread.__init__(self, Scripter.qt)
        self.func = func
        self.args = args


    def run(self):
        threads.append(self)
        self.func(*self.args)
        threads.remove(self)
        

def run_background(func, *args):
    thread = RunThread(func, *args)
    thread.start()
    # XXX: connect done signal with cleanup?
    return thread



def mark_keep():
    """
    mark every child of Scripter.collector to keep
    """
    for child in Scripter.collector.children():
        if hasattr(child, "qt"): child = child.qt        
        child.setProperty("keep", QVariant(True))



def cleanup():
    """ 
    delete every child which is not marked as keep
    """
    for child in Scripter.collector.children():
        if hasattr(child, "qt"): child = child.qt
        v = child.property("keep")
        if v and v.toBool() == True:
            #print "Keeping", child
            continue
        print "* deleting collected", child
        sip.delete(child)



def run_filename(filename, subroutine=None, extension=False, background=False):
    """
    Call this function to run a script and nothing else. 
    It will do everything for you, including garbage collection
    for QtScript (very simple implementation, see mark_keep and cleanup).
    Running as extension uses the __main__ namespace and does not
    delete objects after execution.
    Running in background as a thread is not much tested and 
    should only be used for non-GUI scripts.
    """
    mark_keep()
    if background:
        run_func = run_background
    else:
        run_func = lambda func, *args: func(*args)
    if filename.endswith((".sqts", ".qts", ".sjs", ".js")):
        run_func(run_qtscript, filename, subroutine, extension)
    else:
        run_func(run_python, filename, subroutine, extension)
    if not background and not extension:
        # XXX: make sure this is called if an exception occures...
        cleanup()
