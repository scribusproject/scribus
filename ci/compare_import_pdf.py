#!/usr/bin/env python3

"""Render two PDFs and make a private, page-by-page import-fidelity report.

This is a diagnostic comparison, not a pass/fail test: font substitution,
different PDF settings, and edition changes can all affect pixel differences.
Requires Poppler's pdfinfo/pdftoppm plus Pillow and NumPy.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import argparse
import json
import subprocess
from pathlib import Path

import numpy as np
from PIL import Image, ImageDraw, ImageOps


def page_count(path):
    output = subprocess.check_output(["pdfinfo", str(path)], text=True)
    for line in output.splitlines():
        if line.startswith("Pages:"):
            return int(line.split(":", 1)[1].strip())
    raise ValueError("pdfinfo did not report a page count")


def render_page(path, number, prefix):
    subprocess.run(
        ["pdftoppm", "-f", str(number), "-l", str(number), "-r", "72", "-png",
         "-singlefile", str(path), str(prefix)],
        check=True,
        stdout=subprocess.DEVNULL,
    )
    with Image.open(str(prefix) + ".png") as image:
        return image.convert("RGB")


def compare(reference, candidate):
    size = (max(reference.width, candidate.width), max(reference.height, candidate.height))
    ref_canvas = Image.new("RGB", size, "white")
    candidate_canvas = Image.new("RGB", size, "white")
    ref_canvas.paste(reference, (0, 0))
    candidate_canvas.paste(candidate, (0, 0))
    difference = np.abs(
        np.asarray(ref_canvas, dtype=np.int16) - np.asarray(candidate_canvas, dtype=np.int16)
    )
    intensity = difference.mean(axis=2)
    heat = ImageOps.colorize(
        Image.fromarray(np.clip(intensity * 3, 0, 255).astype(np.uint8)),
        black="black", white="red",
    )
    return {
        "reference_pixels": list(reference.size),
        "candidate_pixels": list(candidate.size),
        "mean_absolute_error": round(float(intensity.mean()), 2),
        "changed_pixel_fraction": round(float(np.mean(np.max(difference, axis=2) > 24)), 4),
    }, heat


def contact_sheet(rows, path):
    tile_width = 300
    tile_height = 450
    label_height = 42
    sheet = Image.new("RGB", (tile_width * 3, (tile_height + label_height) * len(rows)), "white")
    draw = ImageDraw.Draw(sheet)
    for row_index, (result, reference, candidate, heat) in enumerate(rows):
        y = row_index * (tile_height + label_height)
        label = "Page %d: error %.1f, changed %.1f%%" % (
            result["reference_page"], result["mean_absolute_error"],
            result["changed_pixel_fraction"] * 100,
        )
        draw.text((8, y + 8), label, fill="black")
        for column, image in enumerate((reference, candidate, heat)):
            thumb = ImageOps.contain(image, (tile_width, tile_height))
            x = column * tile_width + (tile_width - thumb.width) // 2
            sheet.paste(thumb, (x, y + label_height))
    sheet.save(path)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, required=True)
    parser.add_argument("--candidate", type=Path, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--reference-pages", help="Comma-separated original page numbers for candidate pages")
    arguments = parser.parse_args()
    reference_count = page_count(arguments.reference)
    candidate_count = page_count(arguments.candidate)
    reference_pages = (
        [int(value) for value in arguments.reference_pages.split(",")]
        if arguments.reference_pages else list(range(1, reference_count + 1))
    )
    if len(reference_pages) != candidate_count or not all(
        1 <= page <= reference_count for page in reference_pages
    ):
        parser.error("Reference page mapping does not match the candidate PDF")
    arguments.output_dir.mkdir(parents=True, exist_ok=True)
    if any(arguments.output_dir.iterdir()):
        parser.error("Output directory must be empty")

    results = []
    worst = []
    for candidate_page, reference_page in enumerate(reference_pages, 1):
        ref = render_page(
            arguments.reference, reference_page,
            arguments.output_dir / ("reference-%03d" % reference_page),
        )
        current = render_page(
            arguments.candidate, candidate_page,
            arguments.output_dir / ("candidate-%03d" % candidate_page),
        )
        metrics, heat = compare(ref, current)
        result = {"reference_page": reference_page, "candidate_page": candidate_page, **metrics}
        results.append(result)
        worst.append((result, ref, current, heat))
        print(
            "IMPORT_VISUAL_PAGE reference=%d candidate=%d error=%.2f changed=%.1f%%"
            % (reference_page, candidate_page, result["mean_absolute_error"],
               result["changed_pixel_fraction"] * 100),
            flush=True,
        )
    worst.sort(key=lambda row: row[0]["mean_absolute_error"], reverse=True)
    contact_sheet(worst[:6], arguments.output_dir / "worst-pages.png")
    (arguments.output_dir / "metrics.json").write_text(
        json.dumps({"pages": results}, indent=2) + "\n", encoding="utf-8"
    )


if __name__ == "__main__":
    main()
