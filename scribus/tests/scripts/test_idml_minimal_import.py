#!/usr/bin/env python3

"""Import a small, generated IDML package without proprietary fixtures.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import math
import os
from base64 import b64encode
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
  <Layer Self="Layer/2" Name="Text" Visible="true" Locked="true" Printable="true"/>
  <Layer Self="Layer/3" Name="Hidden" Visible="false" Locked="true" Printable="false"/>
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
      <TextFrame Self="TextFrame/2" ParentStory="Story/used" ItemLayer="Layer/2"
                 ItemTransform="1 0 0 1 100 100">
        <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
          %s
        </PathPointArray></GeometryPathType></PathGeometry></Properties>
      </TextFrame>
      <TextFrame Self="TextFrame/1" ParentStory="Story/used" NextTextFrame="TextFrame/2" ItemLayer="Layer/2"
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
          <Cell Name="0:0">
            <ParagraphStyleRange><CharacterStyleRange><Content>Cell text</Content></CharacterStyleRange></ParagraphStyleRange>
            <Rectangle Self="Inline/CellImage" ItemLayer="Layer/1" ItemTransform="1 0 0 1 0 0">
              <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
                %s
              </PathPointArray></GeometryPathType></PathGeometry></Properties>
              <Image ImageTypeName="PNG"><Link LinkResourceURI="file:/unavailable/linked.png"/></Image>
            </Rectangle>
          </Cell>
        </Table>
        <Table Self="Table/empty"/>
        <Content>After</Content>
      </CharacterStyleRange></ParagraphStyleRange>
    </Story>
  </idPkg:Story>
</Document>
""" % (path_points, path_points, path_points, path_points, path_points, path_points)

with ZipFile(source, "w", ZIP_DEFLATED) as archive:
    archive.writestr("mimetype", "application/vnd.adobe.indesign-idml-package")
    archive.writestr("designmap.xml", design_map)

assert scribus.openDoc(str(source)), "Minimal IDML import failed"
assert scribus.getActiveLayer() == "Layer 1", "IDML active layer changed"
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
saved_layers = {layer.get("Name"): layer for layer in saved_document.iter("Layers")}
assert saved_layers["Layer 1"].get("IsSelectable") == "1", (
    "IDML active layer cannot be selected"
)
assert saved_layers["Text"].get("IsSelectable") == "1", (
    "IDML text layer cannot be selected while another layer is active"
)
assert saved_layers["Text"].get("IsEditable") == "1", (
    "IDML text layer was not unlocked for editing"
)
assert all(layer.get("IsEditable") == "1" for layer in saved_layers.values()), (
    "An IDML layer was left locked"
)
assert saved_layers["Hidden"].get("IsViewable") == "0" and saved_layers["Hidden"].get("IsPrintable") == "0", (
    "Import changed a hidden layer's original visibility or print setting"
)
inline_tables = [
    frame for frame in saved_document.iter("FrameObject") if frame.find("TableData") is not None
]
assert len(inline_tables) == 1, "IDML inline table was not saved in SLA"
assert inline_tables[0].find("./TableData/Cell/StoryText/Content").get("Chars") == "Cell text", (
    "IDML inline table cell text was not saved in SLA"
)
cell_objects = inline_tables[0].findall("./TableData/Cell/StoryText/Content[@Object]")
assert len(cell_objects) == 1, "IDML table-cell image was not saved as an inline object"
inline_frames = {frame.get("InID"): frame for frame in saved_document.iter("FrameObject")}
cell_image = inline_frames.get(cell_objects[0].get("Object"))
assert cell_image is not None and cell_image.get("ImageFileName"), (
    "IDML table-cell image link was not saved in SLA"
)
assert (roundtrip.parent / cell_image.get("ImageFileName")).resolve() == linked_image.resolve(), (
    "IDML table-cell image link changed during SLA save"
)
assert scribus.openDoc(str(roundtrip)), "Could not reopen imported IDML as SLA"
assert scribus.getActiveLayer() == "Layer 1", "IDML active layer changed after SLA reopen"
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
resaved = output / "minimal-import-resaved.sla"
scribus.saveDocAs(str(resaved))
scribus.closeDoc()
resaved_document = ElementTree.parse(resaved)
resaved_frames = {frame.get("InID"): frame for frame in resaved_document.iter("FrameObject")}
resaved_table = next(frame for frame in resaved_frames.values() if frame.find("TableData") is not None)
resaved_cell_objects = resaved_table.findall("./TableData/Cell/StoryText/Content[@Object]")
assert len(resaved_cell_objects) == 1, (
    "IDML table-cell image was lost when reopening SLA"
)
resaved_cell_image = resaved_frames.get(resaved_cell_objects[0].get("Object"))
assert resaved_cell_image is not None and resaved_cell_image.get("ImageFileName"), (
    "IDML table-cell image link was lost when reopening SLA"
)
assert (resaved.parent / resaved_cell_image.get("ImageFileName")).resolve() == linked_image.resolve(), (
    "IDML table-cell image link changed after SLA reopen"
)

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

