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
#define FORMATID_ODGIMPORT 10
#define FORMATID_SXDIMPORT 11
#define FORMATID_SVGIMPORT 12
#define FORMATID_PSIMPORT  13
#define FORMATID_PDFIMPORT 20

#endif
