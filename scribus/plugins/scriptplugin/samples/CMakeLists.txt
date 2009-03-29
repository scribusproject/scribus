INCLUDE_DIRECTORIES(
"${CMAKE_SOURCE_DIR}/scribus"
)

INSTALL(FILES
3columnUSLTR.py
3columnA4.py
boilerplate.py
Calender.py
ExtractText.py
golden-mean.py
legende.py
moins_10_pourcent_group.py
plus_10_pourcent_group.py
pochette_cd.py
quote.py
sample_db_usage.py
Sample1.py
trait_de_coupe.py
wordcount.py
  DESTINATION ${SAMPLESDIR}
)