facing_pages = output / "facing-page-ownership.idml"
facing_map = """<?xml version="1.0" encoding="UTF-8"?>
<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">
  <idPkg:Preferences>
    <DocumentPreference PageWidth="80" PageHeight="60" FacingPages="true"
      DocumentBleedTopOffset="5" DocumentBleedInsideOrLeftOffset="0"
      DocumentBleedOutsideOrRightOffset="5" DocumentBleedBottomOffset="5"/>
    <MarginPreference Top="0" Left="0" Right="0" Bottom="0" ColumnCount="1" ColumnGutter="0"/>
  </idPkg:Preferences>
  <idPkg:Spread><Spread Self="Spread/facing">
    <Page Self="Page/first"/>
    <Page Self="Page/left"/>
    <Page Self="Page/right"/>
    <Rectangle Self="Rectangle/right-background" ItemTransform="1 0 0 1 -0.000000000001 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </Rectangle>
  </Spread></idPkg:Spread>
</Document>
""" % path_points
with ZipFile(facing_pages, "w", ZIP_DEFLATED) as archive:
    archive.writestr("designmap.xml", facing_map)
assert scribus.openDoc(str(facing_pages)), "Facing-page IDML import failed"
assert scribus.pageCount() == 3, "Facing-page IDML page count changed"
assert len(scribus.getAllObjects(page=2)) == 1, "Right-page object was assigned to the wrong page"
facing_roundtrip = output / "facing-page-ownership.sla"
scribus.saveDocAs(str(facing_roundtrip))
scribus.closeDoc()
assert scribus.openDoc(str(facing_roundtrip)), "Facing-page SLA could not be reopened"
assert len(scribus.getAllObjects(page=2)) == 1, "Right-page object ownership changed after SLA reopen"
scribus.closeDoc()

cropped_images = output / "anisotropic-image-crop.idml"
cropped_link = output / "Links" / "cropped.png"
cropped_link.write_bytes(
    b"\x89PNG\r\n\x1a\n"
    + png_chunk(b"IHDR", pack(">IIBBBBB", 1, 1, 8, 2, 0, 0, 0))
    + png_chunk(b"pHYs", pack(">IIB", 2835, 5669, 1))
    + png_chunk(b"IDAT", compress(b"\x00\xff\x00\x00"))
    + png_chunk(b"IEND", b"")
)
crop_map = """<?xml version="1.0" encoding="UTF-8"?>
<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">
  <idPkg:Spread><Spread Self="Spread/crop">
    <Page Self="Page/crop"/>
    <Rectangle Self="Rectangle/linked" ItemTransform="1 0 0 1 0 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
      <FrameFittingOption LeftCrop="5" TopCrop="4"/>
      <Image ImageTypeName="PNG" ItemTransform="2 0 0 3 0 0">
        <Link LinkResourceURI="file:/unavailable/cropped.png"/>
      </Image>
    </Rectangle>
    <Rectangle Self="Rectangle/embedded" ItemTransform="1 0 0 1 100 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
      <FrameFittingOption LeftCrop="5" TopCrop="4"/>
      <Image ImageTypeName="PNG" ItemTransform="2 0 0 3 0 0">
        <Properties><Contents>%s</Contents></Properties>
      </Image>
    </Rectangle>
  </Spread></idPkg:Spread>
</Document>
""" % (path_points, path_points, b64encode(cropped_link.read_bytes()).decode("ascii"))
with ZipFile(cropped_images, "w", ZIP_DEFLATED) as archive:
    archive.writestr("designmap.xml", crop_map)
