# Apscribe fork changes and status

This is a public inventory of work in the [Apscribe development branch](https://github.com/appajid/apscribe/tree/codex/apscribe-granular-2026-10-03) as of 3 October 2026. It describes code and tests in that branch, **not** a release or a promise that every document round-trips perfectly. The fork's `master` branch is separate and does not yet contain all of this work. The [commit history](https://github.com/appajid/apscribe/commits/codex/apscribe-granular-2026-10-03/) records each implementation slice and correction.

## Implemented in the development branch

| Area | Added or changed | Scope to keep in mind |
| --- | --- | --- |
| Workspace and artwork | Compact dockable Tools palette, related-tool flyouts, original editable SVG tool icons, context-sensitive controls, grouped Type/Paragraph/style sections, refreshed Welcome, New Document, splash, About, Story Editor, path/node editor, table inspector, image effects, asset manager, Quick Actions, and Find/Replace. | This is an evolving interface, not a claim that every screen has been redesigned. Light/dark appearance has dedicated artwork and theme support; visual QA remains useful on each OS. |
| Long documents | Document and user-defined variables, SLA persistence and scripting, running headers based on styles, facing master-page pair creation, live page/text cross-references with preflight and navigation. | Layout changes can affect pagination; validate long documents after import and export. |
| Styles and layout | Quick Apply, document object styles with import/undo/scripting, anchored object layout and text wrap, improved direct text selection and anchored-media editing. | Anchored image/table parity with every free-standing editing operation is still a quality target. |
| Production assets | Missing-image discovery and relinking, image-link status and replacement, embedded-image extraction, document font replacement, ICC-based RGB swatch conversion, non-destructive CMYK image export, and editable contours from image alpha. | Check output colour and links in the target workflow. |
| Data publishing | CSV/JSON variable mapping and preview, PDF-per-record mail merge, headless job manifests, and a generated card-grid catalogue with optional editable SLA. | Combined mail-merge PDF and arbitrary template-driven catalogue layouts are not implemented. |
| Import | PageMaker `.p65`/`.pm7` file selection and import, and multiple IDML text, image-link, table-cell, crop, leading, facing-page, editability, and SLA round-trip corrections. | Import is best-effort; inspect typography, links, and page appearance against the source. IDML is **not** INDD. |

## Active, limited tracks

**Native INDD:** the separate INDD code performs bounded, read-only format inspection and diagnostic comparisons. It can inspect certain headers, metadata, saved thumbnails, and opaque database structures. It does **not** open or convert native INDD content into editable pages. Do not treat a successful probe as import support. The [INDD research notes](https://github.com/appajid/apscribe/blob/codex/apscribe-granular-2026-10-03/doc/phase6-indd-probe.md) describe the evidence and gaps.

**EPUB:** File > Export > Export Limited EPUB and Scripter can generate a reflowable EPUB 3.3 package from supported, explicitly ordered text stories and linked PNG/JPEG image frames. The work includes semantic style mapping, selected list forms, headings/navigation, image descriptions or decorative flags, captions, optional width and a restricted pixel-exact PNG crop, plus a structured preflight. Unsupported content blocks export rather than disappearing silently. Inline images/tables, arbitrary page geometry and typography, full image effects/colour fidelity, and fixed-layout EPUB are **not** complete. The [EPUB implementation notes](https://github.com/appajid/apscribe/blob/codex/apscribe-granular-2026-10-03/doc/phase7-epub.md) list each slice and its limits.

## Verification and provenance

The branch contains focused C++ and application-level tests, plus cross-platform CI definitions. A local macOS rebuild and installed-bundle EPUB, IDML, and PageMaker smoke tests passed after the Apscribe bundle change. The Windows native-package workflow and the portable cross-platform checks passed on earlier branch commits; consult the [current Actions results](https://github.com/appajid/apscribe/actions) before relying on a newer commit. Linux runtime validation of the recent C++17 INDD fix was still running when this inventory was written. These checks exercise representative paths, not every import format or document.

Further implementation details are in the [data-publishing notes](https://github.com/appajid/apscribe/blob/codex/apscribe-granular-2026-10-03/doc/phase5-data-publish.md), [PageMaker QA notes](https://github.com/appajid/apscribe/blob/codex/apscribe-granular-2026-10-03/doc/phase6-pagemaker-qa.md), and [Phase 3 portability notes](https://github.com/appajid/apscribe/blob/codex/apscribe-granular-2026-10-03/ci/PHASE_3_VALIDATION.md). No proprietary sample documents are included.

## Origin and license

Apscribe builds on Scribus. Original Scribus copyright, license, and attribution notices remain in the source. Redistribution of this GPL-licensed fork must follow [COPYING](COPYING), including the applicable source-availability obligations. Apscribe is a working brand for this fork; it is not an official Scribus product.
