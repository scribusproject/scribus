#!/usr/bin/env python3

# For general Scribus (>=1.3.2) copyright and licensing information please
# refer to the COPYING file provided with the program.

"""EPUB linked-image metadata and preflight, including SLA reopen."""

import os
import struct
import tempfile
import zlib

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def chunk(kind, data):
    return (struct.pack(">I", len(data)) + kind + data
            + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF))


def write_png(path):
    pixels = (b"\x00" + b"\x30\x70\xb0" * 4) * 4
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", 4, 4, 8, 2, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(pixels))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
image_path = os.path.abspath(os.path.join(output_dir, "epub-linked-image.png"))
document_path = os.path.abspath(os.path.join(output_dir, "epub-linked-image.sla"))
jpeg_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "..",
                                   "resources", "templates", "textbased", "an_image.jpg"))
if os.path.exists(document_path):
    os.remove(document_path)
write_png(image_path)
check(os.path.isfile(jpeg_path), "repository JPEG fixture is missing")

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create document")
image = scribus.createImage(72, 72, 100, 100, "EPUB Illustration")
scribus.loadImage(image_path, image)
jpeg = scribus.createImage(300, 72, 100, 100, "EPUB Photograph")
scribus.loadImage(jpeg_path, jpeg)
empty = scribus.createImage(180, 72, 100, 100, "Empty EPUB Image")
text = scribus.createText(72, 200, 150, 60, "Not an Image")

alt = "Blue square & Telugu తెలుగు"
check(scribus.epubImageFramePreflight(image)["status"] == "unsupported-content",
      "an image without saved alt text was accepted")
scribus.setObjectAttributes([{
    "Name": "custom-note", "Type": "String", "Value": "keep",
    "Parameter": "", "Relationship": "", "RelationshipTo": "", "AutoAddTo": "",
}], image)
scribus.setEpubImageAltText(image, alt)
check(any(value["Name"] == "custom-note" and value["Value"] == "keep"
          for value in scribus.getObjectAttributes(image)),
      "setting alt text removed an unrelated attribute")
ready = scribus.epubImageFramePreflight(image, alt)
check(ready["status"] == "ready", ready)
check(ready["mediaType"] == "image/png", ready)
check(ready["byteSize"] == os.path.getsize(image_path), ready)
check(ready["readingOrder"] == -1, ready)
scribus.setEpubImageReadingOrder(image, 1)
check(scribus.epubImageFramePreflight(image)["readingOrder"] == 1,
      "image rank was not saved")
check(scribus.epubImageFramePreflight(image)["status"] == "ready",
      "saved alt text was not used by preflight")
photograph = scribus.epubImageFramePreflight(jpeg, "Portrait photograph")
check(photograph["status"] == "ready", photograph)
check(photograph["mediaType"] == "image/jpeg", photograph)
check(photograph["byteSize"] == os.path.getsize(jpeg_path), photograph)
scribus.setEpubUseImageFrameCrop(jpeg, True)
check(scribus.epubImageFramePreflight(jpeg, "Portrait photograph")["status"] == "unsupported-content",
      "JPEG frame crop was silently approximated")
scribus.setEpubUseImageFrameCrop(jpeg, False)
check(scribus.epubImageFramePreflight(jpeg, "Portrait photograph")["status"] == "ready",
      "disabling unsupported JPEG crop did not restore preflight")
scribus.setEpubImageReadingOrder(jpeg, 2)
check(scribus.epubImageFramePreflight(jpeg, "Portrait photograph")["readingOrder"] == 2,
      "JPEG rank was not saved")
check(any(value["Name"] == "custom-note" and value["Value"] == "keep"
          for value in scribus.getObjectAttributes(image)),
      "setting image rank removed an unrelated attribute")
check(scribus.epubImageFramePreflight(image, " ")["status"] == "unsupported-content",
      "empty alt text was accepted")
check(scribus.epubImageFramePreflight(image, "bad\x00alt")["status"] == "unsupported-content",
      "invalid XML alt text was accepted")
check(scribus.epubImageFramePreflight(empty, alt)["status"] == "unsupported-content",
      "an empty image frame was accepted")
check(scribus.epubImageFramePreflight(text, alt)["status"] == "unsupported-content",
      "a text frame was accepted as an image")
for bad_alt in (" ", "bad\x00alt"):
    try:
        scribus.setEpubImageAltText(image, bad_alt)
    except ValueError:
        pass
    else:
        raise AssertionError("invalid alt text was stored")
check(scribus.epubImageFramePreflight(image)["status"] == "ready",
      "a rejected edit changed saved alt text")
try:
    scribus.setEpubImageAltText(text, alt)
except ValueError:
    pass
else:
    raise AssertionError("alt text was stored on a text frame")
for bad_rank in (-1, 2 ** 40):
    try:
        scribus.setEpubImageReadingOrder(image, bad_rank)
    except ValueError:
        pass
    else:
        raise AssertionError("invalid image rank was accepted")
try:
    scribus.setEpubImageReadingOrder(image, True)
except TypeError:
    pass
else:
    raise AssertionError("boolean image rank was accepted")
try:
    scribus.setEpubImageReadingOrder(text, 3)
except ValueError:
    pass
else:
    raise AssertionError("text frame was ranked as an image")
check(scribus.epubImageFramePreflight(image)["readingOrder"] == 1,
      "rejected image-rank edit changed the saved value")
try:
    scribus.epubImageFramePreflight(image, 42)
except TypeError:
    pass
else:
    raise AssertionError("non-string alt text was accepted")

scribus.saveDocAs(document_path)
scribus.closeDoc()
scribus.openDoc(document_path)
reopened = scribus.epubImageFramePreflight(image)
check(reopened["status"] == "ready", reopened)
check(reopened["mediaType"] == "image/png", reopened)
check(reopened["byteSize"] == ready["byteSize"], reopened)
check(reopened["readingOrder"] == 1, reopened)
check(scribus.epubImageFramePreflight(jpeg, "Portrait photograph")["readingOrder"] == 2,
      "JPEG rank did not survive SLA reopen")
check(any(value["Name"] == "custom-note" and value["Value"] == "keep"
          for value in scribus.getObjectAttributes(image)),
      "save/reopen lost an unrelated attribute")
with tempfile.TemporaryDirectory(dir=output_dir) as holding_dir:
    held_path = os.path.join(holding_dir, "epub-linked-image.png")
    os.rename(image_path, held_path)
    try:
        missing = scribus.epubImageFramePreflight(image, alt)
        check(missing["status"] == "unsupported-content", missing)
    finally:
        os.rename(held_path, image_path)
scribus.setEpubImageAltText(image, "")
check(scribus.epubImageFramePreflight(image)["status"] == "unsupported-content",
      "clearing saved alt text did not make preflight fail")
check(not any(value["Name"] == "scribus:epub-image-alt-text"
              for value in scribus.getObjectAttributes(image)),
      "clearing alt text left a reserved attribute")
scribus.setEpubImageReadingOrder(image, 0)
check(scribus.epubImageFramePreflight(image, alt)["readingOrder"] == -1,
      "clearing image rank did not remove it")
check(not any(value["Name"] == "scribus:epub-reading-order"
              for value in scribus.getObjectAttributes(image)),
      "clearing image rank left a reserved attribute")

print("EPUB_IMAGE_FRAME_PREFLIGHT_PASSED", flush=True)
