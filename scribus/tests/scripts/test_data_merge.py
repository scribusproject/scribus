#!/usr/bin/env python3

"""Regression test for the first Phase 5 data-record binding slice.

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
import tempfile

import scribus


def check(condition, message):
    if not condition:
        raise AssertionError(message)


def expect_error(action, message):
    try:
        action()
    except Exception:
        return
    raise AssertionError(message)


output_dir = os.environ.get("SCRIBUS_TEST_OUTPUT_DIR", tempfile.gettempdir())
os.makedirs(output_dir, exist_ok=True)
csv_path = os.path.join(output_dir, "data_merge_records.csv")
json_path = os.path.join(output_dir, "data_merge_records.json")
document_path = os.path.join(output_dir, "data_merge_document.sla")


check(
    scribus.newDocument(
        scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
        scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
    ),
    "could not create the data-merge test document",
)
name_id = scribus.createVariable("Customer", "Original")
address_id = scribus.createVariable("Address", "Old address")
frame = scribus.createText(40, 40, 400, 100, "MergeText")
scribus.insertVariable(name_id, frame)
scribus.insertVariable(address_id, frame)

with open(csv_path, "w", encoding="utf-8-sig", newline="") as source:
    writer = csv.DictWriter(source, fieldnames=["Customer", "Address"])
    writer.writeheader()
    writer.writerow({"Customer": "రవి, Kumar", "Address": "First line\nSecond line"})
csv_records = scribus.loadDataSource(csv_path)
check(len(csv_records) == 1, "CSV source did not return one record")
check(csv_records[0]["Customer"] == "రవి, Kumar", "CSV source lost Unicode data")
csv_record = csv_records[0]

check(scribus.applyDataRecord(csv_record) == 2, "CSV record did not match two variables")
check(scribus.getVariable(name_id) == "రవి, Kumar", "quoted Unicode CSV field failed")
check(scribus.getVariable(address_id) == "First line\nSecond line", "multiline CSV field failed")
scribus.undo()
check(scribus.getVariable(name_id) == "Original", "CSV application was not one undo step")
check(scribus.getVariable(address_id) == "Old address", "second field was not undone")
scribus.redo()
check(scribus.getVariable(name_id) == "రవి, Kumar", "redo failed")

with open(json_path, "w", encoding="utf-8") as source:
    json.dump([{"Customer": "Maya", "Address": "42 Example Rd", "Unused": "skip"}], source)
json_records = scribus.loadDataSource(json_path)
check(len(json_records) == 1, "JSON source did not return one record")
check(json_records[0]["Customer"] == "Maya", "JSON source did not return the customer field")
json_record = json_records[0]

expect_error(lambda: scribus.applyDataRecord(json_record), "unknown JSON field was accepted")
check(scribus.getVariable(name_id) == "రవి, Kumar", "invalid record partially changed the document")
check(scribus.applyDataRecord(json_record, False) == 2, "non-strict JSON record failed")
check(scribus.getVariable("Customer") == "Maya", "JSON customer value failed")
check(scribus.getVariable("Address") == "42 Example Rd", "JSON address value failed")

expect_error(lambda: scribus.applyDataRecord({"Customer": "Wrong", "Address": 42}),
             "non-string value was accepted")
expect_error(lambda: scribus.applyDataRecord({"Customer": "Wrong", "page-count": "5"}, False),
             "computed variable was writable")
expect_error(lambda: scribus.applyDataRecord({"Customer": "Wrong", name_id: "Duplicate"}),
             "duplicate name and ID binding was accepted")
check(scribus.getVariable("Customer") == "Maya", "validation failure partially changed a value")
check(scribus.applyDataRecord({"Unused": "ignored"}, False) == 0, "unmatched record count was wrong")

scribus.saveDocAs(document_path)
scribus.closeDoc()
check(scribus.openDoc(document_path), "could not reopen the merge document")
check(scribus.getVariable("Customer") == "Maya", "merged value did not persist")
check(scribus.getVariable("Address") == "42 Example Rd", "second merged value did not persist")
scribus.closeDoc()

# The source reader itself must reject malformed shape and unsafe nested JSON,
# and its preview limit must be deterministic.
with open(csv_path, "w", encoding="utf-8", newline="") as source:
    source.write("Customer,Address\nOnly one field\n")
expect_error(lambda: scribus.loadDataSource(csv_path), "ragged CSV was accepted")
with open(csv_path, "w", encoding="utf-8", newline="") as source:
    source.write("Customer,Address\n\"Bad\"suffix,Ok\n")
expect_error(lambda: scribus.loadDataSource(csv_path), "malformed quoted CSV was accepted")
with open(json_path, "w", encoding="utf-8") as source:
    json.dump([{"Customer": "One", "Meta": {"nested": True}}, {"Customer": "Two"}], source)
expect_error(lambda: scribus.loadDataSource(json_path), "nested JSON was accepted")
with open(json_path, "w", encoding="utf-8") as source:
    json.dump([{"Customer": "One"}, {"Customer": "Two"}], source)
check(len(scribus.loadDataSource(json_path, "json", 1)) == 1, "JSON preview limit failed")
expect_error(lambda: scribus.loadDataSource(json_path, "xml"), "unsupported format was accepted")

# Exercise the shared dialog/Scripter batch exporter with two distinct records.
check(
    scribus.newDocument(
        scribus.PAPER_A4, (36, 36, 36, 36), scribus.PORTRAIT, 1,
        scribus.UNIT_POINTS, scribus.PAGE_1, 0, 1,
    ),
    "could not create the PDF merge test document",
)
pdf_variable = scribus.createVariable("Recipient", "Template Value")
pdf_frame = scribus.createText(40, 40, 400, 100, "PdfMergeText")
scribus.insertVariable(pdf_variable, pdf_frame)
with tempfile.TemporaryDirectory(prefix="scribus-data-merge-", dir=output_dir) as pdf_dir:
    batch_source = os.path.join(pdf_dir, "recipients.csv")
    with open(batch_source, "w", encoding="utf-8", newline="") as source:
        writer = csv.DictWriter(source, fieldnames=["Recipient"])
        writer.writeheader()
        writer.writerow({"Recipient": "Alpha Recipient"})
        writer.writerow({"Recipient": "Beta Recipient"})
    original_pdf_file = scribus.PDFfile().file
    files = scribus.exportDataMergePDFs(batch_source, pdf_dir)
    check(len(files) == 2, "batch export did not return two PDF paths")
    check(files[0].endswith("record-0001.pdf"), "batch output numbering is unstable")
    for pdf_path, recipient in zip(files, ("Alpha Recipient", "Beta Recipient")):
        check(os.path.getsize(pdf_path) > 100, "merged PDF was empty")
        with open(pdf_path, "rb") as result:
            check(result.read(5) == b"%PDF-", "merged export was not a PDF")
        if shutil.which("pdftotext"):
            rendered = subprocess.run(
                ["pdftotext", pdf_path, "-"], check=True,
                capture_output=True, text=True,
            ).stdout
            check(recipient in rendered, "merged value was not rendered in its PDF")
    check(scribus.getVariable(pdf_variable) == "Template Value",
          "template variable was not restored after export")
    check(scribus.PDFfile().file == original_pdf_file,
          "batch export changed the document's PDF filename setting")
    expect_error(lambda: scribus.exportDataMergePDFs(batch_source, pdf_dir),
                 "batch export overwrote existing PDFs")
    expect_error(lambda: scribus.exportDataMergePDFs(batch_source, pdf_dir, None, "../unsafe"),
                 "unsafe filename prefix was accepted")
    expect_error(lambda: scribus.exportDataMergePDFs(batch_source, os.path.join(pdf_dir, "missing")),
                 "missing output directory was accepted")
    ranged_files = scribus.exportDataMergePDFs(
        batch_source, pdf_dir, None, "ranged", 2, 2, "Recipient",
    )
    check(len(ranged_files) == 1 and
          os.path.basename(ranged_files[0]) == "ranged-record-0002-Beta Recipient.pdf",
          "record selection or field-based naming failed")
    expect_error(lambda: scribus.exportDataMergePDFs(batch_source, pdf_dir,
                                                      None, "invalid", 2, 1),
                 "reversed record range was accepted")
    expect_error(lambda: scribus.exportDataMergePDFs(batch_source, pdf_dir,
                                                      None, "invalid", 1, 2, "Missing Field"),
                 "missing filename field was accepted")

    mapped_source = os.path.join(pdf_dir, "mapped.json")
    with open(mapped_source, "w", encoding="utf-8") as source:
        json.dump([{"Full Name": "Mapped Recipient", "Unused": "ignored"}], source)
    mapped_files = scribus.exportDataMergePDFs(
        mapped_source, pdf_dir, {"Full Name": pdf_variable}, "mapped",
    )
    check(len(mapped_files) == 1 and os.path.basename(mapped_files[0]) == "mapped-record-0001.pdf",
          "explicit mapping or prefix failed")
    if shutil.which("pdftotext"):
        rendered = subprocess.run(
            ["pdftotext", mapped_files[0], "-"], check=True,
            capture_output=True, text=True,
        ).stdout
        check("Mapped Recipient" in rendered, "explicit mapping was not rendered")
    expect_error(lambda: scribus.exportDataMergePDFs(mapped_source, pdf_dir,
                                                      {"Full Name": "Missing Variable"}),
                 "invalid explicit variable mapping was accepted")
    expect_error(lambda: scribus.exportDataMergePDFs(mapped_source, pdf_dir,
                                                      {"Missing Source": pdf_variable}),
                 "mapping to a missing source field was accepted")
    expect_error(lambda: scribus.exportDataMergePDFs(mapped_source, pdf_dir,
                                                      {"Full Name": pdf_variable,
                                                       "Unused": pdf_variable}),
                 "two fields were mapped to the same variable")
    overflow_source = os.path.join(pdf_dir, "overflow.csv")
    with open(overflow_source, "w", encoding="utf-8", newline="") as source:
        writer = csv.DictWriter(source, fieldnames=["Recipient"])
        writer.writeheader()
        writer.writerow({"Recipient": "Long recipient " * 500})
    expect_error(lambda: scribus.exportDataMergePDFs(
        overflow_source, pdf_dir, None, "preflight", 1, 1, "", True),
        "strict mail-merge preflight accepted overflowing recipient text")
    check(not os.path.exists(os.path.join(pdf_dir, "preflight-record-0001.pdf")),
          "failed preflight left a record PDF")
    check(scribus.getVariable(pdf_variable) == "Template Value",
          "validation failure changed the template variable")
scribus.closeDoc()

print("DATA_MERGE_TEST_PASSED", flush=True)
