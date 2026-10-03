#!/usr/bin/env python3

# For general Scribus (>=1.3.2) copyright and licensing information please
# refer to the COPYING file provided with the program.

"""Saved-SLA mixed text/image EPUB preflight and limited export."""

import os
import struct
import xml.etree.ElementTree as ET
import zlib
import zipfile

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def chunk(kind, data):
    return (struct.pack(">I", len(data)) + kind + data
            + struct.pack(">I", zlib.crc32(kind + data) & 0xFFFFFFFF))


def write_png(path):
    pixels = (b"\x00" + b"\x20\x80\xe0" * 4) * 4
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", 4, 4, 8, 2, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(pixels))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


def write_pattern_png(path):
    pixels = b"".join(b"\x00" + b"".join(bytes((x * 40, y * 40, 10))
                    for x in range(4)) for y in range(4))
    data = b"\x89PNG\r\n\x1a\n"
    data += chunk(b"IHDR", struct.pack(">IIBBBBB", 4, 4, 8, 2, 0, 0, 0))
    data += chunk(b"IDAT", zlib.compress(pixels))
    data += chunk(b"IEND", b"")
    with open(path, "wb") as image_file:
        image_file.write(data)


def png_pixels(data):
    check(data.startswith(b"\x89PNG\r\n\x1a\n"), "cropped asset is not PNG")
    offset, image_data, width, height, channels = 8, b"", 0, 0, 0
    while offset < len(data):
        length = struct.unpack_from(">I", data, offset)[0]
        kind = data[offset + 4:offset + 8]
        payload = data[offset + 8:offset + 8 + length]
        offset += length + 12
        if kind == b"IHDR":
            width, height, depth, colour, _, _, _ = struct.unpack(">IIBBBBB", payload)
            check(depth == 8 and colour in (2, 6), "unexpected cropped PNG pixel format")
            channels = 3 if colour == 2 else 4
        elif kind == b"IDAT":
            image_data += payload
        elif kind == b"IEND":
            break
    raw = zlib.decompress(image_data)
    rows, previous, position, stride = [], bytearray(width * channels), 0, width * channels
    for _ in range(height):
        mode = raw[position]
        position += 1
        encoded = raw[position:position + stride]
        position += stride
        row = bytearray(stride)
        for index, value in enumerate(encoded):
            left = row[index - channels] if index >= channels else 0
            up = previous[index]
            upper_left = previous[index - channels] if index >= channels else 0
            if mode == 0:
                predictor = 0
            elif mode == 1:
                predictor = left
            elif mode == 2:
                predictor = up
            elif mode == 3:
                predictor = (left + up) // 2
            elif mode == 4:
                base = left + up - upper_left
                distances = (abs(base - left), abs(base - up), abs(base - upper_left))
                predictor = (left, up, upper_left)[distances.index(min(distances))]
            else:
                raise AssertionError("unsupported PNG row filter")
            row[index] = (value + predictor) & 255
        rows.append([tuple(row[index:index + channels][:3])
                     for index in range(0, stride, channels)])
        previous = row
    return width, height, rows


def text_rank(frame, rank):
    scribus.setObjectAttributes([{
        "Name": "scribus:epub-reading-order", "Type": "Integer",
        "Value": str(rank), "Parameter": "", "Relationship": "",
        "RelationshipTo": "", "AutoAddTo": "",
    }], frame)