assert scribus.openDoc(str(cropped_images)), "Cropped-image IDML import failed"
for name in ("Rectangle/linked", "Rectangle/embedded"):
    assert scribus.getObjectType(name) == "ImageFrame", "%s was not imported as an image" % name
    x, y = scribus.getImageOffset(name)
    assert abs(x + 10) < 0.01 and abs(y + 12) < 0.01, (
        "%s crop offset changed: (%f, %f)" % (name, x, y)
    )
    x_scale, y_scale = scribus.getImageScale(name)
    assert abs(x_scale - 2) < 0.01 and abs(y_scale - 3) < 0.01, (
        "%s image scale changed: (%f, %f)" % (name, x_scale, y_scale)
    )
cropped_roundtrip = output / "anisotropic-image-crop.sla"
scribus.saveDocAs(str(cropped_roundtrip))
scribus.closeDoc()
assert scribus.openDoc(str(cropped_roundtrip)), "Cropped-image SLA could not be reopened"
for name in ("Rectangle/linked", "Rectangle/embedded"):
    x, y = scribus.getImageOffset(name)
    x_scale, y_scale = scribus.getImageScale(name)
    assert abs(x + 10) < 0.01 and abs(y + 12) < 0.01, "%s crop changed after SLA reopen" % name
    assert abs(x_scale - 2) < 0.01 and abs(y_scale - 3) < 0.01, (
        "%s scale changed after SLA reopen" % name
    )
scribus.closeDoc()
missing_image = output / "original-location" / "not-present.png"
assert not missing_image.exists(), "Missing-image fixture unexpectedly exists"
missing_link_package = output / "missing-linked-image.idml"
missing_link_map = """<?xml version="1.0" encoding="UTF-8"?>
<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">
  <idPkg:Spread><Spread Self="Spread/missing">
    <Page Self="Page/missing"/>
    <Rectangle Self="Rectangle/missing-image" ItemTransform="1 0 0 1 0 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
      <Image ImageTypeName="PNG"><Link LinkResourceURI="%s"/></Image>
    </Rectangle>
  </Spread></idPkg:Spread>
</Document>
""" % (path_points, missing_image.as_uri())
with ZipFile(missing_link_package, "w", ZIP_DEFLATED) as archive:
    archive.writestr("designmap.xml", missing_link_map)
