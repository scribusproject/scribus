include_directories(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
${FREETYPE_INCLUDE_DIRS}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_FPIC}")

set(SCRIBUS_RTF_LIB_SOURCES
	AbstractRtfOutput.cpp
	AuthorPcdataDestination.cpp
	CategoryPcdataDestination.cpp
	ColorTableDestination.cpp
	CommentPcdataDestination.cpp
	CompanyPcdataDestination.cpp
	controlword.cpp
	Destination.cpp
	DocumentCommentPcdataDestination.cpp
	DocumentDestination.cpp
	FontTableDestination.cpp
	GeneratorPcdataDestination.cpp
	HLinkBasePcdataDestination.cpp
	IgnoredDestination.cpp
	InfoCreatedTimeDestination.cpp
	InfoDestination.cpp
	InfoPrintedTimeDestination.cpp
	InfoRevisedTimeDestination.cpp
	InfoTimeDestination.cpp
	KeywordsPcdataDestination.cpp
	ManagerPcdataDestination.cpp
	OperatorPcdataDestination.cpp
	PcdataDestination.cpp
	PictDestination.cpp
	rtfreader.cpp
	sladocumentrtfoutput.cpp
	StyleSheetDestination.cpp
	SubjectPcdataDestination.cpp
	TitlePcdataDestination.cpp
	Token.cpp
	Tokenizer.cpp
	UserPropsDestination.cpp
)

set(SCRIBUS_RTF_LIB_MOC_CLASSES
	rtfreader.h
)


QT5_WRAP_CPP(SCRIBUS_RTF_LIB_MOC_SOURCES ${SCRIBUS_RTF_LIB_MOC_CLASSES})

set(SCRIBUS_RTF_LIB "scribus_rtf_lib")
add_library(${SCRIBUS_RTF_LIB} STATIC ${SCRIBUS_RTF_LIB_SOURCES} ${SCRIBUS_RTF_LIB_MOC_SOURCES})
set_target_properties(${SCRIBUS_ZIB_LIB}
  PROPERTIES
  COMPILE_FLAGS -DCOMPILE_SCRIBUS_MAIN_APP
  )
