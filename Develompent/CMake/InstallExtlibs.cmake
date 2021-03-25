function( InstallExtlibs EXTLIBS_DIR INSTALL_DIR )
	if( ${CMAKE_SYSTEM_NAME} MATCHES "Windows" )
		if ( ${ARCH_TYPE} MATCHES "x64" )
			set( BIN_DIR Bin/Win64 )
		else()
			set( BIN_DIR Bin/Win32 )
		endif()
		
		set( BINARES "${EXTLIBS_DIR}/SDL2/${BIN_DIR}/SDL2.dll" )
	endif()	
	
	install( FILES ${BINARES} DESTINATION ${INSTALL_DIR} )
endfunction()