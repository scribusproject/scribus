#!/usr/bin/env python3

"""Import a small, generated IDML package without proprietary fixtures.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import math
import os
from pathlib import Path
from struct import pack
from xml.etree import ElementTree
from zipfile import ZIP_DEFLATED, ZipFile
from zlib import compress, crc32

import scribus


output = Path(os.environ["SCRIBUS_TEST_OUTPUT_DIR"])
output.mkdir(parents=True, exist_ok=True)
source = output / "minimal-import.idml"


def png_chunk(kind, payload):
    return pack(">I", len(payload)) + kind + payload + pack(">I", crc32(kind + payload))


linked_image = output / "Links" / "linked.png"
linked_image.parent.mkdir(parents=True, exist_ok=True)
linked_image.write_bytes(
    b"\x89PNG\r\n\x1a\n"
    + png_chunk(b"IHDR", pack(">IIBBBBB", 1, 1, 8, 2, 0, 0, 0))
    + png_chunk(b"IDAT", compress(b"\x00\xff\x00\x00"))
    + png_chunk(b"IEND", b"")
)

points = [(0, 0), (80, 0), (80, 60), (0, 60)]
path_points = "\n".join(
    '<PathPointType Anchor="%d %d" LeftDirection="%d %d" RightDirection="%d %d"/>'
    % (x, y, x, y, x, y)
    for x, y in points
)
design_map = """<?xml version="1.0" encoding="UTF-8"?>
<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/" ActiveLayer="Layer/1">
  <Layer Self="Layer/1" Name="Layer 1" Visible="true" Locked="false" Printable="true"/>
  <idPkg:Spread>
    <Spread Self="Spread/1">
      <Page Self="Page/1" ItemTransform="1 0 0 1 0 0"/>
      <Rectangle Self="Rectangle/1" ItemLayer="Layer/1" ItemTransform="1 0 0 1 0 0">
        <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
          %s
        </PathPointArray></GeometryPathType></PathGeometry></Properties>
      </Rectangle>
      <Rectangle Self="Rectangle/2" ItemLayer="Layer/1" ItemTransform="1 0 0 1 100 0">
        <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
          %s
        </PathPointArray></GeometryPathType></PathGeometry></Properties>
        <Image ImageTypeName="PNG"><Link LinkResourceURI="file:/unavailable/linked.png"/></Image>
      </Rectangle>
      <TextFrame Self="TextFrame/2" ParentStory="Story/used" ItemLayer="Layer/1"
                 ItemTransform="1 0 0 1 100 100">
        <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
          %s
        </PathPointArray></GeometryPathType></PathGeometry></Properties>
      </TextFrame>
      <TextFrame Self="TextFrame/1" ParentStory="Story/used" NextTextFrame="TextFrame/2" ItemLayer="Layer/1"
                 ItemTransform="1 0 0 1 0 100">
        <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
          %s
        </PathPointArray></GeometryPathType></PathGeometry></Properties>
      </TextFrame>
    </Spread>
  </idPkg:Spread>
  <idPkg:Story>
    <Story Self="Story/orphan"/>
    <Story Self="Story/used">
      <ParagraphStyleRange><CharacterStyleRange>
        <Content>Before</Content>
        <Rectangle Self="Inline/1" ItemLayer="Layer/1" ItemTransform="1 0 0 1 0 0">
          <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
            %s
          </PathPointArray></GeometryPathType></PathGeometry></Properties>
        </Rectangle>
        <Table Self="Table/1">
          <Row SingleRowHeight="20"/>
          <Column SingleColumnWidth="40"/>
          <Cell Name="malformed"/>
          <Cell Name="99:99"/>
          <Cell Name="0:0"><ParagraphStyleRange><CharacterStyleRange><Content>Cell text</Content></CharacterStyleRange></ParagraphStyleRange></Cell>
        </Table>
        <Table Self="Table/empty"/>
        <Content>After</Content>
      </CharacterStyleRange></ParagraphStyleRange>
    </Story>
  </idPkg:Story>