output_dir = os.environ["SCRIBUS_TEST_OUTPUT_DIR"]
os.makedirs(output_dir, exist_ok=True)
image_path = os.path.abspath(os.path.join(output_dir, "epub-mixed-image.png"))
document_path = os.path.abspath(os.path.join(output_dir, "epub-mixed-order.sla"))
epub_path = os.path.abspath(os.path.join(output_dir, "epub-mixed-order.epub"))
image_only_path = os.path.abspath(os.path.join(output_dir, "epub-image-only.epub"))
crop_image_path = os.path.abspath(os.path.join(output_dir, "epub-crop-source.png"))
crop_document_path = os.path.abspath(os.path.join(output_dir, "epub-crop.sla"))
crop_path = os.path.abspath(os.path.join(output_dir, "epub-crop.epub"))
decorative_path = os.path.abspath(os.path.join(output_dir, "epub-decorative-only.epub"))
decorative_document_path = os.path.abspath(os.path.join(output_dir, "epub-decorative-only.sla"))
rejected_path = os.path.abspath(os.path.join(output_dir, "epub-mixed-rejected.epub"))
for generated_path in (document_path, epub_path, image_only_path, decorative_path,
                       decorative_document_path, crop_document_path, crop_path, rejected_path):
    if os.path.exists(generated_path):
        os.remove(generated_path)
write_png(image_path)
write_pattern_png(crop_image_path)

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create mixed EPUB test document")
first = scribus.createText(50, 50, 240, 50, "EPUB Before")
second = scribus.createText(50, 250, 240, 50, "EPUB After")
image = scribus.createImage(50, 115, 100, 100, "EPUB Picture")
scribus.setText("Before picture", first)
scribus.setText("After picture", second)
scribus.loadImage(image_path, image)
text_rank(first, 1)
text_rank(second, 3)
scribus.setEpubImageAltText(image, "Blue square & తెలుగు")
scribus.setEpubImageCaption(image, "Figure 1: blue & తెలుగు")
scribus.setEpubImageCaptionAlignment(image, "center")
scribus.setEpubImageWidthPercent(image, 55)
scribus.setEpubImageReadingOrder(image, 2)
scribus.setImageOffset(2, 3, image)


def check_mixed():
    result = scribus.epubMixedReadingOrderPreflight()
    check(result["status"] == "ready", result)
    check(result["blockKinds"] == ["paragraph", "image", "paragraph"], result)
    check(result["texts"] == ["Before picture", "Blue square & తెలుగు", "After picture"], result)
    check(result["imageIndices"] == [-1, 0, -1], result)
    check(result["imageMediaTypes"] == ["image/png"], result)
    check(len(result["lineSpacingWarnings"]) == 1, result)
    warnings = result["imageFidelityWarnings"]
    check(len(warnings) == 1 and warnings[0]["frameName"] == image, warnings)
    check({"frame-geometry", "colour-management", "image-offset"}
          <= set(warnings[0]["omitted"]), warnings)
    check(all(issue["severity"] == "warning" for issue in result["issues"]), result)
    check({"image-fidelity", "line-spacing"} <=
          {issue["code"] for issue in result["issues"]}, result)


check_mixed()
check(scribus.epubImageFramePreflight(image)["caption"] == "Figure 1: blue & తెలుగు",
      "saved EPUB caption missing before document save")
check(scribus.epubImageFramePreflight(image)["captionAlignment"] == "center",
      "saved EPUB caption alignment missing before document save")
check(scribus.epubImageFramePreflight(image)["widthPercent"] == 55,
      "saved EPUB figure width missing before document save")
check(scribus.epubReadingOrderPreflight()["status"] == "unsupported-content",
      "text-only EPUB path accepted an image frame")
check(scribus.saveDocAs(document_path), "could not save mixed EPUB test document")
scribus.closeDoc()
scribus.openDoc(document_path)
check_mixed()
check(scribus.epubImageFramePreflight(image)["caption"] == "Figure 1: blue & తెలుగు",
      "EPUB caption did not survive SLA save/reopen")
check(scribus.epubImageFramePreflight(image)["captionAlignment"] == "center",
      "EPUB caption alignment did not survive SLA save/reopen")
check(scribus.epubImageFramePreflight(image)["widthPercent"] == 55,
      "EPUB figure width did not survive SLA save/reopen")

