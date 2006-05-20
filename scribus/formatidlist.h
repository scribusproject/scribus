#ifndef FORMATIDLIST_H
#define FORMATIDLIST_H

/**
 * \brief The IDs for the load/save format plugins. They must be a macro not a static const member
 * because they must be available even when the specifid plugin is not linked in
 * or even compiled.
 */

#define FORMATID_ODGIMPORT 1
#define FORMATID_SXDIMPORT 2
#define FORMATID_SVGIMPORT 3
#define FORMATID_PSIMPORT  4
#define FORMATID_PDFIMPORT 9

#endif