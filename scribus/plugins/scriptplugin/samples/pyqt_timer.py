#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
A simple demo script showing the use of PyQt timers.

Note that if you run this script again before the timer
fires, the first timer is cancelled (because the timer object
is deleted when you replace the only reference to it, the
`timer' name in the global scope. Watch out for this sort
of thing when writing your scripts.
"""

try:
    import qt
except ImportError,err:
    print "Seems you don't have PyQt. Doing nothing."

try:
    import scribus
except ImportError,err:
    print "This script only runs from within Scribus"

def msg():
    scribus.messageBox("Scribus - Python Timer",
                       "The timer has run out",
                       scribus.BUTTON_OK)

def main():
    global timer
    timer = qt.QTimer()
    timer.connect(timer, qt.SIGNAL("timeout()"), msg)
    scribus.messageBox("Scribus - Python Timer",
                       "About to start a 5 second timer",
                       scribus.BUTTON_OK)
    timer.start(5*1000, True)

if __name__ == '__main__':
    main()
