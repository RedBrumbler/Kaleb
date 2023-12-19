# cmake file that generates the assets.hpp file

if (NOT DEFINED ASSETS_FILE)
    message(FATAL_ERROR "define ASSETS_FILE was not passed to assets_include.cmake")
endif()
if (NOT DEFINED ASSET_DECLARES)
    message(FATAL_ERROR "define ASSET_DECLARES was not passed to assets_include.cmake")
endif()

function(asset_file assets_file asset_declares)
    file(WRITE ${assets_file} "#pragma once\n")
    file(APPEND ${assets_file} "#include \"kaleb/shared/kaleb.hpp\"\n\n")

    foreach(decl IN LISTS asset_declares)
        file(APPEND ${assets_file} "${decl};\n")
    endforeach()
endfunction()

asset_file("${ASSETS_FILE}", "${ASSET_DECLARES}")