publication = ("urn:uuid:3e593311-a28d-4e56-8c4f-283ecfb70c91", "Mixed Story", "en")
exported = scribus.exportEpubTextAndImages(epub_path, *publication)
check(exported["status"] == "exported", exported)
check("crop" in exported["detail"], "export did not disclose frame-fidelity limits")
with open(epub_path, "rb") as publication_file:
    local_header = publication_file.read(30)
    check(local_header[:4] == b"PK\x03\x04", "EPUB has no first ZIP local header")
    filename_size = struct.unpack_from("<H", local_header, 26)[0]
    check(publication_file.read(filename_size) == b"mimetype",
          "EPUB mimetype is not the first ZIP local entry")
with zipfile.ZipFile(epub_path) as archive:
    names = archive.namelist()
    check(archive.getinfo("mimetype").compress_type == zipfile.ZIP_STORED,
          "EPUB mimetype is compressed")
    check("OEBPS/images/image-1.png" in names, names)
    with open(image_path, "rb") as source:
        check(archive.read("OEBPS/images/image-1.png") == source.read(),
              "linked image bytes changed")
    package = ET.fromstring(archive.read("OEBPS/content.opf"))
    manifest = package.find("{http://www.idpf.org/2007/opf}manifest")
    check(any(item.get("href") == "images/image-1.png" and
              item.get("media-type") == "image/png" for item in manifest),
          "image is missing from OPF manifest")
    chapter = ET.fromstring(archive.read("OEBPS/chapter.xhtml"))
    main = chapter.find(".//{http://www.w3.org/1999/xhtml}main")
    blocks = list(main)
    check([block.tag.rsplit("}", 1)[-1] for block in blocks] ==
          ["h1", "p", "figure", "p"], "XHTML mixed reading order changed")
    check(blocks[1].text == "Before picture" and blocks[3].text == "After picture",
          "text blocks changed")
    image_element = blocks[2].find("{http://www.w3.org/1999/xhtml}img")
    check(blocks[2].get("style") == "width: 55%; max-width: 100%;",
          "explicit reflowable figure width was not exported")
    check(image_element.get("alt") == "Blue square & తెలుగు", "image alt text changed")
    check(image_element.get("style") == "width: 100%;",
          "image does not fill the explicit figure width")
    check(image_element.get("src") == "images/image-1.png", "image reference changed")
    caption_element = blocks[2].find("{http://www.w3.org/1999/xhtml}figcaption")
    check(caption_element is not None and caption_element.text ==
          "Figure 1: blue & తెలుగు", "visible figure caption changed")
    check(caption_element.get("class") == "scribus-align-center",
          "caption-only alignment was not exported")
with open(epub_path, "rb") as existing_file:
    original_epub = existing_file.read()
check(scribus.exportEpubTextAndImages(epub_path, *publication)["status"] == "output-exists",
      "mixed export overwrote an existing EPUB")
with open(epub_path, "rb") as existing_file:
    check(existing_file.read() == original_epub, "existing EPUB bytes changed")

scribus.setEpubImageReadingOrder(image, 0)
check(scribus.epubMixedReadingOrderPreflight()["status"] == "invalid-order",
      "missing image rank was accepted")
check(scribus.exportEpubTextAndImages(rejected_path, *publication)["status"] == "invalid-order",
      "mixed export accepted a missing image rank")
check(not os.path.exists(rejected_path), "failed export created an output file")
scribus.setEpubImageReadingOrder(image, 1)
check(scribus.epubMixedReadingOrderPreflight()["status"] == "invalid-order",
      "duplicate text/image rank was accepted")
scribus.setEpubImageReadingOrder(image, 2)
scribus.setEpubImageAltText(image, "")
check(scribus.epubMixedReadingOrderPreflight()["status"] == "unsupported-content",
      "missing image alt text was accepted")
check(scribus.exportEpubTextAndImages(rejected_path, *publication)["status"] == "unsupported-content",
      "mixed export accepted missing alt text")
