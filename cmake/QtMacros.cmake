
MACRO( PREPARE_USE_QT )

	SET( QT_USED_MODULES ${ARGN} )

	IF( USE_QT5 )

		FOREACH( mod ${QT_USED_MODULES} )

			IF( NOT ${Qt5${mod}_DIR} )
				FIND_PACKAGE( Qt5${mod} REQUIRED )
			ENDIF()

			INCLUDE( ${Qt5${mod}_DIR}/Qt5${mod}Config.cmake )

		ENDFOREACH()

	ELSE( USE_QT5 )

		SET( QT_DONT_USE_QTGUI 1 )
		INCLUDE( ${QT_USE_FILE} )

	ENDIF( USE_QT5 )

ENDMACRO( PREPARE_USE_QT )

MACRO( ADD_QT_EXECUTABLE _target )

	ADD_EXECUTABLE( ${_target} ${ARGN} )

	IF( USE_QT5 )

		QT5_USE_MODULES( ${_target} ${QT_USED_MODULES} )

	ELSE( USE_QT5 )

		TARGET_LINK_LIBRARIES( ${_target} ${QT_LIBRARIES} )

	ENDIF( USE_QT5 )

ENDMACRO( ADD_QT_EXECUTABLE )

MACRO( ADD_QT_LIBRARY _target )

	ADD_LIBRARY( ${_target} ${ARGN} )

	IF( USE_QT5 )

		QT5_USE_MODULES( ${_target} ${QT_USED_MODULES} )

	ELSE( USE_QT5 )

		TARGET_LINK_LIBRARIES( ${_target} ${QT_LIBRARIES} )

	ENDIF( USE_QT5 )

ENDMACRO( ADD_QT_LIBRARY )

