project(ITKHDF5)
set(ITKHDF5_THIRD_PARTY 1)

if (BUILD_SHARED_LIBS)
   add_definitions(-DH5_BUILT_AS_DYNAMIC_LIB=1)
endif()
if(ITK_USE_SYSTEM_HDF5)
  set(ITKHDF5_LIBRARIES )
  if(BUILD_SHARED_LIBS)
    list(APPEND ITKHDF5_LIBRARIES ${HDF5_C_SHARED_LIBRARY} ${HDF5_CXX_SHARED_LIBRARY})
  else()
    list(APPEND ITKHDF5_LIBRARIES ${HDF5_C_STATIC_LIBRARY} ${HDF5_CXX_STATIC_LIBRARY})
  endif()

  set(ITKHDF5_INCLUDE_DIRS
    ${ITKHDF5_BINARY_DIR}/src # itk_hdf5.h and itk_H5Cpp.h
  )
  set(ITKHDF5_SYSTEM_INCLUDE_DIRS
    ${HDF5_INCLUDE_DIR}
    ${HDF5_INCLUDE_DIR_CPP}
    ${HDF5_INCLUDE_DIRS}
    ${HDF5_INCLUDE_DIR_CPP}
    )
  list(APPEND ITKHDF5_LIBRARIES
    ${HDF5_LIBRARIES}
    )
  set(ITKHDF5_NO_SRC 1)
else()
  set(ITKHDF5_INCLUDE_DIRS
    ${ITKHDF5_SOURCE_DIR}/src
    ${ITKHDF5_BINARY_DIR}/src
    )
  set(ITKHDF5_LIBRARIES itkhdf5_cpp itkhdf5 itkhdf5_hl itkhdf5_hl_cpp)
endif()

configure_file(src/itk_hdf5.h.in src/itk_hdf5.h)
configure_file(src/itk_H5Cpp.h.in src/itk_H5Cpp.h)
set(ITK_LIBRARY_PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS FALSE)
itk_module_impl()

install(FILES ${ITKHDF5_BINARY_DIR}/src/itk_hdf5.h
              ${ITKHDF5_BINARY_DIR}/src/itk_H5Cpp.h
  DESTINATION ${ITKHDF5_INSTALL_INCLUDE_DIR}
  COMPONENT Development
  )