check(not os.path.exists(rejected_path), "failed alt-text export created an output file")
scribus.setEpubImageAltText(image, "Blue square & తెలుగు")
check_mixed()
try:
    scribus.setEpubImageCaption(image, "Invalid\x00caption")
    raise AssertionError("invalid EPUB caption was accepted")
except ValueError:
    pass
scribus.setEpubImageCaption(image, "")
check(scribus.epubImageFramePreflight(image)["caption"] == "",
      "clearing an optional caption failed")
check(scribus.epubImageFramePreflight(image)["captionAlignment"] == "left",
      "clearing a caption left stale alignment metadata")
try:
    scribus.setEpubImageCaptionAlignment(image, "right")
    raise AssertionError("caption alignment was set without a caption")
except ValueError:
    pass
scribus.setEpubImageCaption(image, "Figure 1: blue & తెలుగు")
try:
    scribus.setEpubImageCaptionAlignment(image, "diagonal")
    raise AssertionError("unknown caption alignment was accepted")
except ValueError:
    pass
scribus.setEpubImageCaptionAlignment(image, "right")
check(scribus.epubImageFramePreflight(image)["captionAlignment"] == "right",
      "caption alignment could not be changed after clearing")
scribus.setEpubImageCaptionAlignment(image, "left")
bad_alignment_attributes = scribus.getObjectAttributes(image)
bad_alignment_attributes.append({
    "Name": "scribus:epub-image-caption-alignment", "Type": "Integer",
    "Value": "7", "Parameter": "", "Relationship": "",
    "RelationshipTo": "", "AutoAddTo": "",
})
scribus.setObjectAttributes(bad_alignment_attributes, image)
check("invalid EPUB caption alignment" in scribus.epubImageFramePreflight(image)["detail"],
      "malformed saved caption alignment was silently accepted")
scribus.setEpubImageCaptionAlignment(image, "left")
for invalid_width in (-1, 101):
    try:
        scribus.setEpubImageWidthPercent(image, invalid_width)
        raise AssertionError("invalid EPUB image width was accepted")
    except ValueError:
        pass
scribus.setEpubImageWidthPercent(image, 0)
check(scribus.epubImageFramePreflight(image)["widthPercent"] == 0,
      "clearing the figure-width override failed")
try:
    scribus.setEpubImageWidthPercent(image, True)
    raise AssertionError("boolean EPUB image width was accepted")
except TypeError:
    pass

scribus.createRect(300, 100, 30, 30, "Unsupported Shape")
check(scribus.epubMixedReadingOrderPreflight()["status"] == "unsupported-content",
      "an unsupported shape was silently omitted")
check(scribus.exportEpubTextAndImages(rejected_path, *publication)["status"] == "unsupported-content",
      "mixed export silently omitted a shape")
check(not os.path.exists(rejected_path), "failed shape export created an output file")
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create exact-crop document")
cropped_frame = scribus.createImage(72, 72, 2, 2, "Exact EPUB Crop")
scribus.loadImage(crop_image_path, cropped_frame)
scribus.setImageScale(4 / 3, 4 / 3, cropped_frame)
scribus.setImageOffset(-1, -1, cropped_frame)
scribus.setEpubImageAltText(cropped_frame, "Central two-by-two pixels")
scribus.setEpubUseImageFrameCrop(cropped_frame, True)
crop_preflight = scribus.epubImageFramePreflight(cropped_frame)
check(crop_preflight["frameCropApplied"] is True,
      "exact frame crop was not accepted: " + repr((crop_preflight,
        scribus.getImageScale(cropped_frame), scribus.getImageOffset(cropped_frame),
        scribus.getSize(cropped_frame))))
check(scribus.saveDocAs(crop_document_path), "could not save exact-crop document")
scribus.closeDoc()
scribus.openDoc(crop_document_path)
check(scribus.epubImageFramePreflight(cropped_frame)["frameCropApplied"] is True,
      "frame crop did not survive SLA save/reopen")
