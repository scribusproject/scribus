#!/usr/bin/env python3

"""Check that EPUB story-order attributes survive an SLA save/reopen cycle.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program.
"""

import os
import tempfile
import zipfile
from pathlib import Path
from xml.etree import ElementTree

import scribus


output = Path(os.environ["SCRIBUS_TEST_OUTPUT_DIR"])
output.mkdir(parents=True, exist_ok=True)
document = output / "epub-order.sla"
attribute_name = "scribus:epub-reading-order"


def attribute(name, value):
    return {
        "Name": name,
        "Type": "Integer" if name == attribute_name else "String",
        "Value": value,
        "Parameter": "",
        "Relationship": "",
        "RelationshipTo": "",
        "AutoAddTo": "",
    }


assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create EPUB order test document"
first = scribus.createText(40, 40, 240, 90, "FirstStory")
second = scribus.createText(40, 150, 240, 90, "SecondStory")
scribus.setText("First story", first)
scribus.setText("Second story", second)
scribus.setObjectAttributes([attribute("custom-note", "keep"), attribute(attribute_name, "2")], first)
scribus.setObjectAttributes([attribute(attribute_name, "1")], second)
scribus.setItemName("RenamedFirstStory", first)
first = "RenamedFirstStory"
assert scribus.saveDocAs(str(document)), "Could not save EPUB order test document"
scribus.closeDoc()

saved = ElementTree.parse(document)
item_attributes = [
    (node.get("Name"), node.get("Value"))
    for node in saved.iter("ItemAttribute")
]
assert (attribute_name, "1") in item_attributes and (attribute_name, "2") in item_attributes, (
    "EPUB ranks were not written to SLA"
)
assert ("custom-note", "keep") in item_attributes, "Unrelated item attribute was lost"

assert scribus.openDoc(str(document)), "Could not reopen EPUB order test document"
preflight = scribus.epubReadingOrderPreflight()
assert preflight["status"] == "ready", preflight["detail"]
assert preflight["paragraphs"] == ["Second story", "First story"], preflight
with tempfile.TemporaryDirectory(prefix="epub-export-", dir=output) as export_dir:
    publication = Path(export_dir) / "text-only.epub"
    result = scribus.exportEpubTextOnly(
        str(publication), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Ordered Stories", "en", {}, {}, "Test Author",
    )
    assert result["status"] == "exported", result
    local_header = publication.read_bytes()[:58]
    assert local_header[:4] == b"PK\x03\x04"
    assert local_header[30:38] == b"mimetype", local_header
    with zipfile.ZipFile(publication) as archive:
        assert archive.getinfo("mimetype").compress_type == zipfile.ZIP_STORED
        chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
        paragraphs = chapter.findall(".//{http://www.w3.org/1999/xhtml}p")
        assert [node.text for node in paragraphs] == ["Second story", "First story"]
        package = ElementTree.fromstring(archive.read("OEBPS/content.opf"))
        creator = package.find(".//{http://purl.org/dc/elements/1.1/}creator")
        assert creator is not None and creator.text == "Test Author"
    validation_copy = os.environ.get("SCRIBUS_EPUB_VALIDATE_OUTPUT")
    if validation_copy:
        validation_path = Path(validation_copy)
        assert validation_path.is_absolute() and not validation_path.exists()
        validation_path.write_bytes(publication.read_bytes())
    original_bytes = publication.read_bytes()
    result = scribus.exportEpubTextOnly(
        str(publication), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Ordered Stories", "en",
    )
    assert result["status"] == "output-exists", result
    assert publication.read_bytes() == original_bytes, "Existing EPUB was overwritten"
    invalid_path = Path(export_dir) / "missing-title.epub"
    result = scribus.exportEpubTextOnly(
        str(invalid_path), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d", "", "en",
    )
    assert result["status"] == "invalid-input" and not invalid_path.exists(), result
    result = scribus.exportEpubTextOnly(
        "relative.epub", "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Ordered Stories", "en",
    )
    assert result["status"] == "invalid-input", result
