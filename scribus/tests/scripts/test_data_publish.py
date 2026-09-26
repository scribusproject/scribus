#!/usr/bin/env python3

"""Headless Phase 5 mail-merge and catalogue publishing regression.

For general Scribus (>=1.3.2) copyright and licensing information please refer
to the COPYING file provided with the program. Following this notice may exist
a copyright and/or license notice that predates the release of Scribus 1.3.2
for which a new license (GPL+exception) is in place.
"""

import csv
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

import scribus


scripts_dir = Path(__file__).resolve().parents[2] / "plugins" / "scriptplugin" / "scripts"
sys.path.insert(0, str(scripts_dir))
import DataPublish  # noqa: E402


def check(condition, message):
    if not condition:
        raise AssertionError(message)


test_root = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(test_root, exist_ok=True)
with tempfile.TemporaryDirectory(prefix="scribus-publish-", dir=test_root) as folder:
    work = Path(folder)
    source = work / "people.csv"
    with source.open("w", encoding="utf-8", newline="") as target:
        writer = csv.DictWriter(target, fieldnames=["Recipient", "Picture"])
        writer.writeheader()
        writer.writerow({"Recipient": "Alpha Person", "Picture": "sample.png"})
        writer.writerow({"Recipient": "Beta Person", "Picture": "sample.png"})
    sample_image = Path(__file__).resolve().parents[3] / "resources" / "editorconfig" / "povray_32.png"
    shutil.copyfile(sample_image, work / "sample.png")

    check(scribus.newDocument(
        scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT,
        1, scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
    ), "could not create mail-merge template")
    variable = scribus.createVariable("Recipient", "Template Person")
    frame = scribus.createText(40, 40, 400, 100, "RecipientFrame")
    scribus.insertVariable(variable, frame)
    template = work / "template.sla"
    scribus.saveDocAs(str(template))
    scribus.closeDoc()

    mail_output = work / "mail-output"
    mail_output.mkdir()
    mail_job = work / "mail-job.json"
    mail_job.write_text(json.dumps({
        "schema_version": 1, "mode": "mail_merge",
        "source": "people.csv", "template": "template.sla",
        "output_directory": "mail-output", "prefix": "letter",
        "first_record": 2, "last_record": 2,
        "filename_field": "Recipient",
    }), encoding="utf-8")
    mail_result = DataPublish.run_job(str(mail_job))
    check(mail_result["records_published"] == 1, "mail job ignored record range")
    check(len(mail_result["outputs"]) == 1, "mail job did not create one PDF")
    check(Path(mail_result["outputs"][0]).name == "letter-record-0002-Beta Person.pdf",
          "mail job filename was wrong")
    check((mail_output / "publish-manifest.json").is_file(), "mail manifest was not created")
    if shutil.which("pdftotext"):
        rendered = subprocess.run(
            ["pdftotext", mail_result["outputs"][0], "-"], check=True,
            capture_output=True, text=True,
        ).stdout
        check("Beta Person" in rendered, "mail PDF did not render the selected record")

    catalogue_output = work / "catalogue-output"
    catalogue_output.mkdir()
    catalogue_job = work / "catalogue-job.json"
    catalogue_spec = {
        "schema_version": 1, "mode": "catalogue",
        "source": "people.csv", "output_directory": "catalogue-output",
        "page": {"width": 300, "height": 200, "margin": 10},
        "grid": {"columns": 2, "rows": 1, "horizontal_gap": 10},
        "fields": [
            {"source": "Picture", "type": "image", "x": 5, "y": 5,
             "width": 80, "height": 80},
            {"source": "Recipient", "type": "text", "x": 5, "y": 100,
             "width": 120, "height": 45, "font_size": 11},
        ],
        "pdf_file": "people-catalogue.pdf", "sla_file": "people-catalogue.sla",
    }
    catalogue_job.write_text(json.dumps(catalogue_spec), encoding="utf-8")
    catalogue_result = DataPublish.run_job(str(catalogue_job))
    check(catalogue_result["records_published"] == 2, "catalogue record count was wrong")
    check(len(catalogue_result["outputs"]) == 2, "catalogue outputs were not recorded")
    check(all(Path(path).is_file() for path in catalogue_result["outputs"]),
          "catalogue PDF or SLA was not created")
    if shutil.which("pdftotext"):
        rendered = subprocess.run(
            ["pdftotext", str(catalogue_output / "people-catalogue.pdf"), "-"],
            check=True, capture_output=True, text=True,
        ).stdout
        check("Alpha Person" in rendered and "Beta Person" in rendered,
              "catalogue text did not render both records")

    invalid_output = work / "invalid-output"
    invalid_output.mkdir()
    invalid_job = work / "invalid-job.json"
    invalid_spec = dict(catalogue_spec, output_directory="invalid-output")
    invalid_job.write_text(json.dumps(invalid_spec), encoding="utf-8")
    source.write_text("Recipient,Picture\nBroken,missing.png\n", encoding="utf-8")
    try:
        DataPublish.run_job(str(invalid_job))
    except FileNotFoundError:
        pass
    else:
        raise AssertionError("missing catalogue image was accepted")
    check(not any(invalid_output.iterdir()), "invalid catalogue job left output files")

    overflow_output = work / "overflow-output"
    overflow_output.mkdir()
    overflow_spec = dict(catalogue_spec, output_directory="overflow-output")
    overflow_job = work / "overflow-job.json"
    overflow_job.write_text(json.dumps(overflow_spec), encoding="utf-8")
    source.write_text("Recipient,Picture\n" + "Long text " * 500 + ",sample.png\n",
                      encoding="utf-8")
    try:
        DataPublish.run_job(str(overflow_job))
    except ValueError as error:
        check("overflow" in str(error).lower(), "overflow error was not explained")
    else:
        raise AssertionError("catalogue text overflow was accepted")
    check(not any(overflow_output.iterdir()), "overflowing catalogue left output files")

    collision_spec = dict(catalogue_spec, output_directory="overflow-output",
                          manifest_file="people-catalogue.pdf")
    collision_job = work / "collision-job.json"
    collision_job.write_text(json.dumps(collision_spec), encoding="utf-8")
    try:
        DataPublish.run_job(str(collision_job))
    except ValueError as error:
        check("manifest filename" in str(error), "output collision was not explained")
    else:
        raise AssertionError("manifest/PDF filename collision was accepted")
    check(not any(overflow_output.iterdir()), "colliding output names left files")

print("DATA_PUBLISH_TEST_PASSED", flush=True)