assert scribus.openDoc(str(missing_link_package)), "Missing-link IDML import failed"
assert scribus.getObjectType("Rectangle/missing-image") == "ImageFrame", "Missing image frame was lost"
assert Path(scribus.getImageFile("Rectangle/missing-image")) == missing_image, (
    "Missing IDML image lost its original path"
)
assert all(math.isfinite(value) for value in scribus.getImageScale("Rectangle/missing-image")), (
    "Missing IDML image has invalid scale"
)
missing_roundtrip = output / "missing-linked-image.sla"
scribus.saveDocAs(str(missing_roundtrip))
scribus.closeDoc()
assert scribus.openDoc(str(missing_roundtrip)), "Missing-link SLA could not be reopened"
assert Path(scribus.getImageFile("Rectangle/missing-image")) == missing_image, (
    "Missing IDML image path changed after SLA reopen"
)
scribus.closeDoc()
leading_package = output / "character-range-leading.idml"
leading_map = """<?xml version="1.0" encoding="UTF-8"?>
<Document xmlns:idPkg="http://ns.adobe.com/AdobeInDesign/idml/1.0/">
  <idPkg:Styles><RootParagraphStyleGroup>
    <ParagraphStyle Self="ParagraphStyle/AutoBase" Name="AutoBase" AutoLeading="150"/>
    <ParagraphStyle Self="ParagraphStyle/AutoChild" Name="AutoChild">
      <Properties><BasedOn type="object">ParagraphStyle/AutoBase</BasedOn></Properties>
    </ParagraphStyle>
  </RootParagraphStyleGroup></idPkg:Styles>
  <idPkg:Spread><Spread Self="Spread/leading">
    <Page Self="Page/leading"/>
    <TextFrame Self="TextFrame/leading" ParentStory="Story/leading"
               ItemTransform="1 0 0 1 0 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
    <TextFrame Self="TextFrame/terminal" ParentStory="Story/terminal"
               ItemTransform="1 0 0 1 100 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
    <TextFrame Self="TextFrame/explicit" ParentStory="Story/explicit"
               ItemTransform="1 0 0 1 200 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
    <TextFrame Self="TextFrame/multiple" ParentStory="Story/multiple"
               ItemTransform="1 0 0 1 300 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
    <TextFrame Self="TextFrame/mixed" ParentStory="Story/mixed"
               ItemTransform="1 0 0 1 400 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
    <TextFrame Self="TextFrame/display" ParentStory="Story/display"
               ItemTransform="1 0 0 1 500 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
    <TextFrame Self="TextFrame/mixed-inline" ParentStory="Story/mixed-inline"
               ItemTransform="1 0 0 1 600 0">
      <Properties><PathGeometry><GeometryPathType PathOpen="false"><PathPointArray>
        %s
      </PathPointArray></GeometryPathType></PathGeometry></Properties>
    </TextFrame>
  </Spread></idPkg:Spread>
  <idPkg:Story><Story Self="Story/leading">
    <ParagraphStyleRange><CharacterStyleRange>
      <Properties><Leading type="unit">18</Leading></Properties>
      <Content>First line</Content><Br/><Content>Second line</Content>
    </CharacterStyleRange></ParagraphStyleRange>
  </Story></idPkg:Story>
  <idPkg:Story><Story Self="Story/terminal">
    <ParagraphStyleRange AppliedParagraphStyle="ParagraphStyle/AutoChild">
      <CharacterStyleRange PointSize="10"><Content>Short</Content>
    </CharacterStyleRange></ParagraphStyleRange>
  </Story></idPkg:Story>
  <idPkg:Story><Story Self="Story/explicit">
    <ParagraphStyleRange><CharacterStyleRange><Content>Keeps break</Content><Br/>
    </CharacterStyleRange></ParagraphStyleRange>
  </Story></idPkg:Story>
  <idPkg:Story><Story Self="Story/multiple">
    <ParagraphStyleRange><CharacterStyleRange><Content>One</Content>
    </CharacterStyleRange></ParagraphStyleRange>
    <ParagraphStyleRange><CharacterStyleRange><Content>Two</Content>
    </CharacterStyleRange></ParagraphStyleRange>
  </Story></idPkg:Story>
  <idPkg:Story><Story Self="Story/mixed">
    <ParagraphStyleRange AppliedParagraphStyle="ParagraphStyle/AutoChild">
      <CharacterStyleRange PointSize="12"><Content>Large</Content></CharacterStyleRange>
      <CharacterStyleRange PointSize="10"><Br/><Br/><Content>Small</Content></CharacterStyleRange>
    </ParagraphStyleRange>
  </Story></idPkg:Story>
  <idPkg:Story><Story Self="Story/display">
    <ParagraphStyleRange AppliedParagraphStyle="ParagraphStyle/AutoChild">
      <CharacterStyleRange PointSize="24"><Content>Title</Content></CharacterStyleRange>
    </ParagraphStyleRange>
  </Story></idPkg:Story>
  <idPkg:Story><Story Self="Story/mixed-inline">
    <ParagraphStyleRange AppliedParagraphStyle="ParagraphStyle/AutoChild">
      <CharacterStyleRange PointSize="10"><Content>Small </Content></CharacterStyleRange>
      <CharacterStyleRange PointSize="12"><Content>large</Content></CharacterStyleRange>
    </ParagraphStyleRange>
  </Story></idPkg:Story>
</Document>
""" % (path_points, path_points, path_points, path_points, path_points, path_points, path_points)
with ZipFile(leading_package, "w", ZIP_DEFLATED) as archive:
    archive.writestr("designmap.xml", leading_map)
