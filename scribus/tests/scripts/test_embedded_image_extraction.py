#!/usr/bin/env python3

"""Exercise embedded-image extraction, persistence and undo/redo.

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


def png_chunk(chunk_type, data):
    return (
        struct.pack(">I", len(data))
        + chunk_type
        + data
        + struct.pack(">I", zlib.crc32(chunk_type + data) & 0xFFFFFFFF)
    )


def write_png(path):
    width = 11
    height = 7
    row = b"\x00" + bytes((42, 116, 203)) * width
    data = b"\x89PNG\r\n\x1a\n"
    data += png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0))
    data += png_chunk(b"IDAT", zlib.compress(row * height))
    data += png_chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


def read_bytes(path):
    with open(path, "rb") as binary_file:
        return binary_file.read()


def read_sla(path):
    data = read_bytes(path)
    return gzip.decompress(data) if data.startswith(b"\x1f\x8b") else data


def step(message):
    print("EMBEDDED_IMAGE_EXTRACTION_QA: " + message, flush=True)


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
source_path = os.path.abspath(os.path.join(output_dir, "embedded-source.png"))
copy_path = os.path.abspath(os.path.join(output_dir, "embedded-copy.png"))
relinked_path = os.path.abspath(os.path.join(output_dir, "embedded-relinked.png"))
document_path = os.path.abspath(os.path.join(output_dir, "embedded-image.sla"))
for old_path in (source_path, copy_path, relinked_path, document_path):
    if os.path.exists(old_path):
        os.remove(old_path)
write_png(source_path)
source_data = read_bytes(source_path)

step("creating an image frame and embedding its original bytes")
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
    "could not create the embedded-image test document",
)
frame = scribus.createImage(72, 72, 240, 180, "EmbeddedImageFrame")
scribus.loadImage(source_path, frame)
check(not scribus.isImageEmbedded(frame), "newly placed image was unexpectedly embedded")
check(scribus.embedImage(frame) is True, "image could not be embedded")
check(scribus.isImageEmbedded(frame), "embedded state was not reported")

step("saving and reopening the embedded image")
scribus.saveDocAs(document_path)
saved_data = read_sla(document_path)
check(b'IsInlineImage="1"' in saved_data, "embedded state was not written to the SLA")
check(b"ImageData=" in saved_data, "embedded bytes were not written to the SLA")
scribus.closeDoc()
check(scribus.openDoc(document_path), "could not reopen the embedded-image document")
check(scribus.isImageEmbedded(frame), "embedded state did not survive reopen")

step("extracting exact bytes without changing the document link")
check(
    scribus.extractEmbeddedImage(copy_path, False, False, frame) is True,
    "copy-only extraction failed",
)
check(read_bytes(copy_path) == source_data, "copy-only extraction changed the image bytes")
check(scribus.isImageEmbedded(frame), "copy-only extraction changed the embedded state")

step("extracting and relinking with undo and redo")
check(
    scribus.extractEmbeddedImage(relinked_path, True, False, frame) is True,
    "extract-and-relink failed",
)
check(read_bytes(relinked_path) == source_data, "extract-and-relink changed the image bytes")
check(not scribus.isImageEmbedded(frame), "frame remained embedded after relinking")
check(os.path.abspath(scribus.getImageFile(frame)) == relinked_path, "frame was not linked to the extracted file")

scribus.undo()
check(scribus.isImageEmbedded(frame), "undo did not restore the embedded image")
check(read_bytes(relinked_path) == source_data, "undo unexpectedly removed the user extraction")

os.remove(relinked_path)
scribus.redo()
check(not scribus.isImageEmbedded(frame), "redo did not restore the external link")
check(os.path.exists(relinked_path), "redo did not recreate a missing extracted file")
check(read_bytes(relinked_path) == source_data, "redo recreated different image bytes")
check(os.path.abspath(scribus.getImageFile(frame)) == relinked_path, "redo restored the wrong link")

scribus.undo()
check(scribus.isImageEmbedded(frame), "second undo did not restore the embedded image")
scribus.saveDocAs(document_path)
check(b'IsInlineImage="1"' in read_sla(document_path), "restored embedded state was not persisted")

print("EMBEDDED_IMAGE_EXTRACTION_QA_PASSED", flush=True)
