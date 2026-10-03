# Apscribe

Apscribe is a work-in-progress desktop-publishing fork of [Scribus](https://www.scribus.net/). It retains Scribus's open-source foundation while developing a revised workspace and additional long-document, production, import, and publishing tools. Apscribe is the **working name**, not a claim of affiliation with the Scribus project or a finished release. The application currently reports version 2.0.0; that number does not imply feature-complete or production-ready status.

**Where the changes are:** the [Apscribe development branch](https://github.com/appajid/scribus/tree/codex/apscribe-granular-2026-10-03) contains the new code and granular commits. The fork's `master` branch does **not** yet contain all of these features. The [feature inventory](FORK_CHANGES.md) separates implemented work from limited or experimental work and points to the relevant tests. Consult the [development branch's commit history](https://github.com/appajid/scribus/commits/codex/apscribe-granular-2026-10-03/) for individual changes.

## Highlights in the development branch

- A reorganized workspace with a compact tool rail, grouped tool flyouts, original SVG tool artwork, selection-sensitive inspector sections, refreshed dialogs, and light/dark appearance work.
- Document variables, running headers, facing master-page pairs, cross-references, anchored objects, Quick Apply, and object styles.
- Image-link repair, asset information, embedded-image extraction, font replacement, colour conversion, and image contour tools.
- CSV/JSON data merge, mail-merge PDFs, and a limited catalogue publisher.
- Improved PageMaker `.p65`/`.pm7` and IDML import paths. **Native INDD import is not available**; the current INDD work is read-only format research.
- A **limited reflowable EPUB** export path for supported text and linked images. It has explicit reading order and preflight but does not yet preserve every document feature or offer fixed-layout EPUB.

See [what works and what remains](FORK_CHANGES.md) before relying on a feature for a production document. Automated checks cover parts of the macOS, Linux, and Windows builds; they are not a guarantee of identical behavior across all platforms or documents.

## Build and contribute

Build instructions are in [BUILDING](BUILDING), [README.MacOSX](README.MacOSX), and [BUILDING_win32_cmake.txt](BUILDING_win32_cmake.txt). Use the development branch to try fork features. Keep source files and license notices when distributing modified builds; see [COPYING](COPYING). The executable and `.sla` document format still use some Scribus identifiers while branding and compatibility work continues.

For issues about these fork changes, use this repository's [issue tracker](https://github.com/appajid/scribus/issues). For upstream Scribus issues, use the [Scribus project](https://www.scribus.net/). This fork is independently developed and is not an official Scribus release.