for frame, expected in ((first, "2"), (second, "1")):
    values = [item["Value"] for item in scribus.getObjectAttributes(frame)
              if item["Name"] == attribute_name]
    assert values == [expected], "EPUB rank did not survive reopen: %s" % frame
assert any(item["Name"] == "custom-note" for item in scribus.getObjectAttributes(first)), (
    "Unrelated item attribute did not survive reopen"
)
scribus.closeDoc()

# Grouped child attributes must also survive the current SLA loader/saver.
assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create grouped EPUB order document"
grouped_frames = [
    scribus.createText(40, 40 + offset * 90, 240, 60, "GroupedStory%d" % offset)
    for offset in range(3)
]
for index, frame in enumerate(grouped_frames):
    scribus.setText("Grouped story %d" % index, frame)
    scribus.setObjectAttributes([attribute(attribute_name, str(3 - index))], frame)
inner_group = scribus.groupObjects(grouped_frames[:2])
assert inner_group, "Could not create inner text group"
outer_group = scribus.groupObjects([inner_group, grouped_frames[2]])
assert outer_group, "Could not create nested text group"
grouped_document = output / "epub-order-grouped.sla"
assert scribus.saveDocAs(str(grouped_document)), "Could not save grouped EPUB order document"
scribus.closeDoc()


def grouped_ranks(path):
    tree = ElementTree.parse(path)
    return sorted(node.get("Value") for node in tree.iter("ItemAttribute")
                  if node.get("Name") == attribute_name)


assert grouped_ranks(grouped_document) == ["1", "2", "3"], "Grouped child ranks were not saved"
assert scribus.openDoc(str(grouped_document)), "Could not reopen grouped EPUB order document"
preflight = scribus.epubReadingOrderPreflight()
assert preflight["status"] == "ready", preflight["detail"]
assert preflight["paragraphs"] == ["Grouped story 2", "Grouped story 1", "Grouped story 0"], preflight
grouped_roundtrip = output / "epub-order-grouped-roundtrip.sla"
assert scribus.saveDocAs(str(grouped_roundtrip)), "Could not re-save grouped EPUB order document"
scribus.closeDoc()
assert grouped_ranks(grouped_roundtrip) == ["1", "2", "3"], (
    "Grouped child ranks were lost after reopen"
)

# A text-only reading-order result must not imply that shapes can be exported.
assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create unsupported-content document"
visual = scribus.createRect(40, 40, 120, 60)
assert visual, "Could not create a shape for EPUB preflight"
preflight = scribus.epubReadingOrderPreflight()
assert preflight["status"] == "unsupported-content", preflight
assert preflight["paragraphs"] == [], preflight
with tempfile.TemporaryDirectory(prefix="epub-reject-", dir=output) as export_dir:
    unsupported_output = Path(export_dir) / "shape.epub"
    result = scribus.exportEpubTextOnly(
        str(unsupported_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Shape", "en",
    )
    assert result["status"] == "unsupported-content", result
    assert not unsupported_output.exists(), "Unsupported content wrote an EPUB"
scribus.closeDoc()

# A named paragraph style needs an explicit semantic assignment. Verify it
# after reopening, so the adapter reads a real saved Scribus style reference.
assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create semantic-style document"
scribus.createParagraphStyle("EPUB Chapter")
scribus.createParagraphStyle("EPUB Unused Paragraph")
heading = scribus.createText(40, 40, 240, 60, "StyledHeading")
scribus.setText("A styled chapter", heading)
scribus.setParagraphStyle("EPUB Chapter", heading)
styled_document = output / "epub-styled-heading.sla"
assert scribus.saveDocAs(str(styled_document)), "Could not save semantic-style document"
scribus.closeDoc()
assert scribus.openDoc(str(styled_document)), "Could not reopen semantic-style document"
assert scribus.epubMixedReadingOrderPreflight()["usedParagraphStyles"] == ["EPUB Chapter"], (
    "EPUB dialog style discovery missed the used paragraph style"
)
mixed_preflight = scribus.epubMixedReadingOrderPreflight({"EPUB Chapter": 1})
assert mixed_preflight["status"] == "ready" and mixed_preflight["blockKinds"] == ["heading"], (
    mixed_preflight
)
assert scribus.epubReadingOrderPreflight()["status"] == "unsupported-content", (
    "A named style was silently flattened"
)
preflight = scribus.epubReadingOrderPreflight({"EPUB Chapter": 1})
assert preflight["status"] == "ready", preflight
assert preflight["paragraphs"] == ["A styled chapter"], preflight
assert preflight["headingLevels"] == [1], preflight
with tempfile.TemporaryDirectory(prefix="epub-heading-", dir=output) as export_dir:
    heading_output = Path(export_dir) / "heading.epub"
    result = scribus.exportEpubTextOnly(
        str(heading_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Headings", "en", {"EPUB Chapter": 1},
    )
    assert result["status"] == "exported", result
    with zipfile.ZipFile(heading_output) as archive:
        chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
        heading = chapter.find(".//{http://www.w3.org/1999/xhtml}h1")
        assert heading is not None and heading.text == "A styled chapter"
        nav = archive.read("OEBPS/nav.xhtml")
        assert b"chapter.xhtml#h1" in nav
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create paragraph-metrics document"
scribus.createParagraphStyle(
    "EPUB Spaced", linespacingmode=0, linespacing=22.0,
    leftmargin=18.0, rightmargin=7.0,
    firstindent=-4.0, gapbefore=6.0, gapafter=3.0,
)
spaced = scribus.createText(40, 40, 240, 90, "SpacedStory")
scribus.setText("Indented paragraph", spaced)
scribus.setParagraphStyle("EPUB Spaced", spaced)
spacing_document = output / "epub-paragraph-metrics.sla"
assert scribus.saveDocAs(str(spacing_document)), "Could not save paragraph-metrics document"
scribus.closeDoc()
assert scribus.openDoc(str(spacing_document)), "Could not reopen paragraph-metrics document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Spaced": 0})
assert preflight["status"] == "ready", preflight
assert len(preflight["lineSpacingWarnings"]) == 1, preflight
with tempfile.TemporaryDirectory(prefix="epub-spacing-", dir=output) as export_dir:
    spacing_output = Path(export_dir) / "spacing.epub"
    result = scribus.exportEpubTextOnly(
        str(spacing_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Paragraph Metrics", "en", {"EPUB Spaced": 0},
    )
    assert result["status"] == "exported", result
    assert "line spacing" in result["detail"], result
    with zipfile.ZipFile(spacing_output) as archive:
        chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
        paragraph = chapter.find(".//{http://www.w3.org/1999/xhtml}p")
        assert paragraph is not None and paragraph.text == "Indented paragraph"
        style = paragraph.get("style", "")
        for property_value in (
            "margin-left:18.000000pt", "margin-right:7.000000pt",
            "text-indent:-4.000000pt", "margin-top:6.000000pt",
            "margin-bottom:3.000000pt",
        ):
            assert property_value in style, style
        assert "line-height" not in style, style
    validation_copy = os.environ.get("SCRIBUS_EPUB_VALIDATE_SPACING_OUTPUT")
    if validation_copy:
        validation_path = Path(validation_copy)
        assert validation_path.is_absolute() and not validation_path.exists()
        validation_path.write_bytes(spacing_output.read_bytes())
scribus.closeDoc()
spacing_tree = ElementTree.parse(spacing_document)
spaced_style = next(node for node in spacing_tree.iter("ParagraphStyle")
                    if node.get("Name") == "EPUB Spaced")
spaced_style.set("LineSpacingMode", "2")
baseline_document = output / "epub-baseline-grid.sla"
spacing_tree.write(baseline_document, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(baseline_document)), "Could not reopen baseline-grid document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Spaced": 0})
assert preflight["status"] == "unsupported-content", preflight
assert "Baseline-grid" in preflight["detail"], preflight
with tempfile.TemporaryDirectory(prefix="epub-grid-reject-", dir=output) as export_dir:
    grid_output = Path(export_dir) / "grid.epub"
    result = scribus.exportEpubTextOnly(
        str(grid_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Baseline Grid", "en", {"EPUB Spaced": 0},
    )
    assert result["status"] == "unsupported-content" and not grid_output.exists(), result
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create EPUB navigation document"
scribus.createParagraphStyle("EPUB Nav Chapter")
scribus.createParagraphStyle("EPUB Nav Section")
nav_chapter = scribus.createText(40, 40, 240, 60, "NavChapter")
nav_section = scribus.createText(40, 120, 240, 60, "NavSection")
nav_next_chapter = scribus.createText(40, 200, 240, 60, "NavNextChapter")
scribus.setText("Chapter", nav_chapter)
scribus.setText("Section", nav_section)
scribus.setText("Next chapter", nav_next_chapter)
scribus.setParagraphStyle("EPUB Nav Chapter", nav_chapter)
scribus.setParagraphStyle("EPUB Nav Section", nav_section)
scribus.setParagraphStyle("EPUB Nav Chapter", nav_next_chapter)
scribus.setObjectAttributes([attribute(attribute_name, "1")], nav_chapter)
scribus.setObjectAttributes([attribute(attribute_name, "2")], nav_section)
scribus.setObjectAttributes([attribute(attribute_name, "3")], nav_next_chapter)
nav_document = output / "epub-navigation.sla"
assert scribus.saveDocAs(str(nav_document)), "Could not save EPUB navigation document"
scribus.closeDoc()
nav_tree = ElementTree.parse(nav_document)
nav_section_style = next(node for node in nav_tree.iter("ParagraphStyle")
                         if node.get("Name") == "EPUB Nav Section")
nav_section_style.set("ALIGN", "1")  # Centered, distinct from chapter headings.
nav_tree.write(nav_document, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(nav_document)), "Could not reopen EPUB navigation document"
preflight = scribus.epubReadingOrderPreflight(
    {"EPUB Nav Chapter": 1, "EPUB Nav Section": 2})
assert preflight["status"] == "ready", preflight
assert preflight["alignments"] == ["left", "center", "left"], preflight
with tempfile.TemporaryDirectory(prefix="epub-navigation-", dir=output) as export_dir:
    nav_output = Path(export_dir) / "navigation.epub"
    result = scribus.exportEpubTextOnly(
        str(nav_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Navigation", "en", {"EPUB Nav Chapter": 1, "EPUB Nav Section": 2},
    )
    assert result["status"] == "exported", result
    with zipfile.ZipFile(nav_output) as archive:
        nav = ElementTree.fromstring(archive.read("OEBPS/nav.xhtml"))
        xhtml = "{http://www.w3.org/1999/xhtml}"
        top_list = nav.find(".//" + xhtml + "nav/" + xhtml + "ol")
        assert top_list is not None
        top_items = top_list.findall(xhtml + "li")
        assert len(top_items) == 2
        top_item = top_items[0]
        assert top_item.find(xhtml + "a").get("href") == "chapter.xhtml#h1"
        nested_list = top_item.find(xhtml + "ol")
        assert nested_list is not None
        nested_link = nested_list.find(xhtml + "li/" + xhtml + "a")
        assert nested_link is not None and nested_link.get("href") == "chapter.xhtml#h2"
        assert top_items[1].find(xhtml + "a").get("href") == "chapter-2.xhtml#h3"
        assert "OEBPS/chapter-2.xhtml" in archive.namelist()
        first_chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
        second_chapter = ElementTree.fromstring(archive.read("OEBPS/chapter-2.xhtml"))
        assert first_chapter.find(".//" + xhtml + "title").text == "Chapter"
        assert second_chapter.find(".//" + xhtml + "title").text == "Next chapter"
        assert nav.find(".//" + xhtml + "title").text == "Navigation"
        assert [node.text for node in first_chapter.findall(".//" + xhtml + "h1")] == ["Chapter"]
        assert [node.text for node in second_chapter.findall(".//" + xhtml + "h1")] == ["Next chapter"]
        section_heading = first_chapter.find(".//" + xhtml + "h2")
        assert section_heading is not None and section_heading.get("class") == "scribus-align-center"
        assert b"text-align: center" in archive.read("OEBPS/styles.css")
        package = ElementTree.fromstring(archive.read("OEBPS/content.opf"))
        styles = package.find(".//{http://www.idpf.org/2007/opf}item[@id='styles']")
        assert styles is not None and styles.get("media-type") == "text/css"
        spine = package.find("{http://www.idpf.org/2007/opf}spine")
        assert [node.get("idref") for node in spine] == ["chapter", "chapter-2"]
    validation_copy = os.environ.get("SCRIBUS_EPUB_VALIDATE_OUTLINE_OUTPUT")
    if validation_copy:
        validation_path = Path(validation_copy)
        assert validation_path.is_absolute() and not validation_path.exists()
        validation_path.write_bytes(nav_output.read_bytes())
scribus.closeDoc()
nav_section_style.set("ALIGN", "4")  # Extended cannot be reproduced exactly by this slice.
extended_document = output / "epub-extended-alignment.sla"
nav_tree.write(extended_document, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(extended_document)), "Could not reopen extended-alignment document"
preflight = scribus.epubReadingOrderPreflight(
    {"EPUB Nav Chapter": 1, "EPUB Nav Section": 2})
assert preflight["status"] == "unsupported-content", preflight
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create EPUB direction document"
scribus.createParagraphStyle("EPUB RTL Heading")
rtl_frame = scribus.createText(40, 40, 240, 60, "RTLHeading")
scribus.setText("שלום עולם", rtl_frame)
scribus.setParagraphStyle("EPUB RTL Heading", rtl_frame)
rtl_document = output / "epub-rtl-heading.sla"
assert scribus.saveDocAs(str(rtl_document)), "Could not save EPUB direction document"
scribus.closeDoc()
rtl_tree = ElementTree.parse(rtl_document)
rtl_style = next(node for node in rtl_tree.iter("ParagraphStyle")
                 if node.get("Name") == "EPUB RTL Heading")
rtl_style.set("DIRECTION", "1")
rtl_tree.write(rtl_document, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(rtl_document)), "Could not reopen EPUB direction document"
preflight = scribus.epubReadingOrderPreflight({"EPUB RTL Heading": 1})
assert preflight["status"] == "ready", preflight
assert preflight["directions"] == ["rtl"], preflight
with tempfile.TemporaryDirectory(prefix="epub-rtl-", dir=output) as export_dir:
    rtl_output = Path(export_dir) / "rtl.epub"
    result = scribus.exportEpubTextOnly(
        str(rtl_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "RTL", "he", {"EPUB RTL Heading": 1},
    )
    assert result["status"] == "exported", result
    with zipfile.ZipFile(rtl_output) as archive:
        xhtml = "{http://www.w3.org/1999/xhtml}"
        chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
        heading = chapter.find(".//" + xhtml + "h1")
        assert heading is not None and heading.get("dir") == "rtl"
        nav = ElementTree.fromstring(archive.read("OEBPS/nav.xhtml"))
        nav_item = nav.find(".//" + xhtml + "li")
        assert nav_item is not None and nav_item.get("dir") == "rtl"
    validation_copy = os.environ.get("SCRIBUS_EPUB_VALIDATE_RTL_OUTPUT")
    if validation_copy:
        validation_path = Path(validation_copy)
        assert validation_path.is_absolute() and not validation_path.exists()
        validation_path.write_bytes(rtl_output.read_bytes())
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create local-numbering document"
scribus.createParagraphStyle("EPUB Local Number")
numbered = scribus.createText(40, 40, 240, 80, "NumberedStory")
scribus.setText("First item\nSecond item", numbered)
scribus.setParagraphStyle("EPUB Local Number", numbered)
numbered_document = output / "epub-local-numbering.sla"
assert scribus.saveDocAs(str(numbered_document)), "Could not save local-numbering document"
scribus.closeDoc()
numbered_tree = ElementTree.parse(numbered_document)
numbered_style = next(node for node in numbered_tree.iter("ParagraphStyle")
                      if node.get("Name") == "EPUB Local Number")
numbered_style.set("Numeration", "1")
numbered_style.set("NumerationName", "<local block>")
numbered_style.set("NumerationFormat", "0")
numbered_style.set("NumerationLevel", "0")
numbered_style.set("NumerationStart", "3")
numbered_style.set("NumerationPrefix", "")
numbered_style.set("NumerationSuffix", ".")
numbered_style.set("NumerationRestart", "0")
numbered_tree.write(numbered_document, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(numbered_document)), "Could not reopen local-numbering document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Local Number": 0})
assert preflight["status"] == "ready", preflight
assert preflight["paragraphs"] == ["First item", "Second item"], preflight
assert preflight["blockKinds"] == ["ordered-item", "ordered-item"], preflight
assert preflight["listStarts"] == [True, False], preflight
assert preflight["listStartNumbers"] == [3, 0], preflight
assert preflight["listTypes"] == ["1", ""], preflight
scribus.closeDoc()
for format_value, expected_type, label in (("2", "i", "lower"), ("3", "I", "upper")):
    numbered_style.set("NumerationFormat", format_value)
    roman_numbering = output / ("epub-%s-roman-numbering.sla" % label)
    numbered_tree.write(roman_numbering, encoding="utf-8", xml_declaration=True)
    assert scribus.openDoc(str(roman_numbering)), "Could not reopen Roman-numbering document"
    preflight = scribus.epubReadingOrderPreflight({"EPUB Local Number": 0})
    assert preflight["status"] == "ready", preflight
    assert preflight["paragraphs"] == ["First item", "Second item"], preflight
    assert preflight["listStartNumbers"] == [3, 0], preflight
    assert preflight["listTypes"] == [expected_type, ""], preflight
    with tempfile.TemporaryDirectory(prefix="epub-roman-", dir=output) as export_dir:
        roman_output = Path(export_dir) / "roman.epub"
        result = scribus.exportEpubTextOnly(
            str(roman_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
            "Roman", "en", {"EPUB Local Number": 0},
        )
        assert result["status"] == "exported", result
        with zipfile.ZipFile(roman_output) as archive:
            chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
            ordered = chapter.find(".//{http://www.w3.org/1999/xhtml}ol")
            assert ordered is not None and ordered.get("type") == expected_type
            assert ordered.get("start") == "3"
    scribus.closeDoc()
numbered_style.set("NumerationStart", "25")
for format_value, expected_type, label in (("4", "a", "lower"), ("5", "A", "upper")):
    numbered_style.set("NumerationFormat", format_value)
    alphabetic_numbering = output / ("epub-%s-alphabetic-numbering.sla" % label)
    numbered_tree.write(alphabetic_numbering, encoding="utf-8", xml_declaration=True)
    assert scribus.openDoc(str(alphabetic_numbering)), "Could not reopen alphabetic-numbering document"
    preflight = scribus.epubReadingOrderPreflight({"EPUB Local Number": 0})
    assert preflight["status"] == "ready", preflight
    assert preflight["listStartNumbers"] == [25, 0], preflight
    assert preflight["listTypes"] == [expected_type, ""], preflight
    with tempfile.TemporaryDirectory(prefix="epub-alphabetic-", dir=output) as export_dir:
        alphabetic_output = Path(export_dir) / "alphabetic.epub"
        result = scribus.exportEpubTextOnly(
            str(alphabetic_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
            "Alphabetic", "en", {"EPUB Local Number": 0},
        )
        assert result["status"] == "exported", result
        with zipfile.ZipFile(alphabetic_output) as archive:
            chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
            ordered = chapter.find(".//{http://www.w3.org/1999/xhtml}ol")
            assert ordered is not None and ordered.get("type") == expected_type
            assert ordered.get("start") == "25"
    scribus.closeDoc()
numbered_style.set("NumerationStart", "26")
alphabetic_overflow = output / "epub-alphabetic-overflow.sla"
numbered_tree.write(alphabetic_overflow, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(alphabetic_overflow)), "Could not reopen alphabetic-overflow document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Local Number": 0})
assert preflight["status"] == "unsupported-content", preflight
scribus.closeDoc()
numbered_style.set("NumerationStart", "3999")
numbered_style.set("NumerationFormat", "3")
roman_overflow = output / "epub-roman-overflow.sla"
numbered_tree.write(roman_overflow, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(roman_overflow)), "Could not reopen Roman-overflow document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Local Number": 0})
assert preflight["status"] == "unsupported-content", preflight
scribus.closeDoc()
numbered_style.set("NumerationFormat", "0")
numbered_style.set("NumerationStart", "3")
numbered_style.set("NumerationLevel", "1")
nested_numbering = output / "epub-nested-numbering.sla"
numbered_tree.write(nested_numbering, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(nested_numbering)), "Could not reopen nested-numbering document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Local Number": 0})
assert preflight["status"] == "unsupported-content", preflight
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create list-style document"
scribus.createParagraphStyle("EPUB Bullet", bullet="•")
bullet = scribus.createText(40, 40, 240, 60, "BulletStory")
scribus.setText("An item\nSecond item", bullet)
scribus.setParagraphStyle("EPUB Bullet", bullet)
bullet_document = output / "epub-bullet.sla"
assert scribus.saveDocAs(str(bullet_document)), "Could not save bullet document"
scribus.closeDoc()
assert scribus.openDoc(str(bullet_document)), "Could not reopen bullet document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Bullet": 0})
assert preflight["status"] == "ready", preflight
assert preflight["paragraphs"] == ["An item", "Second item"], preflight
assert preflight["blockKinds"] == ["bullet-item", "bullet-item"], preflight
assert preflight["listStarts"] == [True, False], preflight
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create nested-bullet document"
scribus.createParagraphStyle("EPUB Outer Bullet", bullet="•")
scribus.createParagraphStyle("EPUB Inner Bullet", bullet="•")
nested_bullets = scribus.createText(40, 40, 240, 90, "NestedBulletStory")
scribus.setText("Parent\nChild\nNext parent", nested_bullets)
scribus.setParagraphStyle("EPUB Outer Bullet", nested_bullets)
scribus.selectText(7, 5, nested_bullets)
scribus.setParagraphStyle("EPUB Inner Bullet", nested_bullets)
nested_bullet_document = output / "epub-nested-bullet.sla"
assert scribus.saveDocAs(str(nested_bullet_document)), "Could not save nested-bullet document"
scribus.closeDoc()
nested_bullet_tree = ElementTree.parse(nested_bullet_document)
inner_style = next(node for node in nested_bullet_tree.iter("ParagraphStyle")
                   if node.get("Name") == "EPUB Inner Bullet")
