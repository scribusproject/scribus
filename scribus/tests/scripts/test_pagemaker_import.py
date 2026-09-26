#!/usr/bin/env python3

"""Smoke-test a PageMaker document without exposing its contents.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import os

import scribus


source = os.environ["SCRIBUS_PAGEMAKER_TEST_FILE"]
assert scribus.openDoc(source), "PageMaker import failed"
pages = scribus.pageCount()
objects = len(scribus.getAllObjects())
assert pages > 0, "PageMaker import created no pages"
assert objects > 0, "PageMaker import created no objects"
scribus.closeDoc()
print("PAGEMAKER_IMPORT_PASSED pages=%d objects=%d" % (pages, objects), flush=True)
