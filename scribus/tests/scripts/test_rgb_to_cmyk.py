#!/usr/bin/env python3

"""Integration test for ICC conversion of named RGB process colors."""

import os
import tempfile
import xml.etree.ElementTree as ET

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def saved_color(path, name):
    document = ET.parse(path)
    for element in document.iter("Color"):
        if element.get("Name") == name:
            return element.attrib
    raise AssertionError("saved color was missing: " + name)


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
document_path = os.path.join(output_dir, "rgb-to-cmyk.sla")

check(
    scribus.newDocument(
        scribus.PAPER_A4,
        (20, 20, 20, 20),
        scribus.PORTRAIT,
        1,
        scribus.UNIT_POINTS,
        scribus.PAGE_1,
        0,
        1,
    ),
    "could not create document",
)

scribus.defineColorRGB("Conversion Target", 60, 120, 210)
scribus.defineColorRGB("Spot Control", 220, 60, 40)
scribus.setSpotColor("Spot Control", True)
scribus.defineColorCMYK("CMYK Control", 40, 20, 0, 10)
frame = scribus.createRect(30, 40, 120, 80, "Converted Fill")
scribus.setFillColor("Conversion Target", frame)

preview = scribus.previewRGBToCMYK()
check("Conversion Target" in preview, "RGB swatch missing from preview")
check("Spot Control" not in preview, "spot swatch appeared in preview")
check("CMYK Control" not in preview, "existing CMYK swatch appeared in preview")
check(len(preview["Conversion Target"]) == 4, "preview does not contain four channels")
check(all(0 <= value <= 100 for value in preview["Conversion Target"]), "preview channel out of range")

try:
    scribus.convertRGBToCMYK(["Conversion Target", "Unknown Color"])
except ValueError:
    pass
else:
    raise AssertionError("conversion accepted an unknown color")
check("Conversion Target" in scribus.previewRGBToCMYK(), "failed conversion changed the swatch")

check(scribus.convertRGBToCMYK(["Conversion Target"]) == 1, "expected one converted swatch")
check("Conversion Target" not in scribus.previewRGBToCMYK(), "converted swatch remained RGB")
check("Red" in scribus.previewRGBToCMYK(), "unselected RGB swatches were converted")
check(scribus.getFillColor(frame) == "Conversion Target", "frame lost its named color")

scribus.undo()
check("Conversion Target" in scribus.previewRGBToCMYK(), "undo did not restore RGB swatch")
check(scribus.getFillColor(frame) == "Conversion Target", "undo changed the frame color name")
scribus.redo()
check("Conversion Target" not in scribus.previewRGBToCMYK(), "redo did not restore CMYK swatch")

scribus.saveDocAs(document_path)
check(saved_color(document_path, "Conversion Target")["Space"] == "CMYK", "CMYK did not persist")
check(saved_color(document_path, "Spot Control")["Space"] == "RGB", "spot control changed")
check(saved_color(document_path, "CMYK Control")["Space"] == "CMYK", "CMYK control changed")
check(saved_color(document_path, "Red")["Space"] == "RGB", "unselected RGB swatch changed")
scribus.closeDoc()

check(scribus.openDoc(document_path), "could not reopen saved document")
check("Conversion Target" not in scribus.previewRGBToCMYK(), "saved CMYK swatch reopened as RGB")
check(scribus.getFillColor(frame) == "Conversion Target", "saved frame lost its color reference")
scribus.closeDoc()

print("RGB_TO_CMYK_QA_PASSED", flush=True)
