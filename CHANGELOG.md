What’s New in Scribus 1.5.0?
============================

This is a cursory overview over the most important changes in Scribus compared to the latest stable version 1.4.x.

Please note that the file format has changed and is not backwardly compatible with any prior stable version (1.4.x or earlier). To be precise, you will not be able to open 1.5 files with version 1.4.x or any earlier version.

Also note that the 1.5.x series is not an officially stable version. Instead, all 1.5.x versions are technical previews of the upcoming stable Scribus 1.6.0! If you try to do some serious work with 1.5.x, you do so at your own peril.

## General

- Qt 5: As of version 1.5.0, Scribus is being built using version 5 of the Qt toolkit.

- More dependencies: Scribus 1.5.x makes extensive use of many more external libraries than earlier versions, especially for import filters. In most cases Scribus needs to be explicitly compiled against these libraries, so they become dependencies. In other cases (e.g. UniConvertor), Scribus will detect an installed version automatically. Note that full support for some libraries may not be available on Mac OS X and Windows until the final 1.6.0 release.

## Text and Typography

### Typography

- Vertical alignment of text.

- Orphans and widows control.

### Text

- Foot- and endnotes.

- Freely defineable text variables.

- Text references.

- Anchored text frames.

- Automatic numbering and bullets.

- Automatic creation of text frame links.

- Vertical alignment of text. This has been one of the most-requested features with respect to Scribus’s text features. Text in a frame can now be aligned to the top, the center and the bottom.

- Auto-adjustment of text frames to text size.

- Inline Objects in text frames are finally fully editable.

## Tables

The tables tool has been completely rewritten. Tables in Scribus work now more like those in other DTP and wordprocessing software. Basic table and cell styles have also been implemented.

## Graphics

### Picture Browser

A new feature-rich plug-in called "Picture Browser" enables asset management for graphics files by tagging them or creating graphics collections. Graphics files can be dragged into a Scribus document from the Picture Browser.

### Vector Tools

- New vector types: Arcs and Spirals.

- Calligraphic pen.

- New path tools: Flattening and smoothing of paths.

- One of the most requested features – drop shadows – has been added and works in all export formats.

### Raster Images

- Image cache: To prevent Scribus from slowing down with many high resolution images being loaded into a document, an image cache for raster images has been added.

- Embedded images: Raster images can now be embedded in Scribus files. Beware, though, that the resulting SLA or even SLA.GZ files can be huge.

### Frame Fills and Lines

- Enhanced resource management: Solid colors, gradients and patterns can be managed in a single dialog ("Colors & Fills").

- More supported color palette formats: Scribus can now read and use swatches from Adobe Photoshop (ACO) and AutoCAD (ACB).

- More gradient fills: Scribus can import GIMP gradients via the "Colors & Fills" dialog. Moreover, the number of available gradient options in Scribus has grown significantly and includes, among others, mesh gradients.

- Hatchings: As of this version, Scribus supports real vector hatchings with various options.

- New frame stroke and line options: All types of lines, including frame strokes, can now use a gradient or a pattern instead of only a color.

- Advanced transparency effects: Transparency effects that were previously only available for multi-layer Photoshop files in an image frame, as well as additional effects are now available for all frames and layers via the new "Transparency" tab in the Properties Palette.

## Import Filters

A considerable amount of work has been spent on new import filters, as well as improving existing filters.

### Improved Import Filters

- Adobe Illustrator: The Adobe Illustrator importer has been compelety rewritten. In hybrid AI files it’s now possible to select either the PDF or the AI part of the file.

- OpenDocument Graphics: The ODG importer has been completely rewritten and now also supports OpenDocument Presentation files (ODP).

- PDF: The PDF importer has been completely rewritten and now opens most PDF files with the layout 100 % intact. PDF forms elements are being recognized as such and can be edited in Scribus.

- PostScript: The PostScript importer has been completely rewritten and now opens most PS files with the layout 100 % intact.

### New Import Filters

- Adobe PageMaker: Scribus 1.5 includes an early version of a filter for files created in versions 6.5 and 7.0 of the once popular DTP program. It uses libpagemaker.

- Computer Graphics Metafile (CGM): CGM is a 2D vector format standard that has been around for a long time and is still being developed, especially for technical drawings. The new filter is supposed to import most CGM files correctly.

- CorelDraw (CDR, CDT, CMX): Thanks to the tireless work on libcdr, Scribus can now import files created by CorelDraw from version 1 to the present. Moreover, adding support for libcdr not only allows for importing CDR files, but also CorelDraw Templates (CDT) and CorelDraw Presentations Exchange (CMX) files.

- Enhanced Metafile (EMF): EMF files are 32-bit versions of the 16-bit Windows Metafile Format (WMF), which has already been supported by Scribus 1.4.x.

- GraphicsMagick Plug-in: Almost all bitmap formats supported by GraphicsMagick can now be imported into Scribus. Note, though, that to save space in the drop-down menu of the file selectors, the many formats aren’t listed separately.

- InDesign/InCopy: While the reverse-engineering efforts regarding InDesign’s binary format (INDD) by third-party developers haven’t reached the stage of producing anything useful yet, Adobe has released a complete specification of the XML version of that format (IDML). Scribus 1.5 is being shipped with a basic import filter for this format, which will be improved and refined over time. It will also import InDesign XML Snippets (IDMS) and some InCopy XML files (ICML).

- iWorks Pages (PAGES): Pages is Apple’s word processing software, which also provides many DTP features. A native importfilter has been developed to import files created with Pages ’08 and later versions.