assert scribus.openDoc(str(leading_package)), "IDML character-range leading import failed"
assert scribus.getAllText("TextFrame/leading") == "First line\rSecond line", (
    "IDML importer added a paragraph break absent from the source"
)
assert scribus.getAllText("TextFrame/terminal") == "Short", (
    "IDML importer added a terminal paragraph break"
)
scribus.selectText(0, 1, "TextFrame/terminal")
assert scribus.getLineSpacingMode("TextFrame/terminal") == 0, (
    "IDML automatic leading was not converted to point-size-based fixed leading"
)
assert abs(scribus.getLineSpacing("TextFrame/terminal") - 15) < 0.01, (
    "IDML inherited auto-leading percentage was not applied"
)
assert scribus.getAllText("TextFrame/explicit") == "Keeps break\r", (
    "IDML importer removed an explicit terminal paragraph break"
)
assert scribus.getAllText("TextFrame/multiple") == "One\rTwo", (
    "IDML importer changed the break between paragraph-style ranges"
)
scribus.selectText(0, 1, "TextFrame/leading")
assert abs(scribus.getLineSpacing("TextFrame/leading") - 18) < 0.01, (
    "IDML character-range leading was not applied"
)
assert scribus.getAllText("TextFrame/mixed") == "Large\r\rSmall", (
    "IDML mixed-size paragraph boundaries changed"
)
scribus.selectText(0, 1, "TextFrame/display")
assert scribus.getLineSpacingMode("TextFrame/display") == 1, (
    "IDML display type lost its first-line offset behavior"
)
scribus.selectText(0, 1, "TextFrame/mixed-inline")
assert scribus.getLineSpacingMode("TextFrame/mixed-inline") == 1, (
    "IDML mixed-size line was given paragraph-wide fixed leading"
)
leading_roundtrip = output / "character-range-leading.sla"
scribus.saveDocAs(str(leading_roundtrip))
scribus.closeDoc()
saved_leading = ElementTree.parse(leading_roundtrip)
mixed_frame = next(
    frame for frame in saved_leading.iter("PageObject")
    if frame.get("AutoName") == "TextFrame/mixed"
)
mixed_paragraphs = mixed_frame.findall("./StoryText/para")
assert len(mixed_paragraphs) == 2, "IDML blank paragraph was lost"
assert [round(float(para.get("LineSpacing")), 2) for para in mixed_paragraphs] == [18, 15], (
    "IDML automatic leading changed at a mixed-size paragraph boundary"
)
assert scribus.openDoc(str(leading_roundtrip)), "IDML leading SLA could not be reopened"
scribus.selectText(0, 1, "TextFrame/terminal")
assert abs(scribus.getLineSpacing("TextFrame/terminal") - 15) < 0.01, (
    "IDML automatic leading was lost after SLA reopen"
)
scribus.closeDoc()
print("IDML_MINIMAL_IMPORT_PASSED", flush=True)
