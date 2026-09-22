#!/usr/bin/env python3

"""End-to-end non-destructive ICC RGB image to CMYK TIFF export test.

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


def png_chunk(kind, data):
    return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF)


def write_png(path, transparent=False):
    width, height = 8, 6
    pixel = bytes((54, 116, 205, 128)) if transparent else bytes((54, 116, 205))
    row = b"\x00" + pixel * width
    data = b"\x89PNG\r\n\x1a\n"
    data += png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, height, 8, 6 if transparent else 2, 0, 0, 0))
    data += png_chunk(b"IDAT", zlib.compress(row * height))
    data += png_chunk(b"IEND", b"")
    with open(path, "wb") as output:
        output.write(data)


def tiff_tags(path):
    with open(path, "rb") as source:
        data = source.read()
    check(data[:2] in (b"II", b"MM"), "TIFF byte order missing")
    endian = "<" if data[:2] == b"II" else ">"
    check(struct.unpack_from(endian + "H", data, 2)[0] == 42, "invalid TIFF marker")
    ifd = struct.unpack_from(endian + "I", data, 4)[0]
    count = struct.unpack_from(endian + "H", data, ifd)[0]
    tags = {}
    for index in range(count):
        offset = ifd + 2 + index * 12
        tag, kind, length, value = struct.unpack_from(endian + "HHII", data, offset)
        tags[tag] = (kind, length, value)
    check(34675 in tags, "converted TIFF has no embedded ICC profile")
    kind, length, offset = tags[34675]
    check(kind == 7 and length > 128, "invalid ICC profile tag")
    check(data[offset + 36 : offset + 40] == b"acsp", "embedded profile has no ICC signature")
    return tags, endian


def short_tag(tags, endian, number):
    kind, count, value = tags[number]
    check(kind == 3 and count == 1, "unexpected TIFF tag type")
    return value & 0xFFFF if endian == "<" else value >> 16


def expect_export_error(path, frame):
    try:
        scribus.exportImageAsCMYKCopy(path, frame)
    except scribus.ScribusException:
        pass
    else:
        raise AssertionError("invalid export was accepted: " + path)


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
source_path = os.path.abspath(os.path.join(output_dir, "cmyk-export-rgb.png"))
alpha_path = os.path.abspath(os.path.join(output_dir, "cmyk-export-alpha.png"))
result_path = os.path.abspath(os.path.join(output_dir, "cmyk-export-result.tif"))
bad_extension_path = os.path.abspath(os.path.join(output_dir, "cmyk-export-bad.png"))
alpha_result_path = os.path.abspath(os.path.join(output_dir, "cmyk-export-alpha.tif"))
embedded_result_path = os.path.abspath(os.path.join(output_dir, "cmyk-export-embedded.tif"))
document_path = os.path.abspath(os.path.join(output_dir, "cmyk-export.sla"))
for path in (source_path, alpha_path, result_path, bad_extension_path, alpha_result_path,
    embedded_result_path, document_path):
    if os.path.exists(path):
        os.remove(path)
write_png(source_path)
write_png(alpha_path, transparent=True)
with open(source_path, "rb") as source:
    source_bytes = source.read()

check(scribus.newDocument(scribus.PAPER_A4, (20, 20, 20, 20), scribus.PORTRAIT,
    1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1), "could not create document")
frame = scribus.createImage(40, 40, 180, 120, "RGB Source")
scribus.loadImage(source_path, frame)
check(scribus.getImageColorSpace(frame) == scribus.CSPACE_RGB, "source did not load as RGB")
expect_export_error(result_path, frame)
check(not os.path.exists(result_path), "disabled color management still wrote a file")

# The Scripter has no color-management toggle. Save a fixture with the normal
# SLA attribute enabled, then reopen through the standard document loader.
scribus.saveDocAs(document_path)
scribus.closeDoc()
with open(document_path, "rb") as saved:
    saved_data = saved.read()
compressed = saved_data.startswith(b"\x1f\x8b")
xml = gzip.decompress(saved_data) if compressed else saved_data
check(xml.count(b'ColorManagementActive="0"') == 1, "expected disabled color management in the fixture")
xml = xml.replace(b'ColorManagementActive="0"', b'ColorManagementActive="1"', 1)
with open(document_path, "wb") as saved:
    saved.write(gzip.compress(xml) if compressed else xml)
check(scribus.openDoc(document_path), "could not reopen color-managed fixture")
check(scribus.getImageColorSpace(frame) == scribus.CSPACE_RGB, "source did not reopen as RGB")

check(scribus.exportImageAsCMYKCopy(result_path, frame) is True, "CMYK export failed")
check(os.path.exists(result_path), "CMYK file was not created")
check(os.path.abspath(scribus.getImageFile(frame)) == source_path, "export changed the original frame link")
with open(source_path, "rb") as source:
    check(source.read() == source_bytes, "export changed the source image")
tags, endian = tiff_tags(result_path)
check(short_tag(tags, endian, 262) == 5, "TIFF is not separated CMYK")
check(short_tag(tags, endian, 277) == 4, "TIFF does not have four ink channels")
with open(result_path, "rb") as result:
    result_bytes = result.read()

converted_frame = scribus.createImage(40, 180, 180, 120, "CMYK Result")
scribus.loadImage(result_path, converted_frame)
check(scribus.getImageColorSpace(converted_frame) == scribus.CSPACE_CMYK,
    "Scribus could not reopen the exported image as CMYK")

expect_export_error(result_path, frame)
with open(result_path, "rb") as result:
    check(result.read() == result_bytes, "existing CMYK file was overwritten")
expect_export_error(bad_extension_path, frame)
check(not os.path.exists(bad_extension_path), "export wrote to a misleading extension")

alpha_frame = scribus.createImage(240, 40, 180, 120, "Alpha Source")
scribus.loadImage(alpha_path, alpha_frame)
expect_export_error(alpha_result_path, alpha_frame)
check(not os.path.exists(alpha_result_path), "transparent export left a file behind")

check(scribus.embedImage(frame), "could not embed source image")
check(scribus.isImageEmbedded(frame), "source frame did not become embedded")
check(scribus.exportImageAsCMYKCopy(embedded_result_path, frame), "embedded RGB image export failed")
check(scribus.isImageEmbedded(frame), "export changed the embedded frame")
embedded_tags, embedded_endian = tiff_tags(embedded_result_path)
check(short_tag(embedded_tags, embedded_endian, 262) == 5, "embedded image export is not CMYK")

print("IMAGE_CMYK_EXPORT_QA_PASSED", flush=True)
