#ifndef FORMATIDLIST_H
#define FORMATIDLIST_H

/**
 * \brief The IDs for the load/save format plugins. They must be a macro not a static const member
 * because they must be available even when the specifid plugin is not linked in
 * or even compiled.
 */

// IMPORT

#define FORMATID_SLA134IMPORT 48
#define FORMATID_SLA13XIMPORT 49
#define FORMATID_SLA12XIMPORT 50
#define FORMATID_NATIVEIMPORTEND 99

#define FORMATID_ODGIMPORT 100
#define FORMATID_SXDIMPORT 101
#define FORMATID_SVGIMPORT 102
#define FORMATID_PSIMPORT  103
#define FORMATID_WMFIMPORT 104
#define FORMATID_AIIMPORT  105
#define FORMATID_XFIGIMPORT  106
#define FORMATID_PDFIMPORT 120

// EXPORT

#define FORMATID_SLA134EXPORT 48
#define FORMATID_SLA13XEXPORT 49
#define FORMATID_SLA12XEXPORT 50
#define FORMATID_NATIVEEXPORTEND 99

#define FORMATID_ODGEXPORT 100
#define FORMATID_SXDEXPORT 101
#define FORMATID_SVGEXPORT 102
#define FORMATID_PSEXPORT  103
#define FORMATID_PDFEXPORT 120

#endif
