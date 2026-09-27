#!/usr/bin/env python3

"""Audit imported page structure without exposing document contents or paths.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import os
from pathlib import Path

import scribus


source = os.environ["SCRIBUS_IMPORT_QA_FILE"]
assert scribus.openDoc(source), "Document import failed"
source_dir = Path(source).resolve().parent

pages = scribus.pageCount()
assert pages > 0, "Import created no pages"

page_counts = []
group_children = 0
types = {}
text_nonempty = 0
images_linked = 0
images_missing = 0

for page in range(pages):
    names = scribus.getAllObjects(page=page)
    page_counts.append(len(names))
    for name in names:
        kind = scribus.getObjectType(name)
        types[kind] = types.get(kind, 0) + 1
        if kind == "Group":
            children = scribus.getGroupItems(name, recursive=True)
            group_children += len(children)
        elif kind in ("TextFrame", "PathText"):
            if scribus.getFrameText(name):
                text_nonempty += 1
        elif kind == "ImageFrame":
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
        len(scribus.getParagraphStyles()),
        len(scribus.getCharStyles()),
    ),
    flush=True,
)
scribus.closeDoc()