IF( USE_QT5 )

	MACRO( QT_MOC SourcesVar )

		SET( Mocables ${ARGN} )
		QT5_GET_MOC_FLAGS(_moc_INCS)

		SET(_matching_FILES )
		FOREACH (_current_FILE ${Mocables})

			GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
			# if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
			# This is required to make uic work correctly:
			# we need to add generated .cpp files to the sources (to compile them),
			# but we cannot let automoc handle them, as the .cpp files don't exist yet when
			# cmake is run for the very first time on them -> however the .cpp files might
			# exist at a later run. at that time we need to skip them, so that we don't add two
			# different rules for the same moc file
			GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

			IF ( NOT _skip AND EXISTS ${_abs_FILE} )

				FILE(READ ${_abs_FILE} _contents)

				STRING(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
				IF(_match)

					GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
					SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
					QT5_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

					LIST( APPEND ${SourcesVar} ${_moc} )
					# also, keep the generated moc from beeing scanned by auto-moc'ings
					SET_SOURCE_FILES_PROPERTIES(
						${_moc}
						PROPERTIES  SKIP_AUTOMOC TRUE )

				ENDIF(_match)
			ENDIF( NOT _skip AND EXISTS ${_abs_FILE} )
		ENDFOREACH(_current_FILE)
	ENDMACRO()

	MACRO( QT_UIC out )
		QT5_WRAP_UI( ${out} ${ARGN} )
	ENDMACRO()

	function(QT_RCC infiles outfiles )

		set(options)
		set(oneValueArgs)
		set(multiValueArgs OPTIONS)

		cmake_parse_arguments(_RCC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

		set(rcc_files ${_RCC_UNPARSED_ARGUMENTS})
		set(rcc_options ${_RCC_OPTIONS})

		foreach(it ${rcc_files})
			get_filename_component(outfilename ${it} NAME_WE)
			get_filename_component(infile ${it} ABSOLUTE)
			get_filename_component(rc_path ${infile} PATH)
			set(outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${outfilename}.cxx)

			set(_RC_DEPENDS)
			if(EXISTS "${infile}")
				#  parse file for dependencies
				#  all files are absolute paths or relative to the location of the qrc file
				file(READ "${infile}" _RC_FILE_CONTENTS)
				string(REGEX MATCHALL "<file[^<]+" _RC_FILES "${_RC_FILE_CONTENTS}")
				foreach(_RC_FILE ${_RC_FILES})
					string(REGEX REPLACE "^<file[^>]*>" "" _RC_FILE "${_RC_FILE}")
					if(NOT IS_ABSOLUTE "${_RC_FILE}")
						set(_RC_FILE "${rc_path}/${_RC_FILE}")
					endif()
					LIST( APPEND ${infiles} ${_RC_FILE} )
					set(_RC_DEPENDS ${_RC_DEPENDS} "${_RC_FILE}")
				endforeach()
				# Since this cmake macro is doing the dependency scanning for these files,
				# let's make a configured file and add it as a dependency so cmake is run
				# again when dependencies need to be recomputed.
				qt5_make_output_file("${infile}" "" "qrc.depends" out_depends)
				configure_file("${infile}" "${out_depends}" COPY_ONLY)
			else()
				# The .qrc file does not exist (yet). Let's add a dependency and hope
				# that it will be generated later
				set(out_depends)
			endif()

			add_custom_command(OUTPUT ${outfile}
							   COMMAND ${QT_RCC_EXECUTABLE}
							   ARGS ${rcc_options} -name ${outfilename} -o ${outfile} ${infile}
							   MAIN_DEPENDENCY ${infile}
							   DEPENDS ${_RC_DEPENDS} "${out_depends}" VERBATIM)
			list(APPEND ${outfiles} ${outfile})
		endforeach()
		set(${outfiles} ${${outfiles}} PARENT_SCOPE)
	endfunction()

ELSE()

	MACRO(QT4_MOC SourcesVar )

	  SET( Mocables ${ARGN} )
	  QT4_GET_MOC_FLAGS(_moc_INCS)

	  SET(_matching_FILES )
	  FOREACH (_current_FILE ${Mocables})

		GET_FILENAME_COMPONENT(_abs_FILE ${_current_FILE} ABSOLUTE)
		# if "SKIP_AUTOMOC" is set to true, we will not handle this file here.
		# This is required to make uic work correctly:
		# we need to add generated .cpp files to the sources (to compile them),
		# but we cannot let automoc handle them, as the .cpp files don't exist yet when
		# cmake is run for the very first time on them -> however the .cpp files might
		# exist at a later run. at that time we need to skip them, so that we don't add two
		# different rules for the same moc file
		GET_SOURCE_FILE_PROPERTY(_skip ${_abs_FILE} SKIP_AUTOMOC)

		IF ( NOT _skip AND EXISTS ${_abs_FILE} )

		  FILE(READ ${_abs_FILE} _contents)

		  STRING(REGEX MATCHALL "Q_OBJECT" _match "${_contents}")
		  IF(_match)

				GET_FILENAME_COMPONENT(_basename ${_current_FILE} NAME_WE)
				SET(_moc    ${CMAKE_CURRENT_BINARY_DIR}/moc_${_basename}.cpp)
				QT4_CREATE_MOC_COMMAND(${_abs_FILE} ${_moc} "${_moc_INCS}" "")

				LIST( APPEND ${SourcesVar} ${_moc} )
				# also, keep the generated moc from beeing scanned by auto-moc'ings
				SET_SOURCE_FILES_PROPERTIES(
					${_moc}
					PROPERTIES  SKIP_AUTOMOC TRUE )

		  ENDIF(_match)
		ENDIF ( NOT _skip AND EXISTS ${_abs_FILE} )
	  ENDFOREACH (_current_FILE)
	ENDMACRO(QT4_MOC)

	MACRO( QT4_UIC outfiles )
		QT4_EXTRACT_OPTIONS(ui_files ui_options ${ARGN})

		FOREACH (it ${ui_files})

			GET_FILENAME_COMPONENT( itbase  ${it} NAME     )
			GET_FILENAME_COMPONENT( outfile ${it} NAME_WE  )
			GET_FILENAME_COMPONENT( infile  ${it} ABSOLUTE )

			SET( outfile ${CMAKE_CURRENT_BINARY_DIR}/ui_${outfile}.h )
			ADD_CUSTOM_COMMAND(
				OUTPUT			${outfile}
				COMMAND			${QT_UIC_EXECUTABLE}
				ARGS			${ui_options} -o ${outfile} ${infile}
				MAIN_DEPENDENCY	${infile}
				COMMENT			"UIC'ing ${itbase}"
			)
			SET( ${outfiles} ${${outfiles}} ${outfile} )
		ENDFOREACH( it )

	ENDMACRO( QT4_UIC )

	MACRO(QT4_RCC infiles outfiles )
	  QT4_EXTRACT_OPTIONS(rcc_files rcc_options ${ARGN})

	  FOREACH (it ${rcc_files})
		GET_FILENAME_COMPONENT(outfilename ${it} NAME_WE)
		GET_FILENAME_COMPONENT(infile ${it} ABSOLUTE)
		GET_FILENAME_COMPONENT(rc_path ${infile} PATH)
		SET(outfile ${CMAKE_CURRENT_BINARY_DIR}/qrc_${outfilename}.cxx)
		#  parse file for dependencies
		#  all files are absolute paths or relative to the location of the qrc file
		FILE(READ "${infile}" _RC_FILE_CONTENTS)
		STRING(REGEX MATCHALL "<file[^<]+" _RC_FILES "${_RC_FILE_CONTENTS}")
		SET(_RC_DEPENDS)
		FOREACH(_RC_FILE ${_RC_FILES})
		  STRING(REGEX REPLACE "^<file[^>]*>" "" _RC_FILE "${_RC_FILE}")
		  IF(NOT IS_ABSOLUTE "${_RC_FILE}")
			SET(_RC_FILE "${rc_path}/${_RC_FILE}")
		  ENDIF(NOT IS_ABSOLUTE "${_RC_FILE}")
		  LIST( APPEND ${infiles} ${_RC_FILE} )
		  SET(_RC_DEPENDS ${_RC_DEPENDS} "${_RC_FILE}")
		ENDFOREACH(_RC_FILE)
		# Since this cmake macro is doing the dependency scanning for these files,
		# let's make a configured file and add it as a dependency so cmake is run
		# again when dependencies need to be recomputed.
		QT4_MAKE_OUTPUT_FILE("${infile}" "" "qrc.depends" out_depends)
		CONFIGURE_FILE("${infile}" "${out_depends}" COPY_ONLY)
		ADD_CUSTOM_COMMAND(OUTPUT ${outfile}
		  COMMAND ${QT_RCC_EXECUTABLE}
		  ARGS ${rcc_options} -name ${outfilename} -o ${outfile} ${infile}
		  MAIN_DEPENDENCY ${infile}
		  DEPENDS ${_RC_DEPENDS} "${out_depends}" VERBATIM)
		SET(${outfiles} ${${outfiles}} ${outfile})
	  ENDFOREACH (it)

	ENDMACRO(QT4_RCC)


ENDIF()
