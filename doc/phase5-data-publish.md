# Phase 5 data publishing (test feature)

Scribus now accepts CSV and JSON data sources for document variables. Open **Edit → Variables… → Data Merge…** in a document to preview records, map fields to user-defined variables, save/load a mapping preset, apply one record, or export a numbered PDF for a selected record range. A source field can also be appended to each PDF filename. The mapping preset is a small JSON file with `version: 1` and a `fields` object whose keys are source field names and values are document variable names.

For unattended jobs, run the installed `DataPublish.py` through Scribus Scripter:

```sh
Scribus --no-splash --no-gui --python-script /path/to/DataPublish.py /path/to/job.json
```

The job is JSON with `schema_version: 1`, `mode`, `source`, and `output_directory`. Relative paths in the job resolve from the job file's folder. Relative image paths in a catalogue source resolve from the source file's folder. The output folder must already exist. Existing output files are not intentionally overwritten.

Mail merge example:

```json
{
  "schema_version": 1,
  "mode": "mail_merge",
  "source": "customers.csv",
  "template": "letter.sla",
  "output_directory": "published",
  "mapping": {"Customer Name": "Recipient"},
  "first_record": 1,
  "last_record": -1,
  "filename_field": "Customer Name",
  "prefix": "letter"
}
```

The mapping is optional if source fields match user-variable names. Record positions are one-based and inclusive. `last_record: -1` means the final record. Each record receives its own PDF. Critical preflight errors (missing images/glyphs, text overflow, broken cross-references) stop export before the affected record; any PDFs completed earlier remain in the output folder. The template's variables and PDF settings are restored after the run.

Catalogue example:

```json
{
  "schema_version": 1,
  "mode": "catalogue",
  "source": "products.csv",
  "output_directory": "published",
  "page": {"width": 595, "height": 842, "margin": 36},
  "grid": {"columns": 2, "rows": 3, "horizontal_gap": 12, "vertical_gap": 12},
  "fields": [
    {"source": "Photo", "type": "image", "x": 0, "y": 0, "width": 240, "height": 180},
    {"source": "Name", "type": "text", "x": 0, "y": 188, "width": 240, "height": 40, "font_size": 14}
  ],
  "pdf_file": "catalogue.pdf",
  "sla_file": "catalogue.sla"
}
```

Page and field dimensions are points. Field positions are relative to each grid card. `type` defaults to `text`; `sla_file` is optional. The job verifies referenced images and text-frame overflow, checks critical preflight errors, then writes the PDF and optional editable SLA. This is a generated card grid, not yet an arbitrary template-driven catalogue layout.

On success, both modes write `publish-manifest.json` (or the optional `manifest_file` name) with the source, output paths, record count and UTC time. Headless Scripter now returns a failing process status for an unhandled Python error. Automation should also verify the `DATA_PUBLISH_JOB_PASSED` console marker and manifest. A failed run can leave completed mail-merge PDFs or a partially written output; use a fresh output folder per run.

Combined mail-merge PDF output is **not implemented**. The macOS integration tests passed (32/32 CTest cases), and the full-app Phase 5 runtime workflows passed on Linux and on a packaged Windows x64 application for test branch commit `78eeee9`. These automated checks do not replace a manual visual review of the Data Merge dialog on all three platforms.
