find_package( Boost ${SEARCH_PARAMETERS} COMPONENTS
	system
	thread
	date_time
	chrono
	regex
)

compile_project( ${module_name} "*.cpp" "*.h" BINARY binaries Boost trade_message quote_message udp_listener market_data_processor multicast_communication minute_calculator minute_market_data )


