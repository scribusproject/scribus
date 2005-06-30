#!/usr/bin/env python
# -*- coding: utf8 -*-

"""
This script is a simple example to show how you can trigger things based on
event hooks provided by Scribus. This sample runs the `gotSignal' method
when the app has finished setting up and it emits the appStarted() signal.
This is useful if, for example, you need access to the main window to
do your setup when running as a startup script.

This script isn't very interesting unless you run it as a startup script,
with Scribus running in an xterm, and watch the output on stdout/stderr.

You will need PyQt for this script to work.
"""

try:
    import qt
except:
    print "Seems you don't have PyQt. Doing nothing."
    return

from qt import SIGNAL, PYSIGNAL, SLOT

# Note that to connect to signals etc you MUST inherit from QObject or a subclass
# of QObject.
class Recipient(qt.QObject):

    def __init__(self):
        # Connect ourselves to the "appStarted()" signal emitted by Scribus.
        # Ask PyQt to run the self.gotSignal method when the signal is emitted.
        self.connect(qt.qApp, SIGNAL("appStarted()"), self.gotSignal)

    def gotSignal(self):
        print "PONG!"

if __name__ == '__main__':
    recip = Recipient();
