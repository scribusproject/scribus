#!/usr/bin/env python3

# For general Scribus (>=1.3.2) copyright and licensing information please
# refer to the COPYING file provided with the program. Following this notice
# may exist a copyright and/or license notice that predates the release of
# Scribus 1.3.2 for which a new license (GPL+exception) is in place.

"""End-to-end checks for exact-source image replacement and grouped undo."""

import os
import struct
import tempfile
import zlib

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def png_chunk(chunk_type, data):
    return (struct.pack(">I", len(data)) + chunk_type + data
            + struct.pack(">I", zlib.crc32(chunk_type + data) & 0xFFFFFFFF))


def write_png(path, rgb):
    row = b"\x00" + bytes(rgb) * 8
    data = b"\x89PNG\r\n\x1a\n"
    data += png_chunk(b"IHDR", struct.pack(">IIBBBBB", 8, 8, 8, 2, 0, 0, 0))
    data += png_chunk(b"IDAT", zlib.compress(row * 8))
    data += png_chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


def link(name):
    return os.path.abspath(scribus.getImageFile(name))


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
source = os.path.abspath(os.path.join(output_dir, "replace-source.png"))
replacement = os.path.abspath(os.path.join(output_dir, "replace-target.png"))
control = os.path.abspath(os.path.join(output_dir, "replace-control.png"))
missing = os.path.abspath(os.path.join(output_dir, "replace-missing.png"))
document = os.path.abspath(os.path.join(output_dir, "image-link-replacement.sla"))
for path in (source, replacement, control, missing, document):
    if os.path.exists(path):
        os.remove(path)
write_png(source, (230, 40, 40))
write_png(replacement, (30, 90, 220))
write_png(control, (30, 180, 60))

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create document")
print("IMAGE_LINK_REPLACEMENT_QA: creating frames", flush=True)
first = scribus.createImage(50, 50, 160, 100, "Replace First")
second = scribus.createImage(230, 50, 160, 100, "Replace Second")
other = scribus.createImage(50, 180, 160, 100, "Unchanged Control")
embedded = scribus.createImage(230, 180, 160, 100, "Embedded Control")
for frame, path in ((first, source), (second, source), (other, control), (embedded, source)):
    scribus.loadImage(path, frame)
check(scribus.embedImage(embedded), "could not create embedded control")
scribus.setImageScale(1.25, 0.85, first)
scribus.setImageOffset(12, 19, first)
scale = scribus.getImageScale(first)
offset = scribus.getImageOffset(first)

scribus.createMasterPage("Replacement Master")
print("IMAGE_LINK_REPLACEMENT_QA: creating master frame", flush=True)
scribus.editMasterPage("Replacement Master")
master_frame = scribus.createImage(50, 50, 160, 100, "Master Source")
scribus.loadImage(source, master_frame)
scribus.closeMasterPage()
scribus.gotoPage(1)

print("IMAGE_LINK_REPLACEMENT_QA: dry run", flush=True)
check(scribus.replaceImageLinks(source, replacement, True, "page") == (2, 0, 0),
    "page-scoped preview included a master frame")
check(scribus.replaceImageLinks(source, replacement, True, "masters") == (1, 0, 0),
    "master-scoped preview included document frames")
check(scribus.replaceImageLinks(source, replacement, True) == (3, 0, 0),
    "dry run did not count document and master frames")
check(link(first) == source and link(second) == source, "dry run changed image links")
print("IMAGE_LINK_REPLACEMENT_QA: invalid replacement", flush=True)
check(scribus.replaceImageLinks(source, missing) == (3, 0, 3),
    "missing target did not report failed loads")
check(link(first) == source and link(second) == source, "failed replacement changed image links")

print("IMAGE_LINK_REPLACEMENT_QA: valid replacement", flush=True)
check(scribus.replaceImageLinks(source, replacement) == (3, 3, 0),
    "valid replacement did not update every matching frame")
check(link(first) == replacement and link(second) == replacement,
    "document frames were not replaced")
check(link(other) == control and scribus.isImageEmbedded(embedded),
    "unrelated or embedded frames changed")
check(all(abs(a - b) < 0.01 for a, b in zip(scribus.getImageScale(first), scale)),
    "replacement changed image scale")
check(all(abs(a - b) < 0.01 for a, b in zip(scribus.getImageOffset(first), offset)),
    "replacement changed image offset")
scribus.editMasterPage("Replacement Master")
check(link(master_frame) == replacement, "master-page frame was not replaced")
scribus.closeMasterPage()

print("IMAGE_LINK_REPLACEMENT_QA: undo", flush=True)
scribus.undo()
check(link(first) == source and link(second) == source, "one undo did not restore document links")
scribus.editMasterPage("Replacement Master")
check(link(master_frame) == source, "one undo did not restore master link")
scribus.closeMasterPage()
print("IMAGE_LINK_REPLACEMENT_QA: redo", flush=True)
scribus.redo()
check(link(first) == replacement and link(second) == replacement, "redo did not restore links")
scribus.editMasterPage("Replacement Master")
check(link(master_frame) == replacement, "redo did not restore master link")
scribus.closeMasterPage()

print("IMAGE_LINK_REPLACEMENT_QA: save and reopen", flush=True)
scribus.saveDocAs(document)
scribus.closeDoc()
check(scribus.openDoc(document), "could not reopen document")
check(link(first) == replacement and link(second) == replacement,
    "replaced links did not survive reopen")
scribus.editMasterPage("Replacement Master")
check(link(master_frame) == replacement, "master replacement did not survive reopen")
scribus.closeMasterPage()
print("IMAGE_LINK_REPLACEMENT_QA_PASSED", flush=True)