inner_style.set("NumerationLevel", "1")
nested_bullet_tree.write(nested_bullet_document, encoding="utf-8", xml_declaration=True)
assert scribus.openDoc(str(nested_bullet_document)), "Could not reopen nested-bullet document"
preflight = scribus.epubReadingOrderPreflight({"EPUB Outer Bullet": 0, "EPUB Inner Bullet": 0})
assert preflight["status"] == "ready", preflight
assert preflight["paragraphs"] == ["Parent", "Child", "Next parent"], preflight
assert preflight["listLevels"] == [0, 1, 0], preflight
with tempfile.TemporaryDirectory(prefix="epub-nested-bullet-", dir=output) as export_dir:
    nested_output = Path(export_dir) / "nested-bullet.epub"
    result = scribus.exportEpubTextOnly(
        str(nested_output), "urn:uuid:98c50910-9f46-46c1-80ab-5bd8f9047c0d",
        "Nested Bullets", "en", {"EPUB Outer Bullet": 0, "EPUB Inner Bullet": 0},
    )
    assert result["status"] == "exported", result
    with zipfile.ZipFile(nested_output) as archive:
        xhtml = "{http://www.w3.org/1999/xhtml}"
        chapter = ElementTree.fromstring(archive.read("OEBPS/chapter.xhtml"))
        outer = chapter.find(".//" + xhtml + "ul")
        assert outer is not None and len(outer) == 2
        assert outer[0].text == "Parent" and outer[1].text == "Next parent"
        inner = outer[0].find(xhtml + "ul")
        assert inner is not None and [node.text for node in inner] == ["Child"]
    validation_copy = os.environ.get("SCRIBUS_EPUB_VALIDATE_NESTED_OUTPUT")
    if validation_copy:
        validation_path = Path(validation_copy)
        assert validation_path.is_absolute() and not validation_path.exists()
        validation_path.write_bytes(nested_output.read_bytes())
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create custom-bullet document"
scribus.createParagraphStyle("EPUB Custom Bullet", bullet="→")
custom_bullet = scribus.createText(40, 40, 240, 60, "CustomBulletStory")
scribus.setText("An item", custom_bullet)
scribus.setParagraphStyle("EPUB Custom Bullet", custom_bullet)
preflight = scribus.epubReadingOrderPreflight({"EPUB Custom Bullet": 0})
assert preflight["status"] == "unsupported-content", preflight
assert "custom" in preflight["detail"], preflight
scribus.closeDoc()

