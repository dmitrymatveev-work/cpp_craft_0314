find_package( Boost 1.55 ${SEARCH_PARAMETERS} COMPONENTS
	system
	filesystem
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost )

