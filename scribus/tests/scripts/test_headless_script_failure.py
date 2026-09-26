#!/usr/bin/env python3

"""Confirm that an unhandled headless Scripter error fails the process.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

raise RuntimeError("HEADLESS_SCRIPT_FAILURE_EXPECTED")
