macro(prepend_copy from to)
    add_custom_command(
        OUTPUT "${to}"
        COMMAND ${CMAKE_COMMAND} -E echo_append "                                " > "${to}" # start with 32 bytes
        COMMAND ${CMAKE_COMMAND} -E cat "${from}" >> "${to}" # content
        COMMAND ${CMAKE_COMMAND} -E echo_append " " >> "${to}" # end with an extra byte for a null byte to use the data as a c string
        DEPENDS "${from}"
    )
endmacro()

macro(asset_binary workdir relative_from to)
    add_custom_command(
            OUTPUT "${to}"
            COMMAND ${CMAKE_OBJCOPY} "${relative_from}" "${to}" --input-target binary --output-target elf64-aarch64 --set-section-flags binary=strings
            DEPENDS "${workdir}/${relative_from}"
            WORKING_DIRECTORY "${workdir}"
    )
endmacro()

function(ensure_dir file_path)
    get_filename_component(dir "${file_path}" DIRECTORY)
    file(MAKE_DIRECTORY ${dir})
endfunction()

function(include_asset assets_folder file_path output_o ns_o id_o)
    # it's no use including something that doesn't exist
    if (NOT EXISTS "${file_path}")
        message(error "Not including asset ${file_path}: it did not exist!")
        return()
    endif()

    file(RELATIVE_PATH relative_path "${assets_folder}" "${file_path}")

    get_filename_component(file_name "${file_path}" NAME)
    get_filename_component(file_folder "${file_path}" DIRECTORY)
    get_filename_component(relative_folder "${relative_path}" DIRECTORY)

    string(MAKE_C_IDENTIFIER "${file_name}" identifier)

    string(REPLACE "\\" "/" file_folder "${file_folder}")

    string(REPLACE "\\" "/" relative_path "${relative_path}")
    string(REPLACE "\\" "/" relative_folder "${relative_folder}")
    string(REPLACE "/" ";" split_folders "${relative_folder}")

    set(namespaces "")
    foreach(folder IN LISTS split_folders)
        string(MAKE_C_IDENTIFIER "${folder}" ns)
        list(APPEND namespaces "${ns}")
    endforeach()

    string(JOIN "::" namespace ${namespaces})

    set(prepended_dir "${CMAKE_CURRENT_BINARY_DIR}/prependedAssets")
    set(binaries_dir "${CMAKE_CURRENT_BINARY_DIR}/assets")

    file(MAKE_DIRECTORY ${prepended_dir})
    file(MAKE_DIRECTORY ${binaries_dir})

    set(prepended_output "${prepended_dir}/${relative_path}")
    set(asset_output "${binaries_dir}/${relative_path}.o")

    ensure_dir(${prepended_output})
    ensure_dir(${asset_output})

    prepend_copy(${file_path} ${prepended_output})
    asset_binary(${prepended_dir} ${relative_path} ${asset_output})

    message(STATUS "Asset ${file_path} will be included as: ${namespace}::${identifier}")
    set(${output_o} "${asset_output}" PARENT_SCOPE)
    set(${ns_o} "${namespace}" PARENT_SCOPE)
    set(${id_o} "${identifier}" PARENT_SCOPE)
endfunction()

function(binary_identifier namespace identifier identifier_o)
    string(MAKE_C_IDENTIFIER namespace "${namespace}")
    string(MAKE_C_IDENTIFIER identifier "${identifier}")

    string(LENGTH "${namespace}" result)
    if (result GREATER 0)
        set(${identifier_o} "_binary_${namespace}_${identifier}" PARENT_SCOPE)
    else()
        set(${identifier_o} "_binary_${identifier}" PARENT_SCOPE)
    endif()
endfunction()

function(asset_declare namespace identifier declare_o)
    binary_identifier("${namespace}" "${identifier}" bin_id)

    string(LENGTH "${namespace}" result)
    if (result GREATER 0)
        set(${declare_o} "DECLARE_FILE(${bin_id}, Assets::${namespace}, ${identifier})" PARENT_SCOPE)
    else()
        set(${declare_o} "DECLARE_FILE(${bin_id}, Assets, ${identifier})" PARENT_SCOPE)
    endif()
endfunction()

function(asset_file assets_file asset_declares)
    file(REMOVE ${assets_file}) # refresh
    file(APPEND ${assets_file} "#pragma once\n")
    file(APPEND ${assets_file} "#include \"kaleb/shared/kaleb.hpp\"\n\n")

    foreach(decl IN LISTS asset_declares)
        file(APPEND ${assets_file} "${decl};\n")
    endforeach()
endfunction()

function(add_assets assets_binary kind assets_folder assets_file)
    file(GLOB_RECURSE assets LIST_DIRECTORIES false "${assets_folder}/*")

    set(compiled_assets "")
    set(asset_declares "")
    set(ANY OFF)
    foreach(asset IN LISTS assets)
        include_asset("${assets_folder}" "${asset}" obj namespace identifier)
        list(APPEND compiled_assets "${obj}")

        asset_declare("${namespace}" "${identifier}" decl)
        list(APPEND asset_declares "${decl}")
        set(ANY ON)
    endforeach()

    if (NOT ANY)
        message(STATUS "No assets found to include")
    endif()
    add_library(${assets_binary} ${kind} ${compiled_assets})
    set_target_properties(${assets_binary} PROPERTIES LINKER_LANGUAGE CXX)

    asset_file("${assets_file}" "${asset_declares}")
endfunction()
