#!/usr/bin/env python
# -*- coding: utf-8 -*-

""" Make selected group smaller by 10% """

import sys

try:
    from scribus import *
except ImportError:
    print "This script only runs from within Scribus."
    sys.exit(1)

if haveDoc() and selectionCount():
    scaleGroup(1/1.1)
else:
    messageBox("moins_10_pourcent_group.py", "Please select an object to scale before running this script.", ICON_INFORMATION)
