project(rootnftissue)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( rootnftissue rootnftissue rootnftissue.cpp )
target_include_directories( rootnftissue PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( rootnftissue ${CMAKE_SOURCE_DIR}/../ricardian )