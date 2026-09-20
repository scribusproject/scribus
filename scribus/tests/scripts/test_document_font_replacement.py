#!/usr/bin/env python3

"""End-to-end regression test for document font replacement."""

import os
import tempfile

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def selected_font(frame_name):
    scribus.selectText(0, -1, frame_name)
    return scribus.getFont(frame_name)


def make_text_frame(name, text, x, y):
    frame = scribus.createText(x, y, 220, 45, name)
    scribus.setText(text, frame)
    scribus.selectText(0, -1, frame)
    return frame


available_fonts = scribus.getFontNames()
check(len(available_fonts) >= 2, "font replacement test requires two fonts")
source_font = available_fonts[0]
replacement_font = next(font for font in available_fonts if font != source_font)

output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
document_path = os.path.join(output_dir, "document-font-replacement.sla")
if os.path.exists(document_path):
    os.remove(document_path)

check(
    scribus.newDocument(
        scribus.PAPER_A4,
        (20, 20, 20, 20),
        scribus.PORTRAIT,
        1,
        scribus.UNIT_POINTS,
        scribus.PAGE_1,
        0,
        1,
    ),
    "could not create document",
)

source_frame = make_text_frame("Source Font", "Source font content", 30, 40)
scribus.setFont(source_font, source_frame)
check(selected_font(source_frame) == source_font, "could not assign the source font")

control_frame = make_text_frame("Existing Replacement", "Existing destination content", 30, 100)
scribus.setFont(replacement_font, control_frame)
check(
    selected_font(control_frame) == replacement_font,
    "could not assign the replacement font to the control frame",
)

scribus.createCharStyle(name="Source Character Style", font=source_font)
style_frame = make_text_frame("Styled Source", "Character-style content", 30, 160)
scribus.setCharacterStyle("Source Character Style", style_frame)
check(selected_font(style_frame) == source_font, "character style did not use the source font")

chain_first = scribus.createText(30, 220, 85, 22, "Linked Source 1")
chain_second = scribus.createText(125, 220, 260, 70, "Linked Source 2")
scribus.linkTextFrames(chain_first, chain_second)
scribus.setText("Linked source content " * 12, chain_first)
scribus.selectText(0, -1, chain_first)
scribus.setFont(source_font, chain_first)
check(selected_font(chain_first) == source_font, "linked story did not use the source font")

document_fonts = scribus.listDocumentFonts()
check(source_font in document_fonts, "source font was missing from document font inventory")
check(replacement_font in document_fonts, "replacement font was missing from document font inventory")

check(
    scribus.replaceDocumentFont(source_font, replacement_font),
    "document font replacement reported failure",
)
check(selected_font(source_frame) == replacement_font, "direct formatting was not replaced")
check(selected_font(style_frame) == replacement_font, "character-style formatting was not replaced")
check(selected_font(chain_first) == replacement_font, "linked-story formatting was not replaced")
check(
    selected_font(control_frame) == replacement_font,
    "replacement changed text that already used the destination font",
)
check(source_font not in scribus.listDocumentFonts(), "source font remained in the document inventory")

scribus.undo()
check(selected_font(source_frame) == source_font, "undo did not restore direct formatting")
check(selected_font(style_frame) == source_font, "undo did not restore the character style")
check(selected_font(chain_first) == source_font, "undo did not restore the linked story")
check(
    selected_font(control_frame) == replacement_font,
    "undo corrupted text that originally used the destination font",
)

scribus.redo()
check(selected_font(source_frame) == replacement_font, "redo did not replace direct formatting")
check(selected_font(style_frame) == replacement_font, "redo did not replace the character style")
check(selected_font(chain_first) == replacement_font, "redo did not replace the linked story")
check(
    selected_font(control_frame) == replacement_font,
    "redo changed destination-font control text",
)

scribus.saveDocAs(document_path)
scribus.closeDoc()
check(scribus.openDoc(document_path), "could not reopen saved document")
check(selected_font(source_frame) == replacement_font, "direct replacement did not persist")
check(selected_font(style_frame) == replacement_font, "style replacement did not persist")
check(selected_font(chain_first) == replacement_font, "linked-story replacement did not persist")
check(
    selected_font(control_frame) == replacement_font,
    "destination-font control changed after reload",
)
check(source_font not in scribus.listDocumentFonts(), "source font returned after reload")

# Exercise a second undo after reload, then mutate the linked story. If undo had
# restored independent StoryText copies for each frame, the second frame would
# retain its old text and never receive the marker.
check(
    scribus.replaceDocumentFont(replacement_font, source_font),
    "reverse replacement reported failure",
)
scribus.undo()
linked_marker_text = ("A " * 12) + "CHAIN_SHARING_MARKER"
scribus.selectText(0, 0, chain_first)
scribus.setText(linked_marker_text, chain_first)
scribus.layoutTextChain(chain_first)
check(
    "CHAIN_SHARING_MARKER" in scribus.getText(chain_second),
    "undo broke shared text storage in a linked frame chain",
)
scribus.closeDoc()

print("DOCUMENT_FONT_REPLACEMENT_QA_PASSED", flush=True)
