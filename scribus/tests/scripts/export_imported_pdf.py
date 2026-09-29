#!/usr/bin/env python3

"""Export selected pages from an imported document for private visual QA.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import os
from pathlib import Path

import scribus


source = Path(os.environ["SCRIBUS_IMPORT_QA_FILE"]).resolve()
output = Path(os.environ["SCRIBUS_IMPORT_QA_PDF_OUT"]).resolve()
requested = [int(value) for value in os.environ["SCRIBUS_IMPORT_QA_PAGES"].split(",")]
compression = int(os.environ.get("SCRIBUS_IMPORT_QA_COMPRESSION", "0"))

assert source.is_file(), "Import source does not exist"
assert output.parent.is_dir(), "PDF output directory does not exist"
assert not output.exists(), "Refusing to overwrite an existing PDF"
assert requested and len(set(requested)) == len(requested), "Page selection is empty or repeated"
assert 0 <= compression <= 3, "Compression must be 0 (auto), 1 (JPEG), 2 (ZIP), or 3 (none)"
assert scribus.openDoc(str(source)), "Document import failed"
page_count = scribus.pageCount()
assert all(1 <= page <= page_count for page in requested), "Page selection is out of range"
print("IMPORT_VISUAL_IMPORT pages=%d requested=%d" % (page_count, len(requested)), flush=True)

if os.environ.get("SCRIBUS_IMPORT_QA_SPLIT") == "1":
    failed = []
    for page in requested:
        page_output = output.with_name("%s-page-%03d.pdf" % (output.stem, page))
        assert not page_output.exists(), "Refusing to overwrite an existing page PDF"
        pdf = scribus.PDFfile()
        pdf.file = str(page_output)
        pdf.pages = [page]
        pdf.compressmtd = compression
        pdf.useDocBleeds = 0
        pdf.bleedt = pdf.bleedl = pdf.bleedr = pdf.bleedb = 0
        try:
            pdf.save()
        except Exception as error:
            failed.append(page)
            print("IMPORT_VISUAL_PAGE_FAILED page=%d error=%s" % (page, error), flush=True)
        else:
            print("IMPORT_VISUAL_PAGE_EXPORTED page=%d" % page, flush=True)
    scribus.closeDoc()
    assert not failed, "PDF export failed for pages %s" % failed
else:
    pdf = scribus.PDFfile()
    pdf.file = str(output)
    pdf.pages = requested
    pdf.compressmtd = compression
    pdf.useDocBleeds = 0
    pdf.bleedt = pdf.bleedl = pdf.bleedr = pdf.bleedb = 0
    pdf.save()
    scribus.closeDoc()
    assert output.is_file() and output.stat().st_size > 0, "PDF export did not create a file"
    print(
        "IMPORT_VISUAL_PDF_EXPORTED source_pages=%d exported_pages=%d"
        % (page_count, len(requested)),
        flush=True,
    )