crop_report = scribus.epubMixedReadingOrderPreflight()
check(crop_report["status"] == "ready", crop_report)
check("image-offset" not in crop_report["imageFidelityWarnings"][0]["omitted"] and
      "image-scale" not in crop_report["imageFidelityWarnings"][0]["omitted"],
      "preflight still reports applied crop placement as omitted")
crop_export = scribus.exportEpubTextAndImages(crop_path,
    "urn:uuid:e0178616-eb90-4a80-a7e5-5e344566b6de", "Exact Crop", "en")
check(crop_export["status"] == "exported", crop_export)
with zipfile.ZipFile(crop_path) as archive:
    width, height, pixels = png_pixels(archive.read("OEBPS/images/image-1.png"))
    check((width, height) == (2, 2), "frame crop has the wrong size")
    check(pixels == [[(40, 40, 10), (80, 40, 10)],
                     [(40, 80, 10), (80, 80, 10)]], "frame crop selected wrong pixels")
scribus.setEpubUseImageFrameCrop(cropped_frame, False)
check(scribus.epubImageFramePreflight(cropped_frame)["frameCropApplied"] is False,
      "disabling frame crop did not restore original image bytes")
check(scribus.epubImageFramePreflight(cropped_frame)["byteSize"] == os.path.getsize(crop_image_path),
      "disabling frame crop did not restore the linked PNG bytes")
scribus.setEpubUseImageFrameCrop(cropped_frame, True)
scribus.setImageScale(1, 1, cropped_frame)
fractional_report = scribus.epubImageFramePreflight(cropped_frame)
check("integer-pixel viewport" in fractional_report["detail"],
      "fractional-pixel viewport was silently cropped: " + repr(fractional_report))
scribus.setImageScale(4 / 3, 4 / 3, cropped_frame)
scribus.setImageRotation(10, cropped_frame)
check(scribus.epubImageFramePreflight(cropped_frame)["status"] == "unsupported-content",
      "rotated PNG frame crop was silently approximated")
scribus.setObjectAttributes([{
    "Name": "scribus:epub-image-alt-text", "Type": "String",
    "Value": "Central two-by-two pixels", "Parameter": "", "Relationship": "",
    "RelationshipTo": "", "AutoAddTo": "",
}, {
    "Name": "scribus:epub-image-frame-crop", "Type": "Boolean",
    "Value": "false", "Parameter": "", "Relationship": "",
    "RelationshipTo": "", "AutoAddTo": "",
}], cropped_frame)
check("invalid EPUB crop flag" in scribus.epubImageFramePreflight(cropped_frame)["detail"],
      "malformed saved crop choice was silently accepted")
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create image-only document")
solo = scribus.createImage(72, 72, 100, 100, "Only EPUB Picture")
scribus.loadImage(image_path, solo)
scribus.setEpubImageAltText(solo, "One blue image")
image_only = scribus.epubMixedReadingOrderPreflight()
check(image_only["status"] == "ready", image_only)
check(image_only["blockKinds"] == ["image"], image_only)
check(image_only["imageIndices"] == [0], image_only)
check(image_only["lineSpacingWarnings"] == [], image_only)
check(scribus.epubImageFramePreflight(solo)["caption"] == "",
      "optional caption should be absent for a new image")
check(len(image_only["imageFidelityWarnings"]) == 1,
      "image-only preflight omitted the fidelity warning")
solo_export = scribus.exportEpubTextAndImages(image_only_path,
    "urn:uuid:b392b748-911a-4b94-8dd4-0db786724ae8", "One Image", "en")
check(solo_export["status"] == "exported", solo_export)
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create decorative-image document")
decoration = scribus.createImage(72, 72, 100, 100, "Decorative EPUB Picture")
scribus.loadImage(image_path, decoration)
check(scribus.epubMixedReadingOrderPreflight()["status"] == "unsupported-content",
      "an undescribed image was accepted before marking it decorative")