- Micrografx Draw (DRW): This a file format of the once popular Micrografx Draw graphics software for Windows 3 and 95. Since many vector collections using this format are still around, an import filter has been written.

- Microsoft Publisher (PUB): An importer for PUB files has been made possible by the creation of libmspub, wich is also used for Publisher import into LibreOffice. It is supposed to import all Publisher versions.

- Microsoft Visio (VSD, VXD): An importer for Visio files has been made possible by the creation of libvisio, which is also used for Visio import into LibreOffice. It is supposed to import at least Visio versions from 2000–2013.

- Open Raster File (ORA): Open Raster is an open standard for complex bitmap files. It’s supported by all major Free image editors, including GIMP and Krita.

- Photoshop Custom Shapes (CSH): CSH is a vector format used for simple vector graphics collections in Adobe Photoshop. They can be imported via the new "Custom Shapes" manager.

- Progressive Graphics File (PGF): This is a fairly new image format, designed to replace JPEG, just like JPEG 2000 does, but emphasizing speed and quality over compression ratio.

- StarView Metafile (SVM): This is an old Metafile format (similar to WMF) that was used by OpenOffice’s / LibreOffice’s predecessor StarOffice. Since both successors still use SVM in OpenDocument Graphics, the SVM import filter was a by-product of the new ODG filter.

- UniConvertor Plug-in: As with the GraphicsMagick plug-in, Scribus won’t list any of the file types supported by UniConvertor to save space in file selectors. It will also ignore UniConvertor for importing file formats for which Scribus provides generic importers.

- WordPerfect Graphics (WPG): Scribus uses an improved version of libwpg to import the clipart format of WordPerfect Office.
VIVA Designer XML (XML): Like InDesign, VIVA Designer allows for exporting XML versions of its native format. These can now be imported into Scribus.

- Xara Designer (XAR): Xara Designer (formerly known as Xara Xtreme) is known as one of the most powerful vector drawing programs. Its file format (XAR) is well-documented, and thanks to many contributions from professional Xara users, the Scribus Team has been able to create an excellent XAR filter, whose implementation resulted in the addition of many new features in Scribus.

- XML Paper Specification (XPS, OXPS): XML is an alternative to PDF originally developed by Microsoft and now maintained and updated by ECMA International. Scribus 1.5 can import both the original Microsoft version (XPS) and the modified versions as standardized by ECMA (OXPS).

- XPress Tags (XTG): XTG is a format created by Quark to exchange text layouts between documents. Scribus 1.5 provides a basic import filter for XTG files created by Quark XPress, which will be improved and refined over time. Note that XPress Tags is not identical to Xtags, which is an enhanced version of XPress Tags provided by a third party.

## PDF Features

- Support for PDF/X-4: PDF/X-4 is a significant improvement over PDF/X-3 in that it permits transparencies.

- Substantial improvements to PDF forms: The PDF forms code has been completely rewritten, and Scribus now supports all features of PDF forms.

- Support for PDF 3D features, using OpenSceneGraph: Recent versions of the PDF specification allow for 3D objects to be stored in PDF files. Scribus 1.5 introduces a new PDF element called "3D Annotation". It enables the import of 3D objects in many formats and offer limited editing features, such as display mode or lighting.

## Other Export Features

XPS: A basic XPS exporter has been added.

## Usability/User Interface

- Docked palettes: All palettes can now be docked to the right or left side of the screen space.

- New Properties Palette: The PP has had a lot of functionality added over time due to other improvements. We will be looking to simplify it in coming 1.5.x releases.

- Context Menu: The context menu has been redesigned and is now much smaller.

- The new context menu has shrunk a lot.

- Document Setup/Preferences: Both settings dialogs have been rewritten from scratch and now offer many more options.

- File preview: The preview in file selectors works for all types of output, DTP, vector and bitmap files.

- Tabbed view mode: If more than one document is open, users can choose a tabbed view mode for easier access to a particular document.

- Symbols: Scribus 1.5 introduces a great time saver called Symbols. Every object can be converted to a Symbol or "master item". All copies or "child items" of this object will inherit every change made to the Symbol.

- A new snap mode: Items can now be snapped to other items.

- "Weld" feature for objects: Move objects together without grouping them.

- Snap mode: The “snap” mode for guides is now active by default.

- Layers: Items can now be selected across layers.

- Document update: It's now possible to update the display of the whole document via a single menu entry.

- Document Outline: Items can now be moved (up and down, in and out of groups) in the Document Outline.

- Undo/Redo: The Action History has been completely rewritten and now works for all actions.

## Help System

For the 1.5/1.6 series, the online manual now uses a CSS file to control and enhance the appearance of the text. The file is called manual.css, and like many aspects of Scribus can be modified to your liking. If available, the file will choose the typeface Fontin for various headings. This is a freely available font which can easily be added to your system.

Note that the Online Manual will probably not be updated until the release of the stable 1.6.x series.

## Known Issues in 1.5

- Properties Palette functionality has grown significantly and needs to be split into separate palettes later in the 1.5.x series.

- CTL (Complex Text Layout) like RTL/CJK is not supported yet (but is under development).

- Unable to add diacritic marks in Text Editor or directly into a text box (depending on OS/distro).

- Undo/Redo functionality may not be reliable for certain actions yet.

- Certain scripts that come bundled with Scribus do not work.

- Trackpads may cause strange mouse behavior on scribus canvas.