assert scribus.newDocument(
    scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
    scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
), "Could not create character-style document"
scribus.createCharStyle("EPUB Emphasis")
scribus.createCharStyle("EPUB Unused Character")
character = scribus.createText(40, 40, 240, 60, "CharacterStory")
scribus.setText("Emphasized text", character)
scribus.setCharacterStyle("EPUB Emphasis", character)
character_document = output / "epub-styled-character.sla"
assert scribus.saveDocAs(str(character_document)), "Could not save character-style document"
scribus.closeDoc()
assert scribus.openDoc(str(character_document)), "Could not reopen character-style document"
assert scribus.epubMixedReadingOrderPreflight()["usedCharacterStyles"] == ["EPUB Emphasis"], (
    "EPUB dialog style discovery missed the used character style"
)
mixed_preflight = scribus.epubMixedReadingOrderPreflight({}, {"EPUB Emphasis": "emphasis"})
assert mixed_preflight["status"] == "ready" and mixed_preflight["texts"] == ["Emphasized text"], (
    mixed_preflight
)
preflight = scribus.epubReadingOrderPreflight()
assert preflight["status"] == "unsupported-content", preflight
assert "EPUB Emphasis" in preflight["detail"], preflight
preflight = scribus.epubReadingOrderPreflight({}, {"EPUB Emphasis": "emphasis"})
assert preflight["status"] == "ready", preflight
assert preflight["paragraphs"] == ["Emphasized text"], preflight
assert preflight["inlineRuns"] == [[("emphasis", "Emphasized text")]], preflight
scribus.closeDoc()
print("EPUB_ORDER_PERSISTENCE_PASSED", flush=True)
