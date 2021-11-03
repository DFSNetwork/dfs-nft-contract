project(dfsnftassets)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( dfsnftassets dfsnftassets dfsnftassets.cpp )
target_include_directories( dfsnftassets PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( dfsnftassets ${CMAKE_SOURCE_DIR}/../ricardian )