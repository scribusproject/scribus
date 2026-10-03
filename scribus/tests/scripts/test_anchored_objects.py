#!/usr/bin/env python3

"""
Exercise anchored images and tables, reflow, validation and SLA persistence.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import gzip
import os
import struct
import tempfile
import zlib

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def expect_error(function, message):
    try:
        function()
    except Exception:
        return
    raise AssertionError(message)


def read_sla(path):
    with open(path, "rb") as saved_file:
        data = saved_file.read()
    return gzip.decompress(data) if data.startswith(b"\x1f\x8b") else data


def step(message):
    print("ANCHORED_OBJECT_QA: " + message, flush=True)


def write_test_png(path):
    def chunk(kind, data):
        return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF)

    pixels = (b"\x00" + bytes((54, 116, 205)) * 8) * 6
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", 8, 6, 8, 2, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(pixels))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as output:
        output.write(data)


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
output_path = os.path.join(output_dir, "scribus_anchored_object_test.sla")
image_path = os.path.abspath(os.path.join(output_dir, "scribus_anchored_object_test.png"))
if os.path.exists(output_path):
    os.remove(output_path)
write_test_png(image_path)

step("creating a story, image frame and table")
check(
    scribus.newDocument(
        scribus.PAPER_A4,
        (36, 36, 36, 36),
        scribus.PORTRAIT,
        1,
        scribus.UNIT_POINTS,
        scribus.PAGE_1,
        0,
        1,
    ),
    "could not create the anchored-object test document",
)
story = scribus.createText(50, 50, 480, 700, "AnchoredStory")
paragraphs = [
    "Paragraph %02d contains enough words to form a stable line of body text." % index
    for index in range(1, 26)
]
scribus.setText("\n".join(paragraphs), story)
image = scribus.createImage(80, 80, 96, 72, "AnchoredImage")
scribus.loadImage(image_path, image)
check(os.path.abspath(scribus.getImageFile(image)) == image_path, "source image was not loaded")
table = scribus.createTable(80, 180, 180, 70, 2, 3, "AnchoredTable")
check(scribus.getTableRows(table) == 2, "table rows were not available before anchoring")
scribus.setCellText(0, 0, "Anchored table content", table)

step("inserting both object types into specific story positions")
image_id = scribus.insertAnchoredObject(image, story, 260)
table_id = scribus.insertAnchoredObject(table, story, 620)
check(isinstance(image_id, int) and image_id >= 0, "image anchor did not return an identifier")
check(isinstance(table_id, int) and table_id >= 0, "table anchor did not return an identifier")
check(image_id != table_id, "anchored objects received duplicate identifiers")
inline_rect = scribus.getAnchoredObjectRect(image, story)
check(inline_rect[2] > 0 and inline_rect[3] > 0,
      "inline image has no selectable canvas bounds")

step("checking that normal image-frame operations remain available while anchored")
scribus.setImageScale(1.25, 0.85, image)
scribus.setImageOffset(4.0, 6.0, image)
check(all(abs(a - b) < 0.01 for a, b in zip(scribus.getImageScale(image), (1.25, 0.85))),
      "anchoring disabled image scaling")
check(all(abs(a - b) < 0.01 for a, b in zip(scribus.getImageOffset(image), (4.0, 6.0))),
      "anchoring disabled image crop offsets")
replacement_path = os.path.abspath(os.path.join(output_dir, "scribus_anchored_object_replacement.png"))
write_test_png(replacement_path)
old_scale = scribus.getImageScale(image)
old_offset = scribus.getImageOffset(image)
check(scribus.relinkImage(replacement_path, image), "anchoring disabled image relinking")
check(os.path.abspath(scribus.getImageFile(image)) == replacement_path,
      "anchored image relink did not change the source")
check(all(abs(a - b) < 0.01 for a, b in zip(scribus.getImageScale(image), old_scale)),
      "anchored image relink changed scale")
check(all(abs(a - b) < 0.01 for a, b in zip(scribus.getImageOffset(image), old_offset)),
      "anchored image relink changed crop offsets")
check(scribus.relinkImage(image_path, image), "anchored image could not relink to its original source")
scribus.loadImage(replacement_path, image)
check(os.path.abspath(scribus.getImageFile(image)) == replacement_path,
      "anchoring disabled image replacement")
scribus.loadImage(image_path, image)

step("configuring custom image positioning and table above-line positioning")
image_options = {
    "mode": scribus.ANCHOR_MODE_CUSTOM,
    "horizontalReference": scribus.ANCHOR_HREF_COLUMN,
    "verticalReference": scribus.ANCHOR_VREF_LINE,
    "horizontalAlignment": scribus.ANCHOR_HALIGN_RIGHT,
    "verticalAlignment": scribus.ANCHOR_VALIGN_BASELINE,
    "wrapMode": scribus.ANCHOR_WRAP_BOUNDING_BOX,
    "xOffset": -8.0,
    "yOffset": 3.0,
    "wrapLeft": 6.0,
    "wrapTop": 4.0,
    "wrapRight": 6.0,
    "wrapBottom": 4.0,
    "keepWithinBounds": True,
    "preventManualPositioning": True,
}
scribus.setAnchoredObjectOptions(image, image_options)
scribus.setAnchoredObjectOptions(
    table,
    {
        "mode": scribus.ANCHOR_MODE_ABOVE_LINE,
        "horizontalReference": scribus.ANCHOR_HREF_COLUMN,
        "horizontalAlignment": scribus.ANCHOR_HALIGN_CENTER,
        "xOffset": 0.0,
        "yOffset": 5.0,
    },
)
second_image_id = scribus.insertAnchoredObject(image, story, 1000)
check(second_image_id == image_id, "reusing an inline object changed its identifier")
stored_image_options = scribus.getAnchoredObjectOptions(image)
for key, expected in image_options.items():
    actual = stored_image_options[key]
    if isinstance(expected, float):
        check(abs(actual - expected) < 0.01, "image option %s was not applied" % key)
    else:
        check(actual == expected, "image option %s was not applied" % key)
check(
    scribus.getAnchoredObjectOptions(table)["mode"] == scribus.ANCHOR_MODE_ABOVE_LINE,
    "table did not enter above-line mode",
)

step("checking that inserting text above moves both anchors with the story")
image_rect_before = scribus.getAnchoredObjectRect(image, story)
image_rects_before = scribus.getAnchoredObjectRects(image, story)
table_rect_before = scribus.getAnchoredObjectRect(table, story)
check(len(image_rects_before) == 2, "two occurrences of the image did not receive independent anchors")
scribus.insertText(
    "Inserted heading line one.\nInserted heading line two.\nInserted heading line three.\n",
    0,
    story,
)
image_rect_after = scribus.getAnchoredObjectRect(image, story)
image_rects_after = scribus.getAnchoredObjectRects(image, story)
table_rect_after = scribus.getAnchoredObjectRect(table, story)
step("image occurrences before=%r after=%r" % (image_rects_before, image_rects_after))
check(len(image_rects_after) == 2, "an image occurrence was lost during reflow")
check(image_rect_after[1] > image_rect_before[1] + 10.0, "image did not move after story reflow")
check(
    abs(image_rects_after[1][1] - image_rects_before[1][1]) > 10.0,
    "second image occurrence did not move after reflow",
)
check(table_rect_after[1] > table_rect_before[1] + 10.0, "table did not move after story reflow")
check(scribus.getCellText(0, 0, table) == "Anchored table content",
      "anchoring removed access to table cell content")
scribus.setCellText(1, 1, "Edited while anchored", table)
check(scribus.getCellText(1, 1, table) == "Edited while anchored",
      "anchored table cell could not be edited")
table_size_before = scribus.getSize(table)
scribus.sizeObject(table_size_before[0] + 20.0, table_size_before[1] + 10.0, table)
table_size_after = scribus.getSize(table)
check(table_size_after[0] > table_size_before[0] + 15.0,
      "resizing an anchored table did not resize its frame")
scribus.undo()
check(abs(scribus.getSize(table)[0] - table_size_before[0]) < 0.1,
      "undo did not restore the anchored table width")
scribus.redo()
check(abs(scribus.getSize(table)[0] - table_size_after[0]) < 0.1,
      "redo did not restore the anchored table width")
check(abs(image_rect_after[2] - 96.0) < 2.0, "image anchor width changed during reflow")
check(
    scribus.getAnchoredObjectRects(image, story) == image_rects_after,
    "repeated layout changed anchored-object geometry",
)

step("checking custom table wrap and offsets")
scribus.setAnchoredObjectOptions(table, {
    "mode": scribus.ANCHOR_MODE_CUSTOM,
    "wrapMode": scribus.ANCHOR_WRAP_BOUNDING_BOX,
    "xOffset": 4.0,
    "yOffset": 7.0,
    "wrapLeft": 3.0,
    "wrapTop": 2.0,
    "wrapRight": 5.0,
    "wrapBottom": 4.0,
})
table_custom_rect = scribus.getAnchoredObjectRect(table, story)
check(table_custom_rect[2] > 0 and table_custom_rect[3] > 0,
      "custom anchored table lost its selectable bounds")
check(scribus.getAnchoredObjectOptions(table)["wrapLeft"] == 3.0,
      "table text-wrap offset was not applied")

step("rejecting malformed options and invalid destinations")
expect_error(
    lambda: scribus.setAnchoredObjectOptions(image, {"unknownOption": 1}),
    "an unknown option was accepted",
)
expect_error(
    lambda: scribus.setAnchoredObjectOptions(image, {"mode": 99}),
    "an invalid anchor mode was accepted",
)
expect_error(
    lambda: scribus.setAnchoredObjectOptions(image, {"wrapLeft": -1.0}),
    "a negative wrap offset was accepted",
)
expect_error(
    lambda: scribus.setAnchoredObjectOptions(image, {"xOffset": float("nan")}),
    "a non-finite position offset was accepted",
)
expect_error(
    lambda: scribus.setAnchoredObjectOptions(image, {"wrapRight": float("inf")}),
    "a non-finite wrap offset was accepted",
)
shape = scribus.createRect(400, 760, 20, 20, "NotATextFrame")
expect_error(
    lambda: scribus.insertAnchoredObject(shape, image, 0),
    "an inline image was accepted as a text destination",
)

step("checking current-format SLA persistence and reopen")
scribus.saveDocAs(output_path)
saved_data = read_sla(output_path)
check(b'ANCHORMODE="2"' in saved_data or b'AnchorMode="2"' in saved_data, "custom mode was not serialized")
check(b'ANCHORWRAP="1"' in saved_data or b'AnchorWrapMode="1"' in saved_data, "wrap mode was not serialized")
check(b'ANCHORKEEP="1"' in saved_data or b'AnchorKeepWithinBounds="1"' in saved_data, "bounds option was not serialized")
check(b'ImageFileName="scribus_anchored_object_test.png"' in saved_data, "anchored image link was not serialized")
image_marker = ('Object="%d"' % image_id).encode("ascii")
check(saved_data.count(image_marker) == 2, "both image anchor markers were not serialized")
scribus.closeDoc()
check(scribus.openDoc(output_path), "could not reopen the anchored-object document")
reopened_options = scribus.getAnchoredObjectOptions(image)
check(reopened_options["mode"] == scribus.ANCHOR_MODE_CUSTOM, "custom mode did not survive reopen")
check(
    reopened_options["wrapMode"] == scribus.ANCHOR_WRAP_BOUNDING_BOX,
    "wrap mode did not survive reopen",
)
check(reopened_options["keepWithinBounds"] is True, "bounds option did not survive reopen")
reopened_table_options = scribus.getAnchoredObjectOptions(table)
check(reopened_table_options["wrapMode"] == scribus.ANCHOR_WRAP_BOUNDING_BOX,
      "table text wrap did not survive reopen")
check(abs(reopened_table_options["wrapRight"] - 5.0) < 0.01,
      "table text-wrap offset did not survive reopen")
check(scribus.getCellText(0, 0, table) == "Anchored table content",
      "anchored table cell content did not survive reopen")
check(scribus.getCellText(1, 1, table) == "Edited while anchored",
      "table edits made after anchoring did not survive reopen")
reopened_rect = scribus.getAnchoredObjectRect(image, story)
check(len(scribus.getAnchoredObjectRects(image, story)) == 2, "both image occurrences did not survive reopen")
step("resolved image rectangle before reopen=%r after reopen=%r" % (image_rect_after, reopened_rect))
check(abs(reopened_rect[1] - image_rect_after[1]) < 2.0, "resolved image position changed after reopen")

print("ANCHORED_OBJECT_QA_PASSED", flush=True)
