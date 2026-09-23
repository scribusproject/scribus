#!/usr/bin/env python3

# For general Scribus (>=1.3.2) copyright and licensing information please
# refer to the COPYING file provided with the program. Following this notice
# may exist a copyright and/or license notice that predates the release of
# Scribus 1.3.2 for which a new license (GPL+exception) is in place.

"""End-to-end alpha contour, persistence and undo/redo regression."""

import os
import struct
import tempfile
import xml.etree.ElementTree as ET
import zlib

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def chunk(kind, data):
    return (struct.pack(">I", len(data)) + kind + data
            + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF))


def write_png(path, alpha):
    rows = []
    for y in range(16):
        row = bytearray(b"\x00")
        for x in range(16):
            row.extend((210, 50, 80))
            if alpha:
                row.append(255 if 4 <= x < 12 and 4 <= y < 12 else 0)
        rows.append(bytes(row))
    color_type = 6 if alpha else 2
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", 16, 16, 8, color_type, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(b"".join(rows)))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


def write_contrast_png(path):
    rows = []
    for y in range(16):
        row = bytearray(b"\x00")
        for x in range(16):
            value = 0 if 4 <= x < 12 and 4 <= y < 12 else 255
            row.extend((value, value, value))
        rows.append(bytes(row))
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", 16, 16, 8, 2, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(b"".join(rows)))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


def contour_state(path, frame):
    root = ET.parse(path).getroot()
    for element in root.iter("PageObject"):
        if element.get("AutoName") == frame:
            return element.get("ContourLinePath", ""), element.get("TEXTFLOWMODE", "")
    raise AssertionError("image frame missing from saved document")


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
alpha_path = os.path.abspath(os.path.join(output_dir, "alpha-contour.png"))
opaque_path = os.path.abspath(os.path.join(output_dir, "opaque-contour.png"))
contrast_path = os.path.abspath(os.path.join(output_dir, "contrast-contour.png"))
document_path = os.path.abspath(os.path.join(output_dir, "image-alpha-contour.sla"))
for path in (alpha_path, opaque_path, contrast_path, document_path):
    if os.path.exists(path):
        os.remove(path)
write_png(alpha_path, True)
write_png(opaque_path, False)
write_contrast_png(contrast_path)

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create document")
frame = scribus.createImage(72, 72, 160, 160, "Alpha Contour")
scribus.loadImage(alpha_path, frame)
scribus.setImageScale(10, 10, frame)
check(scribus.getTextFlowMode(frame) == 0, "initial text flow was not disabled")
scribus.saveDocAs(document_path)
original_contour = contour_state(document_path, frame)[0]
print("IMAGE_ALPHA_CONTOUR_QA: generating", flush=True)
check(scribus.generateImageAlphaContour(128, 4.0, True, frame),
    "could not generate contour")
check(scribus.getTextFlowMode(frame) == 3, "contour text flow was not enabled")
check(os.path.abspath(scribus.getImageFile(frame)) == alpha_path,
    "contour generation changed the image link")
scribus.saveDocAs(document_path)
generated_contour = contour_state(document_path, frame)[0]
check(generated_contour and generated_contour != original_contour,
    "generated contour was not serialized")

scribus.undo()
check(scribus.getTextFlowMode(frame) == 0, "undo did not restore text flow")
scribus.saveDocAs(document_path)
check(contour_state(document_path, frame)[0] == original_contour,
    "undo did not restore original contour")
scribus.redo()
check(scribus.getTextFlowMode(frame) == 3, "redo did not restore text flow")
scribus.saveDocAs(document_path)
check(contour_state(document_path, frame)[0] == generated_contour,
    "redo did not restore generated contour")

opaque_frame = scribus.createImage(260, 72, 160, 160, "Opaque Control")
scribus.loadImage(opaque_path, opaque_frame)
try:
    scribus.generateImageAlphaContour(128, 0.0, True, opaque_frame)
except scribus.ScribusException:
    pass
else:
    raise AssertionError("opaque image was accepted")
check(scribus.getTextFlowMode(opaque_frame) == 0,
    "failed contour generation changed text flow")

contrast_frame = scribus.createImage(72, 260, 160, 160, "Contrast Source")
scribus.loadImage(contrast_path, contrast_frame)
scribus.setImageScale(10, 10, contrast_frame)
check(scribus.generateImageContour("luminance", 128, 0.0, 0, 0.0, False,
    contrast_frame), "luminance contour failed")
check(scribus.getTextFlowMode(contrast_frame) == 0,
    "wrap=False changed text flow mode")
scribus.saveDocAs(document_path)
luminance_contour = contour_state(document_path, contrast_frame)[0]
check(luminance_contour, "luminance contour was empty")
check(scribus.generateImageContour("edge", 64, 0.0, 0, 0.0, False,
    contrast_frame), "contrast-edge contour failed")
scribus.saveDocAs(document_path)
check(contour_state(document_path, contrast_frame)[0] == luminance_contour,
    "contrast-edge contour disagreed with the high-contrast fixture")
try:
    scribus.generateImageContour("clip", 128, 0.0, 0, 0.0, False, contrast_frame)
except scribus.ScribusException:
    pass
else:
    raise AssertionError("frame without a clipping path was accepted")

scribus.saveDocAs(document_path)
scribus.closeDoc()
check(scribus.openDoc(document_path), "could not reopen document")
check(scribus.getTextFlowMode(frame) == 3, "text flow did not survive reopen")
check(contour_state(document_path, frame)[0] == generated_contour,
    "generated contour did not survive reopen")
print("IMAGE_ALPHA_CONTOUR_QA_PASSED", flush=True)
