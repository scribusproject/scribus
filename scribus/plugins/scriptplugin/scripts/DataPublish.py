#!/usr/bin/env python3

"""Run a validated mail-merge or catalogue job inside Scribus Scripter.

Usage: Scribus --no-gui --python-script DataPublish.py job.json

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import json
import os
import sys
from datetime import datetime, timezone
from pathlib import Path

import scribus


MAX_JOB_BYTES = 1024 * 1024


def _require_object(value, label):
    if not isinstance(value, dict):
        raise ValueError(f"{label} must be an object")
    return value


def _require_int(value, label, minimum=1):
    if isinstance(value, bool) or not isinstance(value, int) or value < minimum:
        raise ValueError(f"{label} must be an integer of at least {minimum}")
    return value


def _require_number(value, label, minimum=0):
    if isinstance(value, bool) or not isinstance(value, (int, float)) or value < minimum:
        raise ValueError(f"{label} must be a number of at least {minimum}")
    return float(value)


def _require_string(value, label):
    if not isinstance(value, str) or not value:
        raise ValueError(f"{label} must be a non-empty string")
    return value


def _resolve(job_dir, value, label):
    raw = Path(_require_string(value, label)).expanduser()
    return (raw if raw.is_absolute() else job_dir / raw).resolve()


def _output_path(output_dir, filename, label):
    name = _require_string(filename, label)
    if name in (".", "..") or name.endswith((" ", ".")) or any(
        ord(char) < 32 or char in '<>:"/\\|?*' for char in name
    ):
        raise ValueError(f"{label} is not a portable filename")
    path = output_dir / name
    if path.exists() or path.is_symlink():
        raise FileExistsError(f"output already exists: {path}")
    return path


def _record_range(job, count):
    start = _require_int(job.get("first_record", 1), "first_record")
    end = job.get("last_record", count)
    if end == -1:
        end = count
    end = _require_int(end, "last_record")
    if not 1 <= start <= end <= count:
        raise ValueError("record range is outside the data source")
    return start, end


def _read_job(job_path):
    if not job_path.is_file() or job_path.stat().st_size > MAX_JOB_BYTES:
        raise ValueError("job file is missing or exceeds the 1 MB limit")
    with job_path.open("r", encoding="utf-8") as source:
        job = _require_object(json.load(source), "job")
    if job.get("schema_version") != 1:
        raise ValueError("schema_version must be 1")
    if job.get("mode") not in ("mail_merge", "catalogue"):
        raise ValueError("mode must be mail_merge or catalogue")
    return job


def _preflight_critical_errors():
    """Reject defects that make a published document incomplete or misleading."""
    report = json.loads(scribus.exportDocumentCheck())
    critical = {"MissingImage", "MissingGlyph", "TextOverflow", "BrokenCrossReference"}
    found = []
    for section in ("pages", "masterPages"):
        for page, items in report.get(section, {}).items():
            for item in items:
                if item.get("error") in critical:
                    found.append(f"{section} {page}: {item.get('item') or 'page'}: {item['error']}")
    for style, errors in report.get("styles", {}).items():
        found.extend(f"style {style}: {error}" for error in errors if error in critical)
    if found:
        raise ValueError("preflight failed: " + "; ".join(found[:10]))


def _mail_merge(job, job_dir, source_path, output_dir, records):
    template = _resolve(job_dir, job.get("template"), "template")
    if not template.is_file():
        raise FileNotFoundError(f"template does not exist: {template}")
    mapping = job.get("mapping")
    if mapping is not None:
        _require_object(mapping, "mapping")
        if any(not isinstance(key, str) or not isinstance(value, str)
               for key, value in mapping.items()):
            raise ValueError("mapping must contain string field and variable names")
    prefix = job.get("prefix", "")
    filename_field = job.get("filename_field", "")
    if not isinstance(prefix, str) or not isinstance(filename_field, str):
        raise ValueError("prefix and filename_field must be strings")
    first, last = _record_range(job, len(records))
    if not scribus.openDoc(str(template)):
        raise RuntimeError(f"could not open template: {template}")
    try:
        _preflight_critical_errors()
        outputs = scribus.exportDataMergePDFs(
            str(source_path), str(output_dir), mapping, prefix, first, last,
            filename_field, True,
        )
    finally:
        scribus.closeDoc()
    return [str(path) for path in outputs], last - first + 1


def _catalogue(job, source_path, output_dir, records):
    first, last = _record_range(job, len(records))
    selected = records[first - 1:last]
    page = _require_object(job.get("page"), "page")
    width = _require_number(page.get("width"), "page.width", 1)
    height = _require_number(page.get("height"), "page.height", 1)
    margin = _require_number(page.get("margin", 36), "page.margin")
    grid = _require_object(job.get("grid"), "grid")
    columns = _require_int(grid.get("columns"), "grid.columns")
    rows = _require_int(grid.get("rows"), "grid.rows")
    gap_x = _require_number(grid.get("horizontal_gap", 0), "grid.horizontal_gap")
    gap_y = _require_number(grid.get("vertical_gap", 0), "grid.vertical_gap")
    cell_width = (width - 2 * margin - (columns - 1) * gap_x) / columns
    cell_height = (height - 2 * margin - (rows - 1) * gap_y) / rows
    if cell_width <= 0 or cell_height <= 0:
        raise ValueError("page margins and grid gaps leave no room for catalogue cards")
    fields = job.get("fields")
    if not isinstance(fields, list) or not fields:
        raise ValueError("fields must be a non-empty array")
    source_fields = set(records[0])
    validated_fields = []
    for number, field in enumerate(fields, 1):
        field = _require_object(field, f"fields[{number}]")
        source = _require_string(field.get("source"), f"fields[{number}].source")
        if source not in source_fields:
            raise ValueError(f"catalogue field is not in the source: {source}")
        kind = field.get("type", "text")
        if kind not in ("text", "image"):
            raise ValueError(f"fields[{number}].type must be text or image")
        x = _require_number(field.get("x"), f"fields[{number}].x")
        y = _require_number(field.get("y"), f"fields[{number}].y")
        frame_width = _require_number(field.get("width"), f"fields[{number}].width", 1)
        frame_height = _require_number(field.get("height"), f"fields[{number}].height", 1)
        if x + frame_width > cell_width or y + frame_height > cell_height:
            raise ValueError(f"fields[{number}] extends outside its catalogue card")
        size = _require_number(field.get("font_size", 11), f"fields[{number}].font_size", 1)
        validated_fields.append((source, kind, x, y, frame_width, frame_height, size))

    image_paths = {}
    for record_index, record in enumerate(selected, first):
        for source, kind, *_ in validated_fields:
            if kind != "image":
                continue
            raw = _require_string(record.get(source), f"record {record_index} image field {source}")
            image = Path(raw).expanduser()
            if not image.is_absolute():
                image = source_path.parent / image
            image = image.resolve()
            if not image.is_file():
                raise FileNotFoundError(f"record {record_index} image is missing: {image}")
            image_paths[(record_index, source)] = image

    pdf_path = _output_path(output_dir, job.get("pdf_file", "catalogue.pdf"), "pdf_file")
    sla_name = job.get("sla_file")
    sla_path = _output_path(output_dir, sla_name, "sla_file") if sla_name else None
    if sla_path == pdf_path:
        raise ValueError("PDF and SLA output paths must differ")
    if not scribus.newDocument(
        (width, height), (margin, margin, margin, margin), scribus.PORTRAIT,
        1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
    ):
        raise RuntimeError("could not create catalogue document")
    try:
        per_page = columns * rows
        for index, record in enumerate(selected):
            if index and index % per_page == 0:
                scribus.newPage(-1)
                scribus.gotoPage(index // per_page + 1)
            slot = index % per_page
            column = slot % columns
            row = slot // columns
            card_x = margin + column * (cell_width + gap_x)
            card_y = margin + row * (cell_height + gap_y)
            for field_index, (source, kind, x, y, fw, fh, size) in enumerate(validated_fields):
                name = f"Catalog-{first + index}-{field_index + 1}"
                if kind == "image":
                    frame = scribus.createImage(card_x + x, card_y + y, fw, fh, name)
                    scribus.loadImage(str(image_paths[(first + index, source)]), frame)
                    scribus.setScaleImageToFrame(True, True, frame)
                else:
                    frame = scribus.createText(card_x + x, card_y + y, fw, fh, name)
                    scribus.setText(record.get(source, ""), frame)
                    scribus.setFontSize(size, frame)
                    if scribus.textOverflows(frame):
                        raise ValueError(f"record {first + index} field '{source}' overflows its text frame")
        _preflight_critical_errors()
        pdf = scribus.PDFfile()
        pdf.file = str(pdf_path)
        pdf.compressmtd = 2  # Lossless ZIP is robust for mixed catalogue images.
        pdf.save()
        if not pdf_path.is_file() or pdf_path.stat().st_size == 0:
            raise RuntimeError(f"PDF export did not create a usable file: {pdf_path}")
        if sla_path:
            scribus.saveDocAs(str(sla_path))
            if not sla_path.is_file() or sla_path.stat().st_size == 0:
                raise RuntimeError(f"SLA export did not create a usable file: {sla_path}")
    finally:
        scribus.closeDoc()
    outputs = [str(pdf_path)]
    if sla_path:
        outputs.append(str(sla_path))
    return outputs, len(selected)


def run_job(job_path):
    job_path = Path(job_path).expanduser().resolve()
    job = _read_job(job_path)
    job_dir = job_path.parent
    source_path = _resolve(job_dir, job.get("source"), "source")
    if not source_path.is_file():
        raise FileNotFoundError(f"data source does not exist: {source_path}")
    output_dir = _resolve(job_dir, job.get("output_directory"), "output_directory")
    if not output_dir.is_dir():
        raise FileNotFoundError(f"output directory does not exist: {output_dir}")
    manifest_path = _output_path(output_dir, job.get("manifest_file", "publish-manifest.json"),
                                 "manifest_file")
    records = scribus.loadDataSource(str(source_path))
    if not records:
        raise ValueError("data source has no records")
    if job["mode"] == "mail_merge":
        outputs, count = _mail_merge(job, job_dir, source_path, output_dir, records)
    else:
        outputs, count = _catalogue(job, source_path, output_dir, records)
    manifest = {
        "schema_version": 1,
        "mode": job["mode"],
        "source": str(source_path),
        "records_published": count,
        "outputs": outputs,
        "created_utc": datetime.now(timezone.utc).isoformat(),
    }
    with manifest_path.open("x", encoding="utf-8") as target:
        json.dump(manifest, target, ensure_ascii=False, indent=2)
        target.write("\n")
    print("DATA_PUBLISH_JOB_PASSED", str(manifest_path), flush=True)
    return manifest


if __name__ == "__main__":
    if len(sys.argv) != 2:
        raise SystemExit("Usage: DataPublish.py job.json")
    run_job(sys.argv[1])
