#!/usr/bin/env python3

"""Audit imported page structure without exposing document contents or paths.

Set SCRIBUS_IMPORT_QA_ROUNDTRIP=1 to verify a temporary SLA save/reopen too.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import math
import os
from pathlib import Path
from tempfile import TemporaryDirectory

import scribus


source = os.environ["SCRIBUS_IMPORT_QA_FILE"]
assert scribus.openDoc(source), "Document import failed"
source_dir = Path(source).resolve().parent
roundtrip_requested = os.environ.get("SCRIBUS_IMPORT_QA_ROUNDTRIP") == "1"

pages = scribus.pageCount()
assert pages > 0, "Import created no pages"

page_counts = []
original_items = {}
original_owners = {}
original_text = {}
group_children = 0
types = {}
text_nonempty = 0
images_linked = 0
images_missing = 0
invalid_image_scales = 0

for page in range(pages):
    names = scribus.getAllObjects(page=page)
    page_counts.append(len(names))
    for name in names:
        kind = scribus.getObjectType(name)
        if roundtrip_requested:
            original_items[name] = (kind, scribus.getPosition(name), scribus.getSize(name))
            original_owners[name] = page
        types[kind] = types.get(kind, 0) + 1
        if kind == "Group":
            children = scribus.getGroupItems(name, recursive=True)
            group_children += len(children)
        elif kind in ("TextFrame", "PathText"):
            if roundtrip_requested:
                original_text[name] = scribus.getAllText(name)
            if scribus.getFrameText(name):
                text_nonempty += 1
        elif kind == "ImageFrame":
            if not all(math.isfinite(scale) for scale in scribus.getImageScale(name)):
                invalid_image_scales += 1
            image_path = scribus.getImageFile(name)
            if image_path:
                image_file = Path(image_path)
                if not image_file.is_absolute():
                    image_file = source_dir / image_file
                if image_file.is_file():
                    images_linked += 1
                else:
                    images_missing += 1

objects = sum(page_counts)
assert objects > 0, "Import created no page objects"

print(
    "IMPORT_FIDELITY_QA pages=%d populated_pages=%d top_level_objects=%d "
    "group_children=%d top_level_text_frames=%d nonempty_text_frames=%d "
    "top_level_image_frames=%d linked_images=%d missing_images=%d "
    "invalid_image_scales=%d "
    "paragraph_styles=%d character_styles=%d"
    % (
        pages,
        sum(count > 0 for count in page_counts),
        objects,
        group_children,
        types.get("TextFrame", 0) + types.get("PathText", 0),
        text_nonempty,
        types.get("ImageFrame", 0),
        images_linked,
        images_missing,
        invalid_image_scales,
        len(scribus.getParagraphStyles()),
        len(scribus.getCharStyles()),
    ),
    flush=True,
)

if roundtrip_requested:
    original_types = types.copy()
    original_page_counts = page_counts[:]
    original_paragraph_styles = len(scribus.getParagraphStyles())
    original_character_styles = len(scribus.getCharStyles())
    with TemporaryDirectory(prefix="scribus-import-roundtrip-") as directory:
        destination = Path(directory) / "imported.sla"
        scribus.saveDocAs(str(destination))
        scribus.closeDoc()
        assert destination.is_file(), "Imported document was not saved as SLA"
        assert scribus.openDoc(str(destination)), "Saved imported document could not be reopened"
        assert scribus.pageCount() == pages, "Page count changed after SLA round-trip"
        reopened_page_counts = []
        reopened_types = {}
        reopened_items = {}
        reopened_owners = {}
        reopened_text = {}
        for page in range(pages):
            names = scribus.getAllObjects(page=page)
            reopened_page_counts.append(len(names))
            for name in names:
                kind = scribus.getObjectType(name)
                reopened_items[name] = (kind, scribus.getPosition(name), scribus.getSize(name))
                reopened_owners[name] = page
                if kind in ("TextFrame", "PathText"):
                    reopened_text[name] = scribus.getAllText(name)
                reopened_types[kind] = reopened_types.get(kind, 0) + 1
        changed_pages = [
            (page + 1, before, after)
            for page, (before, after) in enumerate(zip(original_page_counts, reopened_page_counts))
            if before != after
        ]
        assert set(reopened_items) == set(original_items), (
            "Object identities changed after SLA round-trip: lost=%d added=%d"
            % (len(set(original_items) - set(reopened_items)), len(set(reopened_items) - set(original_items)))
        )
        assert reopened_types == original_types, "Object types changed after SLA round-trip"
        changed_stories = sum(original_text[name] != reopened_text[name] for name in original_text)
        assert changed_stories == 0, (
            "%d text frames changed story content after SLA round-trip" % changed_stories
        )
        changed_types = sum(original_items[name][0] != reopened_items[name][0] for name in original_items)
        geometry_deltas = [
            max(
                abs(before - after)
                for before, after in zip(
                    original_items[name][1] + original_items[name][2],
                    reopened_items[name][1] + reopened_items[name][2],
                )
            )
            for name in original_items
        ]
        changed_geometry = sum(delta > 0.01 for delta in geometry_deltas)
        assert changed_types == 0 and changed_geometry == 0, (
            "Object types or geometries changed after SLA round-trip: types=%d geometry=%d max_delta=%.6f"
            % (changed_types, changed_geometry, max(geometry_deltas, default=0.0))
        )
        assert len(scribus.getParagraphStyles()) == original_paragraph_styles, (
            "Paragraph style count changed after SLA round-trip"
        )
        assert len(scribus.getCharStyles()) == original_character_styles, (
            "Character style count changed after SLA round-trip"
        )
        changed_owners = sum(original_owners[name] != reopened_owners[name] for name in original_owners)
        if changed_pages or changed_owners:
            print(
                "IMPORT_FIDELITY_PAGE_OWNERSHIP_CHANGED pages=%d objects=%d"
                % (len(changed_pages), changed_owners),
                flush=True,
            )
        scribus.closeDoc()
    print("IMPORT_FIDELITY_ROUNDTRIP_PASSED", flush=True)
else:
    scribus.closeDoc()
