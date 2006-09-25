INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
boilerplate.py
Sample1.py
Calender.py
trait_de_coupe.py
pochette_cd.py
3columnUSLTR.py
3columnA4.py
legende.py
moins_10_pourcent_group.py
plus_10_pourcent_group.py
wordcount.py
quote.py
golden-mean.py
sample_db_usage.py
  DESTINATION ${SAMPLESDIR}
)