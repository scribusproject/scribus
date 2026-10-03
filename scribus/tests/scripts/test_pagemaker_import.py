#!/usr/bin/env python3

"""Exercise safe PageMaker rejection and opt-in local P65/PM7 round-trips.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import os
import tempfile
from collections import Counter
from pathlib import Path

import scribus


def document_summary():
    pages = scribus.pageCount()
    types = Counter()
    text_characters = 0
    for page in range(pages):
        for name in scribus.getAllObjects(page=page):
            kind = scribus.getObjectType(name)
            types[kind] += 1
            if kind == "TextFrame":
                text_characters += len(scribus.getAllText(name))
    return pages, types, text_characters


output = Path(os.environ["SCRIBUS_TEST_OUTPUT_DIR"])
output.mkdir(parents=True, exist_ok=True)

for suffix in ("p65", "pm7"):
    invalid = output / ("invalid." + suffix)
    invalid.write_bytes(b"This is not a PageMaker document.\n")
    try:
        opened = scribus.openDoc(str(invalid))
    except Exception:
        opened = False
    assert not opened, "Invalid PageMaker input unexpectedly opened"
    assert not scribus.haveDoc(), "Invalid PageMaker input left a document open"

samples = []
for suffix in ("p65", "pm7"):
    source_value = os.environ.get("SCRIBUS_PAGEMAKER_TEST_" + suffix.upper())
    if source_value:
        samples.append((suffix, source_value))

# Keep the earlier one-file smoke-test entry point working for local callers.
legacy_source = os.environ.get("SCRIBUS_PAGEMAKER_TEST_FILE")
if legacy_source and legacy_source not in (source for _, source in samples):
    samples.append((Path(legacy_source).suffix.lower().lstrip("."), legacy_source))

tested = 0
for suffix, source_value in samples:
    source = Path(source_value)
    assert suffix in {"pmd", "pm", "pm3", "pm4", "pm5", "pm6", "p65", "pm7"}, (
        "Unsupported PageMaker test extension"
    )
    assert source.is_file() and source.suffix.lower() == "." + suffix, (
        "PageMaker test input must be an existing ." + suffix + " file"
    )
    assert scribus.openDoc(str(source)), "PageMaker ." + suffix + " import failed"
    pages, types, text_characters = document_summary()
    assert pages > 0, "PageMaker import produced no pages"
    objects = sum(types.values())
    assert objects > 0, "PageMaker import produced no objects"

    # The converted document is ephemeral; never add a proprietary sample to
    # the source tree or leave its text and links in a build artifact.
    with tempfile.TemporaryDirectory(prefix="scribus-pagemaker-") as temp_dir:
        converted = Path(temp_dir) / ("roundtrip." + suffix + ".sla")
        scribus.saveDocAs(str(converted))
        scribus.closeDoc()
        assert converted.is_file(), "PageMaker conversion did not save an SLA"
        assert scribus.openDoc(str(converted)), "Converted PageMaker SLA did not reopen"
        reopened_pages, reopened_types, reopened_text_characters = document_summary()
        assert reopened_pages == pages, "Page count changed after SLA reopen"
        assert reopened_types == types, "Object types changed after SLA reopen"
        assert reopened_text_characters == text_characters, (
            "Editable text length changed after SLA reopen"
        )
        scribus.closeDoc()
    print(
        "PageMaker .%s: %d pages, %d objects, %d editable text characters"
        % (suffix, pages, objects, text_characters),
        flush=True,
    )
    tested += 1

print("PAGEMAKER_IMPORT_PASSED samples=%d" % tested, flush=True)
