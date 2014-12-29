INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/scribus
)

# Gentoo has an option for no templates. Let's make the ebuild simpler and open up the option to others, but leave as a default for us. Make the directo$
IF (WANT_NOTEMPLATES)
  INSTALL(CODE "FILE(MAKE_DIRECTORY ${ENV}${CMAKE_INSTALL_PREFIX}/${TEMPLATEDIR})")
ELSE (WANT_NOTEMPLATES)
	INSTALL(FILES
	template.xml
	  DESTINATION ${TEMPLATEDIR}
	)

	INSTALL(FILES
	template.ca.xml
	  DESTINATION ${TEMPLATEDIR}
	)
	
	INSTALL(FILES
	template.de.xml
	  DESTINATION ${TEMPLATEDIR}
	)

	INSTALL(FILES
	template.es.xml
	  DESTINATION ${TEMPLATEDIR}
	)

	INSTALL(FILES
	template.hu.xml
	  DESTINATION ${TEMPLATEDIR}
	)

	INSTALL(FILES
	template.it.xml
	  DESTINATION ${TEMPLATEDIR}
	)

	INSTALL(FILES
	br1/br1.png
	br1/br1tn.png
	br1/map.jpg
	br1/br1.sla.gz
	  DESTINATION ${TEMPLATEDIR}br1
	)

	INSTALL(FILES
	nl1/kites.jpg
	nl1/nl1.png
	nl1/nl1tn.png
	nl1/nl1.sla.gz
	  DESTINATION ${TEMPLATEDIR}nl1
	)

	INSTALL(FILES
	nl2/flowers.jpg
	nl2/nl2.png
	nl2/nl2tn.png
	nl2/nl2.sla.gz
	  DESTINATION ${TEMPLATEDIR}nl2
	)

	INSTALL(FILES
	textbased/an_image.jpg
	textbased/author.jpg
	textbased/textbased.png
	textbased/textbased.sla.gz
	textbased/textbasedtn.png
	  DESTINATION ${TEMPLATEDIR}textbased
	)

	INSTALL(FILES
	pres_backgr_1/blue_orange_vector.sla.gz
	pres_backgr_1/blue_orange_vectortn.png
	pres_backgr_1/blue_orange_vector.png
	  DESTINATION ${TEMPLATEDIR}pres_backgr_1
	)

	INSTALL(FILES
	pres_backgr_2/flood_light.sla.gz
	pres_backgr_2/flood_lighttn.png
	pres_backgr_2/flood_light.png
	  DESTINATION ${TEMPLATEDIR}pres_backgr_2
	)

	INSTALL(FILES
	pres_backgr_3/glass_ripples.sla.gz
	pres_backgr_3/glass_ripplestn.png
	pres_backgr_3/glass_ripples.png
	  DESTINATION ${TEMPLATEDIR}pres_backgr_3
	)

	INSTALL(FILES
	pres_backgr_4/bokeh.sla.gz
	pres_backgr_4/Bokehtn.png
	pres_backgr_4/Bokeh.png
	  DESTINATION ${TEMPLATEDIR}pres_backgr_4
	)

	INSTALL(FILES
	cc/Cheat_Cube.png
	cc/Cheat_Cubetn.png
	cc/Cheat_Cube.sla.gz
	  DESTINATION ${TEMPLATEDIR}cc
	)

	INSTALL(FILES
	mc/menu_template1.png
	mc/menu_template1tn.png
	mc/menu_template1.sla.gz
	  DESTINATION ${TEMPLATEDIR}mc
	)

	INSTALL(FILES
	cover1/cover1.png
	cover1/cover1tn.png
	cover1/cover1.sla.gz
	  DESTINATION ${TEMPLATEDIR}cover1
	)

	INSTALL(FILES
	cover2/cover2.png
	cover2/cover2tn.png
	cover2/cover2.sla.gz
	  DESTINATION ${TEMPLATEDIR}cover2
	)

	INSTALL(FILES
	cover3/cover3.png
	cover3/cover3tn.png
	cover3/cover3.sla.gz
	  DESTINATION ${TEMPLATEDIR}cover3
	)

	INSTALL(FILES
	cover4/cover4.png
	cover4/cover4tn.png
	cover4/cover4.sla.gz
	  DESTINATION ${TEMPLATEDIR}cover4
	)

	INSTALL(FILES
	cover5/cover5.png
	cover5/cover5tn.png
	cover5/cover5.sla.gz
	  DESTINATION ${TEMPLATEDIR}cover5
	)

	INSTALL(FILES
	buscard1/Business_Card_1.png
	buscard1/Business_Card_1tn.png
	buscard1/buscard1.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard1
	)

	INSTALL(FILES
	buscard2/Business_Card_2.png
	buscard2/Business_Card_2tn.png
	buscard2/buscard2.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard2
	)

	INSTALL(FILES
	buscard3/Business_Card_3.png
	buscard3/Business_Card_3tn.png
	buscard3/buscard3.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard3
	)

	INSTALL(FILES
	buscard4/Business_Card_4.png
	buscard4/Business_Card_4tn.png
	buscard4/buscard4.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard4
	)

	INSTALL(FILES
	buscard5/Business_Card_5.png
	buscard5/Business_Card_5tn.png
	buscard5/buscard5.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard5
	)

	INSTALL(FILES
	buscard6/Business_Card_6.png
	buscard6/Business_Card_6tn.png
	buscard6/buscard6.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard6
	)

	INSTALL(FILES
	buscard7/Business_Card_7.png
	buscard7/Business_Card_7tn.png
	buscard7/buscard7.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard7
	)

	INSTALL(FILES
	buscard8/Business_Card_8.png
	buscard8/Business_Card_8tn.png
	buscard8/buscard8.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard8
	)

	INSTALL(FILES
	buscard9/Business_Card_9.png
	buscard9/Business_Card_9tn.png
	buscard9/buscard9.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard9
	)

	INSTALL(FILES
	buscard10/Business_Card_10.png
	buscard10/Business_Card_10tn.png
	buscard10/buscard10.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard10
	)

	INSTALL(FILES
	buscard11/Business_Card_11.png
	buscard11/Business_Card_11tn.png
	buscard11/buscard11.sla.gz
	  DESTINATION ${TEMPLATEDIR}buscard11
	)

	INSTALL(FILES
	grid_br_a4_l/Brochure_Grid_A4_L.sla.gz
	grid_br_a4_l/Brochure_Grid_A4_Ltn.png
	grid_br_a4_l/Brochure_Grid_A4_L.png
	  DESTINATION ${TEMPLATEDIR}grid_br_a4_l
	)

	INSTALL(FILES
	grid_br_a4_p/Brochure_Grid_A4_P.sla.gz
	grid_br_a4_p/Brochure_Grid_A4_Ptn.png
	grid_br_a4_p/Brochure_Grid_A4_P.png
	  DESTINATION ${TEMPLATEDIR}grid_br_a4_p
	)

	INSTALL(FILES
	grid_br_975x85in_l/Brochure_Grid_975x85in_L.sla.gz
	grid_br_975x85in_l/Brochure_Grid_975x85_in_Ltn.png
	grid_br_975x85in_l/Brochure_Grid_975x85_in_L.png
	  DESTINATION ${TEMPLATEDIR}grid_br_975x85in_l
	)

	INSTALL(FILES
	grid_br_975x85_p/Brochure_Grid_975x85in_P.sla.gz
	grid_br_975x85_p/Brochure_Grid_975x85_in_Ptn.png
	grid_br_975x85_p/Brochure_Grid_975x85_in_P.png
	  DESTINATION ${TEMPLATEDIR}grid_br_975x85_p
	)

	INSTALL(FILES
	grid_br_375x85_p/Brochure_Grid_375x85in_P.sla.gz
	grid_br_375x85_p/Brochure_Grid_375x85_in_Ptn.png
	grid_br_375x85_p/Brochure_Grid_375x85_in_P.png
	  DESTINATION ${TEMPLATEDIR}grid_br_375x85_p
	)

	INSTALL(FILES
	grid_br_375x85_l/Brochure_Grid_375x85in_L.sla.gz
	grid_br_375x85_l/Brochure_Grid_375x85_in_Landscapetn.png
	grid_br_375x85_l/Brochure_Grid_375x85_in_Landscape.png
	  DESTINATION ${TEMPLATEDIR}grid_br_375x85_l
	)

	INSTALL(FILES
	grid_br_100x210_p/Brochure_Grid_100x210_P.sla.gz
	grid_br_100x210_p/Brochure_Grid_100x210_Ptn.png
	grid_br_100x210_p/Brochure_Grid_100x210_P.png
	  DESTINATION ${TEMPLATEDIR}grid_br_100x210_p
	)

	INSTALL(FILES
	grid_br_100x210_l/Brochure_Grid_100x210_L.sla.gz
	grid_br_100x210_l/Brochure_Grid_100x210_mm_Landscapetn.png
	grid_br_100x210_l/Brochure_Grid_100x210_mm_Landscape.png
	  DESTINATION ${TEMPLATEDIR}grid_br_100x210_l
	)

	INSTALL(FILES
	grid_br_55x86in_p/Brochure_Grid_55x85in_P.sla.gz
	grid_br_55x86in_p/Brochure_Grid_55x85in_Portraoittn.png
	grid_br_55x86in_p/Brochure_Grid_55x85in_Portraoit.png
	  DESTINATION ${TEMPLATEDIR}grid_br_55x86in_p
	)

	INSTALL(FILES
	grid_br_55x85in_l/Brochure_Grid_55x85in_L.sla.gz
	grid_br_55x85in_l/Brochure_Grid_55x85_in_Ltn.png
	grid_br_55x85in_l/Brochure_Grid_55x85_in_L.png
	  DESTINATION ${TEMPLATEDIR}grid_br_55x85in_l
	)

	INSTALL(FILES
	grid_poster_a3/Poster_Grid_A3.sla.gz
	grid_poster_a3/Poster_Grid_A3tn.png
	grid_poster_a3/Poster_Grid_A3.png
	  DESTINATION ${TEMPLATEDIR}grid_poster_a3
	)

	INSTALL(FILES
	grid_poster_a2/Poster_Grid_A2.sla.gz
	grid_poster_a2/Poster_Grid_A2tn.png
	grid_poster_a2/Poster_Grid_A2.png
	  DESTINATION ${TEMPLATEDIR}grid_poster_a2
	)

	INSTALL(FILES
	grid_poster_a1/Poster_Grid_A1.sla.gz
	grid_poster_a1/Poster_Grid_A1tn.png
	grid_poster_a1/Poster_Grid_A1.png
	  DESTINATION ${TEMPLATEDIR}grid_poster_a1
	)

	INSTALL(FILES
	grid_poster_a0/Poster_Grid_A0.sla.gz
	grid_poster_a0/Poster_Grid_A0tn.png
	grid_poster_a0/Poster_Grid_A0.png
	  DESTINATION ${TEMPLATEDIR}grid_poster_a0
	)

	INSTALL(FILES
	grid_br_usl_p/Brochure_Grid_USL_P.sla.gz
	grid_br_usl_p/Brochure_Grid_USL_Ptn.png
	grid_br_usl_p/Brochure_Grid_USL_P.png
	  DESTINATION ${TEMPLATEDIR}grid_poster_a0
	)

	INSTALL(FILES
	grid_br_usl_l/Brochure_Grid_USL_L.sla.gz
	grid_br_usl_l/Brochure_Grid_USL_Ltn.png
	grid_br_usl_l/Brochure_Grid_USL_L.png
	  DESTINATION ${TEMPLATEDIR}grid_br_usl_l
	)

	INSTALL(FILES
	grid_br_dinl_p/Brochure_Grid_DIN_LANG_P.sla.gz
	grid_br_dinl_p/Brochure_Grid_DIN_LANG_Ptn.png
	grid_br_dinl_p/Brochure_Grid_DIN_LANG_P.png
	  DESTINATION ${TEMPLATEDIR}grid_br_dinl_p
	)

	INSTALL(FILES
	grid_br_dinl_l/Brochure_Grid_DIN_LANG_L.sla.gz
	grid_br_dinl_l/Brochure_Grid_DIN_LANG_Ltn.png
	grid_br_dinl_l/Brochure_Grid_DIN_LANG_L.png
	  DESTINATION ${TEMPLATEDIR}grid_br_dinl_l
	)

	INSTALL(FILES
	grid_br_a5_p/Brochure_Grid_A5_P.sla.gz
	grid_br_a5_p/Brochure_Grid_A5_Portraittn.png
	grid_br_a5_p/Brochure_Grid_A5_Portrait.png
	  DESTINATION ${TEMPLATEDIR}grid_br_a5_p
	)

	INSTALL(FILES
	grid_br_a5_l/Brochure_Grid_A5_L.sla.gz
	grid_br_a5_l/Brochure_Grid_A5_Ltn.png
	grid_br_a5_l/Brochure_Grid_A5_L.png
	  DESTINATION ${TEMPLATEDIR}grid_br_a5_l
	)

	INSTALL(FILES
	buscard_coll/business_card_collection.sla.gz
	buscard_coll/Business_Card_Collectiontn.png
	buscard_coll/Business_Card_Collection.png
	  DESTINATION ${TEMPLATEDIR}buscard_coll
	)
	
	INSTALL(FILES
	Business_Card_EU_Green/Business_Card_Waves.sla.gz
	Business_Card_EU_Green/Business_Card_Wavestn.png
	Business_Card_EU_Green/Business_Card_Waves.png
	Business_Card_EU_Green/Line1.png
	  DESTINATION ${TEMPLATEDIR}Business_Card_EU_Green
	)
	
	INSTALL(FILES
	3Fold_Wave_Style/3Fold_Waves_Style.sla.gz
	3Fold_Wave_Style/3Fold_Waves_Styletn.png
	3Fold_Wave_Style/3Fold_Waves_Style.png
	3Fold_Wave_Style/IMAGES_SML.png
	3Fold_Wave_Style/IMAGES_BG.png
	3Fold_Wave_Style/3Fold_Waves_Style.jpeg
	  DESTINATION ${TEMPLATEDIR}3Fold_Wave_Style
	)
	
	INSTALL(FILES
	Flyer_Waves_A5_DoubleSided/Waves_Flyer.sla.gz
	Flyer_Waves_A5_DoubleSided/Waves_Flyer_tn.png
	Flyer_Waves_A5_DoubleSided/Waves_Flyer.png
	  DESTINATION ${TEMPLATEDIR}Flyer_Waves_A5_DoubleSided
	)
	
	INSTALL(FILES
	Layout_Informer_A4/Layout_Informer.sla.gz
	Layout_Informer_A4/Layout_Informer.png
	Layout_Informer_A4/Layout_Informertn.png
	  DESTINATION ${TEMPLATEDIR}Layout_Informer_A4
	)
	
	INSTALL(FILES
	Newsletter_Layout/Newsletter_Wavestyle.sla.gz
	Newsletter_Layout/Newsletter_Wavestyletn.png
	Newsletter_Layout/Newsletter_Wavestyle.png
	Newsletter_Layout/path9902.png
	  DESTINATION ${TEMPLATEDIR}Newsletter_Layout
	)
	  
ENDIF (WANT_NOTEMPLATES)
