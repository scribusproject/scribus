#!/usr/bin/env python3

"""Reject malformed IDML packages without creating a document.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import os
from pathlib import Path
from zipfile import ZipFile

import scribus


output = Path(os.environ["SCRIBUS_TEST_OUTPUT_DIR"])
output.mkdir(parents=True, exist_ok=True)

not_zip = output / "not-a-package.idml"
not_zip.write_bytes(b"not an IDML package")

missing_map = output / "missing-designmap.idml"
with ZipFile(missing_map, "w") as archive:
    archive.writestr("other.xml", "<Document/>")

wrong_root = output / "wrong-root.idml"
with ZipFile(wrong_root, "w") as archive:
    archive.writestr("designmap.xml", "<NotAnInDesignDocument/>")

no_pages = output / "no-document-pages.idml"
with ZipFile(no_pages, "w") as archive:
    archive.writestr(
        "designmap.xml",
        '<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/"/>',
    )

missing_spread = output / "missing-spread-component.idml"
with ZipFile(missing_spread, "w") as archive:
    archive.writestr(
        "designmap.xml",
        '<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">'
        '<idPkg:Spread src="Spreads/missing.xml"/></Document>',
    )

missing_story = output / "missing-story-component.idml"
with ZipFile(missing_story, "w") as archive:
    archive.writestr(
        "designmap.xml",
        '<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">'
        '<idPkg:Spread><Spread><Page Self="Page/1"/></Spread></idPkg:Spread>'
        '<idPkg:Story src="Stories/missing.xml"/></Document>',
    )

for source in (not_zip, missing_map, wrong_root, no_pages, missing_spread, missing_story):
    print("Checking malformed IDML:", source.name, flush=True)
    try:
        opened = scribus.openDoc(str(source))
    except Exception:
        opened = False
    assert not opened, "Malformed IDML unexpectedly opened"
    assert not scribus.haveDoc(), "Malformed IDML left an open document"

print("IDML_REJECTION_PASSED", flush=True)
