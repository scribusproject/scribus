# Phase 6: PageMaker import QA

The `pagemaker_import` integration test always checks that malformed `.p65` and
`.pm7` files are rejected without leaving a document open. Local, proprietary
documents are optional inputs and are not stored in the repository.

To run the opt-in round-trip test, set either or both environment variables
before invoking CTest:

```sh
SCRIBUS_PAGEMAKER_TEST_P65=/path/to/sample.p65 \
SCRIBUS_PAGEMAKER_TEST_PM7=/path/to/sample.pm7 \
ctest --test-dir build-qt-6.11.2 -R '^pagemaker_import$' --output-on-failure
```

The earlier `SCRIBUS_PAGEMAKER_TEST_FILE` variable also remains supported for
a single local sample.

For each supplied file, the test imports it, counts pages, objects by type,
and editable text characters, saves a temporary SLA, reopens it, and checks
that those counts are unchanged. The temporary SLA is removed afterward.

## Current evidence

| Format | Local sample | Result |
| --- | --- | --- |
| `.p65` | 3 pages; 3 objects; 15,729 editable text characters | Import and SLA round-trip passed on macOS ARM |
| `.pm7` | 8 pages; 30 objects; 45,162 editable text characters | Import and SLA round-trip passed on macOS ARM |

These are regression fixtures, not a claim of complete PageMaker compatibility.
Image/link fidelity, styles, colours, masters, visual layout, other PageMaker
versions, and Windows/Linux runtime behaviour still need independent checks.
The samples must not be added to source control or CI artifacts.
