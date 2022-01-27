# it's unfinished add_subdirectory(smwidgets)

include_directories(
${FREETYPE_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

set(SCRIBUS_SCRSPINBOX_PLUGIN_MOC_CLASSES
scrspinboxplugin.h
../commonstrings.h
../ui/scrspinbox.h
)

set(SCRIBUS_SCRSPINBOX_PLUGIN_SOURCES
scrspinboxplugin.cpp
../commonstrings.cpp
../fparser.cpp
../ui/scrspinbox.cpp
../units.cpp
)

set(SCRIBUS_SCTABLEWIDGET_PLUGIN_MOC_CLASSES
sctablewidgetplugin.h
../ui/sctablewidget.h
)

set(SCRIBUS_SCTABLEWIDGET_PLUGIN_SOURCES
sctablewidgetplugin.cpp
../ui/sctablewidget.cpp
)

set(SCRIBUS_SCTEXTBROWSER_PLUGIN_MOC_CLASSES
sctextbrowserplugin.h
../ui/sctextbrowser.h
../urllauncher.h
)

set(SCRIBUS_SCTEXTBROWSER_PLUGIN_SOURCES
sctextbrowserplugin.cpp
../ui/sctextbrowser.cpp
../urllauncher.cpp
)

set(SCRIBUS_SCRSPINBOX_PLUGIN "scribus_scrspinbox")
set(SCRIBUS_SCTABLEWIDGET_PLUGIN "scribus_sctablewidget")
set(SCRIBUS_SCTEXTBROWSER_PLUGIN "scribus_sctextbrowser")

QT5_WRAP_CPP(SCRIBUS_SCRSPINBOX_PLUGIN_MOC_SOURCES ${SCRIBUS_SCRSPINBOX_PLUGIN_MOC_CLASSES})
QT5_WRAP_CPP(SCRIBUS_SCTABLEWIDGET_PLUGIN_MOC_SOURCES ${SCRIBUS_SCTABLEWIDGET_PLUGIN_MOC_CLASSES})
QT5_WRAP_CPP(SCRIBUS_SCTEXTBROWSER_PLUGIN_MOC_SOURCES ${SCRIBUS_SCTEXTBROWSER_PLUGIN_MOC_CLASSES})

add_library(${SCRIBUS_SCRSPINBOX_PLUGIN} SHARED ${SCRIBUS_SCRSPINBOX_PLUGIN_SOURCES} ${SCRIBUS_SCRSPINBOX_PLUGIN_MOC_SOURCES})
target_link_libraries(${SCRIBUS_SCRSPINBOX_PLUGIN}
  ${QT_LIBRARIES}
)

add_library(${SCRIBUS_SCTABLEWIDGET_PLUGIN} SHARED ${SCRIBUS_SCTABLEWIDGET_PLUGIN_SOURCES} ${SCRIBUS_SCTABLEWIDGET_PLUGIN_MOC_SOURCES})
target_link_libraries(${SCRIBUS_SCTABLEWIDGET_PLUGIN}
  ${QT_LIBRARIES}
)

add_library(${SCRIBUS_SCTEXTBROWSER_PLUGIN} SHARED ${SCRIBUS_SCTEXTBROWSER_PLUGIN_SOURCES} ${SCRIBUS_SCTEXTBROWSER_PLUGIN_MOC_SOURCES})
target_link_libraries(${SCRIBUS_SCTEXTBROWSER_PLUGIN}
  ${QT_LIBRARIES}
)

if(APPLE)
  target_link_libraries(${SCRIBUS_SCRSPINBOX_PLUGIN} "-undefined dynamic_lookup")
  target_link_libraries(${SCRIBUS_SCTABLEWIDGET_PLUGIN} "-undefined dynamic_lookup")
  target_link_libraries(${SCRIBUS_SCTEXTBROWSER_PLUGIN} "-undefined dynamic_lookup")
endif()

# install it manually
#install(TARGETS ${SCRIBUS_SCRSPINBOX_PLUGIN}
#  LIBRARY
#  DESTINATION ${QT_INSTALL_PLUGINS}
#  PERMISSIONS ${PLUGIN_PERMISSIONS}
#)

# set_target_properties(${SCRIBUS_DESIGNR_PLUGIN} PROPERTIES VERSION "0.0.0")