</Document>
""" % (path_points, path_points, path_points, path_points, path_points)

with ZipFile(source, "w", ZIP_DEFLATED) as archive:
    archive.writestr("mimetype", "application/vnd.adobe.indesign-idml-package")
    archive.writestr("designmap.xml", design_map)

assert scribus.openDoc(str(source)), "Minimal IDML import failed"
assert scribus.pageCount() == 1, "Minimal IDML page count changed"
objects = scribus.getAllObjects(page=0)
assert len(objects) == 4, "Minimal IDML objects were not imported"
types = {scribus.getObjectType(name): name for name in objects}
assert "Polygon" in types, "Minimal IDML shape type changed"
assert "ImageFrame" in types, "Minimal IDML image type changed"
assert sum(scribus.getObjectType(name) == "TextFrame" for name in objects) == 2, (
    "Minimal IDML linked text frames changed"
)
actual_text = scribus.getAllText("TextFrame/1")
assert "Before" in actual_text and "After" in actual_text, "IDML inline text was lost: %r" % actual_text
assert actual_text.index("Before") < actual_text.index("After"), "IDML inline text order changed"
assert actual_text.count("\x19") == 2, "IDML inline objects were not retained"
assert Path(scribus.getImageFile(types["ImageFrame"])).resolve() == linked_image.resolve(), (
    "IDML image link was not resolved beside its source"
)
assert scribus.getImageColorSpace(types["ImageFrame"]) == 0, "Linked RGB image was not loaded"
assert all(math.isfinite(scale) for scale in scribus.getImageScale(types["ImageFrame"])), (
    "Linked image has invalid scale"
)
roundtrip = output / "minimal-import.sla"
scribus.saveDocAs(str(roundtrip))
scribus.closeDoc()
assert roundtrip.is_file(), "Imported IDML document was not saved as SLA"
saved_document = ElementTree.parse(roundtrip)
inline_tables = [
    frame for frame in saved_document.iter("FrameObject") if frame.find("TableData") is not None
]
assert len(inline_tables) == 1, "IDML inline table was not saved in SLA"
assert inline_tables[0].find("./TableData/Cell/StoryText/Content").get("Chars") == "Cell text", (
    "IDML inline table cell text was not saved in SLA"
)
assert scribus.openDoc(str(roundtrip)), "Could not reopen imported IDML as SLA"
assert scribus.pageCount() == 1, "IDML page count changed after SLA round-trip"
reopened_objects = scribus.getAllObjects(page=0)
assert len(reopened_objects) == 4, "IDML objects changed after SLA round-trip"
reopened_types = {scribus.getObjectType(name): name for name in reopened_objects}
assert "Polygon" in reopened_types and "ImageFrame" in reopened_types, (
    "IDML object types changed after SLA round-trip"
)
assert sum(scribus.getObjectType(name) == "TextFrame" for name in reopened_objects) == 2, (
    "IDML linked text frames changed after SLA round-trip"
)
reopened_text = scribus.getAllText("TextFrame/1")
assert "Before" in reopened_text and "After" in reopened_text, (
    "IDML inline text was lost after SLA round-trip: %r" % reopened_text
)
assert reopened_text.index("Before") < reopened_text.index("After"), (
    "IDML inline text order changed after SLA round-trip"
)
assert reopened_text.count("\x19") == 2, "IDML inline objects changed after SLA round-trip"
assert Path(scribus.getImageFile(reopened_types["ImageFrame"])).resolve() == linked_image.resolve(), (
    "IDML image link changed after SLA round-trip"
)
scribus.closeDoc()

blank_page = output / "blank-page.idml"
with ZipFile(blank_page, "w", ZIP_DEFLATED) as archive:
    archive.writestr(
        "designmap.xml",
        '<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">'
        '<idPkg:Spread><Spread Self="Spread/blank"><Page Self="Page/blank"/>'
        '</Spread></idPkg:Spread></Document>',
    )
assert scribus.openDoc(str(blank_page)), "Blank IDML page was rejected"
assert scribus.pageCount() == 1 and not scribus.getAllObjects(page=0), (
    "Blank IDML page changed during import"
)
scribus.closeDoc()
print("IDML_MINIMAL_IMPORT_PASSED", flush=True)