scribus.setEpubImageDecorative(decoration, True)
check(scribus.epubImageFramePreflight(decoration)["decorative"] is True,
      "decorative flag is absent from image preflight")
for setter in (scribus.setEpubImageAltText, scribus.setEpubImageCaption):
    try:
        setter(decoration, "This must not be added")
        raise AssertionError("decorative image accepted a description or caption")
    except ValueError:
        pass
check(scribus.saveDocAs(decorative_document_path),
      "could not save decorative-image document")
scribus.closeDoc()
scribus.openDoc(decorative_document_path)
check(scribus.epubImageFramePreflight(decoration)["decorative"] is True,
      "decorative flag did not survive SLA save/reopen")
decorative_report = scribus.epubMixedReadingOrderPreflight()
check(decorative_report["status"] == "ready" and
      decorative_report["blockKinds"] == ["image"] and
      decorative_report["texts"] == [""], decorative_report)
decorative_export = scribus.exportEpubTextAndImages(decorative_path,
    "urn:uuid:67a58a25-f6a4-4f59-a67a-e57d77cff910", "Decorative Image", "en")
check(decorative_export["status"] == "exported", decorative_export)
with zipfile.ZipFile(decorative_path) as archive:
    chapter = ET.fromstring(archive.read("OEBPS/chapter.xhtml"))
    image_element = chapter.find(".//{http://www.w3.org/1999/xhtml}img")
    check(image_element is not None and image_element.get("alt") == "" and
          image_element.get("role") == "presentation", "decorative markup changed")
    check(chapter.find(".//{http://www.w3.org/1999/xhtml}figcaption") is None,
          "decorative image acquired a visible caption")
scribus.setEpubImageDecorative(decoration, False)
check(scribus.epubMixedReadingOrderPreflight()["status"] == "unsupported-content",
      "clearing decorative flag did not restore the missing-alt error")
scribus.setEpubImageAltText(decoration, "A meaningful image")
try:
    scribus.setEpubImageDecorative(decoration, True)
    raise AssertionError("described image was marked decorative")
except ValueError:
    pass
scribus.setObjectAttributes([{
    "Name": "scribus:epub-image-decorative", "Type": "Boolean",
    "Value": "false", "Parameter": "", "Relationship": "",
    "RelationshipTo": "", "AutoAddTo": "",
}], decoration)
check("invalid EPUB decorative flag" in scribus.epubImageFramePreflight(decoration)["detail"],
      "malformed saved decorative metadata was silently accepted")
scribus.setObjectAttributes([{
    "Name": "scribus:epub-image-alt-text", "Type": "String",
    "Value": "A meaningful image", "Parameter": "", "Relationship": "",
    "RelationshipTo": "", "AutoAddTo": "",
}, {
    "Name": "scribus:epub-image-width-percent", "Type": "Integer",
    "Value": "101", "Parameter": "", "Relationship": "",
    "RelationshipTo": "", "AutoAddTo": "",
}], decoration)
check("invalid EPUB width percentage" in scribus.epubImageFramePreflight(decoration)["detail"],
      "malformed saved figure width was silently accepted")
scribus.setEpubImageWidthPercent(decoration, 50)
check(scribus.epubImageFramePreflight(decoration)["status"] == "ready",
      "figure-width control could not repair malformed saved metadata")
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create multi-error preflight document")
bad_first = scribus.createImage(50, 50, 100, 100, "First invalid EPUB image")
bad_second = scribus.createImage(50, 200, 100, 100, "Second invalid EPUB image")
for bad_image in (bad_first, bad_second):
    scribus.loadImage(image_path, bad_image)
    scribus.setEpubImageReadingOrder(bad_image, 1)
