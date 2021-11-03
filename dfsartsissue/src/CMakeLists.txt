project(dfsartsissue)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( dfsartsissue dfsartsissue dfsartsissue.cpp )
target_include_directories( dfsartsissue PUBLIC ${CMAKE_SOURCE_DIR}/../include )
target_ricardian_directory( dfsartsissue ${CMAKE_SOURCE_DIR}/../ricardian )