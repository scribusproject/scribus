#ifndef APPMODES_H
#define APPMODES_H

enum AppMode
{
	modeNormal,
	modeDrawShapes,
	modeStoryEditor,
	modeDrawImage,
	modeDrawText,
	modeMagnifier,
	modeEdit,
	modeDrawLine,
	modeRotation,
	modeLinkFrames,
	modeImportImage,
	modeUnlinkFrames,
	modeDrawRegularPolygon,
	modeDrawBezierLine,
	modeInsertPDFButton,
	modeInsertPDFTextfield,
	modeInsertPDFCheckbox,
	modeInsertPDFCombobox,
	modeInsertPDFListbox,
	modeInsertPDFTextAnnotation,
	modeInsertPDFLinkAnnotation,
	modeDrawFreehandLine,
	modeDrawTable,	// Now deprecated as new tables are working now.
	modeDrawTable2, // Temporary name for new tables.
	modePanning,
	modeMeasurementTool,
	modeEditGradientVectors,
	modeEyeDropper,
	modeCopyProperties,
	modeEditClip,
	modeDrawLatex,
	modeImportObject,
	modeInsertPDF3DAnnotation,
	modeEditMeshGradient,
	modeDrawCalligraphicLine,
	modeDrawArc,
	modeEditArc,
	modeEditPolygon,
	modeDrawSpiral,
	modeEditSpiral,
	modeEditTable,
	modeEditMeshPatch,
	modeEditWeldPoint,
	modeInsertPDFRadioButton
};

#endif // APPMODES_H
