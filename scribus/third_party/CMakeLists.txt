if(HAVE_OSG)
  add_subdirectory(prc)
endif ()

if(HAVE_BOOST)
  add_subdirectory(lib2geom)
endif()

add_subdirectory(wpg)
add_subdirectory(pgf)
add_subdirectory(rtf-qt)
add_subdirectory(zip)
