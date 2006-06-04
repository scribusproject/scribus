#ifndef FORMATIDLIST_H
#define FORMATIDLIST_H

/**
 * \brief The IDs for the load/save format plugins. They must be a macro not a static const member
 * because they must be available even when the specifid plugin is not linked in
 * or even compiled.
 */

#define FORMATID_SLA12XIMPORT 0
#define FORMATID_SLA13XIMPORT 1
#define FORMATID_SLA134IMPORT 2
#define FORMATID_NATIVEIMPORTEND 99
#define FORMATID_ODGIMPORT 100
#define FORMATID_SXDIMPORT 101
#define FORMATID_SVGIMPORT 102
#define FORMATID_PSIMPORT  103
#define FORMATID_PDFIMPORT 120

#define FORMATID_SLA12XEXPORT 0
#define FORMATID_SLA13XEXPORT 1
#define FORMATID_SLA134EXPORT 2
#define FORMATID_NATIVEEXPORTEND 99
#define FORMATID_ODGEXPORT 100
#define FORMATID_SXDEXPORT 101
#define FORMATID_SVGEXPORT 102
#define FORMATID_PSEXPORT  103
#define FORMATID_PDFEXPORT 120

#endif
