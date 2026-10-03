#!/usr/bin/env python3

"""Manual QA against an externally exported IDML story oracle.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
"""

import json
import os
from pathlib import Path

import scribus


source = Path(os.environ["SCRIBUS_IDML_ORACLE_FILE"])
output = Path(os.environ["SCRIBUS_TEST_OUTPUT_DIR"]) / "idml-oracle.sla"
expected = (json.loads(os.environ["SCRIBUS_IDML_ORACLE_TEXTS"])
            if "SCRIBUS_IDML_ORACLE_TEXTS" in os.environ
            else [os.environ["SCRIBUS_IDML_ORACLE_TEXT"]])
assert expected and all(isinstance(text, str) and text for text in expected)

assert scribus.openDoc(str(source)), "IDML oracle did not open"
assert scribus.pageCount() == 1, "Unexpected page count"
frames = [name for name in scribus.getAllObjects(page=0)
          if scribus.getObjectType(name) == "TextFrame"]
assert len(frames) == len(expected), "Unexpected text frames: %r" % frames
originals = [scribus.getAllText(frame).strip() for frame in frames]
assert sorted(originals) == sorted(expected), "Imported stories differ: %r" % originals
for frame, original in zip(frames, originals):
    scribus.setText(original + " Editable.", frame)
    assert scribus.getAllText(frame).strip() == original + " Editable.", (
        "Could not edit imported text in %s" % frame
    )
scribus.saveDocAs(str(output))
scribus.closeDoc()
assert output.is_file(), "Could not save imported text as SLA"

assert scribus.openDoc(str(output)), "Could not reopen imported SLA"
frames = [name for name in scribus.getAllObjects(page=0)
          if scribus.getObjectType(name) == "TextFrame"]
assert len(frames) == len(expected), "Text frame lost on SLA reopen"
assert sorted(scribus.getAllText(frame).strip() for frame in frames) == sorted(
    text + " Editable." for text in expected
), "Text edit lost on SLA reopen"
scribus.closeDoc()
print("IDML_ORACLE_EDIT_PASSED", flush=True)
