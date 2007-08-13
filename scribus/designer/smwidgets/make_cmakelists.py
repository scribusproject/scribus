src = [
'smalignselectplugin',
'smcolorcomboplugin',
'smfontcombohplugin',
'smsccomboboxplugin',
'smscrspinboxplugin',
'smshadebuttonplugin',
'smspinboxplugin',
'smstyleselectplugin',
'smtabrulerplugin'
]


out = [
'''INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)''']

template = '''
SET(SCRIBUS_%s_PLUGIN_MOC_CLASSES
%s.h
../../smwidgets.h
)

SET(SCRIBUS_%s_PLUGIN_SOURCES
%s.cpp
../../smwidgets.cpp
)
'''

for i in src:
	up = i.upper()
	out.append(template % (up, i, up, i))

template = '\nSET(SCRIBUS_%s_PLUGIN "scribus_%s")'
for i in src:
	out.append(template % (i.upper(), i))

template = '\nQT4_WRAP_CPP(SCRIBUS_%s_PLUGIN_MOC_SOURCES ${SCRIBUS_%s_PLUGIN_MOC_CLASSES})'
for i in src:
	out.append(template % (i.upper(), i.upper()))
	
template = '''
ADD_LIBRARY(${SCRIBUS_%s_PLUGIN}
	SHARED ${SCRIBUS_%s_PLUGIN_SOURCES}
	${SCRIBUS_%s_PLUGIN_MOC_SOURCES})
TARGET_LINK_LIBRARIES(${SCRIBUS_%s_PLUGIN} ${QT_LIBRARIES} )
'''
for i in src:
	out.append(template % (i.upper(), i.upper(), i.upper(), i.upper()))

out.append('\nIF(APPLE)')
template = '''
  TARGET_LINK_LIBRARIES(${SCRIBUS_%s_PLUGIN} "-undefined dynamic_lookup")
'''
for i in src:
	out.append(template % i.upper())
out.append('ENDIF(APPLE)')

print ''.join(out)