problems = scribus.epubMixedReadingOrderPreflight()
check(problems["status"] != "ready", problems)
errors = [issue for issue in problems["issues"] if issue["severity"] == "error"]
check(len([issue for issue in errors if issue["code"] == "image-invalid"]) == 2, problems)
check(any(issue["code"] == "reading-order" for issue in errors), problems)
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create unsupported-story preflight document")
scribus.createParagraphStyle("EPUB Unsupported Bullet", bullet="→")
bad_story_first = scribus.createText(50, 50, 240, 50, "First unsupported EPUB story")
bad_story_second = scribus.createText(50, 150, 240, 50, "Second unsupported EPUB story")
for rank, story in enumerate((bad_story_first, bad_story_second), 1):
    scribus.setText("Unsupported bullet\nAnother unsupported bullet" if rank == 1
                    else "Unsupported bullet", story)
    scribus.setParagraphStyle("EPUB Unsupported Bullet", story)
    text_rank(story, rank)
story_report = scribus.epubMixedReadingOrderPreflight()
check(story_report["status"] == "unsupported-content", story_report)
story_errors = [issue for issue in story_report["issues"]
                if issue["code"] == "story-unsupported"]
check(len(story_errors) == 3, story_report)
check(bad_story_first in story_errors[0]["detail"] and
      "paragraph 1" in story_errors[0]["detail"] and
      bad_story_first in story_errors[1]["detail"] and
      "paragraph 2" in story_errors[1]["detail"] and
      bad_story_second in story_errors[2]["detail"], story_report)
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create inline-token preflight document")
inline_story = scribus.createText(50, 50, 300, 150, "EPUB Unsupported Inline Tokens")
scribus.setText("One\u2028Two\u2028Three\nFour\u2028Five", inline_story)
inline_report = scribus.epubMixedReadingOrderPreflight()
check(inline_report["status"] == "unsupported-content", inline_report)
inline_errors = [issue for issue in inline_report["issues"]
                 if issue["code"] == "story-unsupported"]
check(len(inline_errors) == 3, inline_report)
check(all("paragraph 1" in issue["detail"] for issue in inline_errors[:2]) and
      "paragraph 2" in inline_errors[2]["detail"], inline_report)
check(scribus.exportEpubTextAndImages(rejected_path, *publication)["status"] ==
      "unsupported-content", "inline control characters were exported")
check(not os.path.exists(rejected_path), "failed inline export created an output file")
scribus.closeDoc()

check(scribus.newDocument(scribus.PAPER_A4, (36, 36, 36, 36),
    scribus.PORTRAIT, 1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1),
    "could not create mixed object-error preflight document")
unsupported_shape_first = scribus.createRect(40, 40, 30, 30, "First unsupported EPUB shape")
unsupported_shape_second = scribus.createRect(90, 40, 30, 30, "Second unsupported EPUB shape")
unsupported_text = scribus.createText(50, 150, 250, 50, "Unsupported EPUB story beside shapes")
scribus.setText("Custom bullet", unsupported_text)
scribus.createParagraphStyle("EPUB Shape Fixture Bullet", bullet="→")
scribus.setParagraphStyle("EPUB Shape Fixture Bullet", unsupported_text)
hierarchy_report = scribus.epubMixedReadingOrderPreflight()
check(hierarchy_report["status"] == "unsupported-content", hierarchy_report)
object_errors = [issue for issue in hierarchy_report["issues"]
                 if issue["code"] == "object-unsupported"]
check(len(object_errors) == 2 and
      unsupported_shape_first in object_errors[0]["detail"] and
      unsupported_shape_second in object_errors[1]["detail"], hierarchy_report)
check(any(issue["code"] == "story-unsupported" and
          unsupported_text in issue["detail"] for issue in hierarchy_report["issues"]),
      hierarchy_report)
check(scribus.exportEpubTextAndImages(rejected_path, *publication)["status"] ==
      "unsupported-content", "unsupported page objects were exported")
check(not os.path.exists(rejected_path), "failed object export created an output file")
scribus.closeDoc()

print("EPUB_MIXED_READING_ORDER_PASSED", flush=True)
