# ODADrawingsExplorer

set( ODADrawingsExplorer_SRCS
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqInterfaces.h
    ${TKERNEL_ROOT}/Examples/Common/ExDynamicModule.h 
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqAudit.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqClipData.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/ExAppServices.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/PropServices.h
    main.cpp
    OdqApplication.cpp
    OdqMainWindow.cpp
    OdqCustomization.cpp
    OdqAppProps.cpp
    OdqXmlUtils.cpp
    OdqPropertyDialog.cpp
    OdqPropertyPalette.cpp
    Services.cpp
    AppModule.cpp
    AppCommands.cpp
    ExAppServicesImpl.cpp 
    PropServicesImpl.cpp
    OdqXmlUtils.h
    Services.h 
    AppModule.h
    ExAppServicesImpl.h
    PropServicesImpl.h
    AppCommands.h
    ResBufPropValueBaseConvertors.h
)

if (NOT IOS AND NOT ANDROID)
set( ODADrawingsExplorer_SRCS
     ${ODADrawingsExplorer_SRCS}
     OdqPlotDialog.h
     OdqPlotDialog.cpp
     AppPlotCommands.cpp
)
endif (NOT IOS AND NOT ANDROID)

if (MSVC)
  set(ODADrawingsExplorer_SRCS
      ${ODADrawingsExplorer_SRCS}
  )

  include_directories(
      ${TKERNEL_ROOT}/Extensions/win/Crypt/
  )

  tdrawing_sources(ODADrawingsExplorer
    data/OdaQtApp.rc
  )
  remove_definitions(-D_CRTDBG_MAP_ALLOC)

  if(MSVC_VERSION GREATER 1310)
    set(CMAKE_MANIFEST_ADDITIONAL_FILES "$(TargetDir)/TDXdependency.manifest")
  endif(MSVC_VERSION GREATER 1310)

endif(MSVC)

set( ODADrawingsExplorer_MOC_HDRS
    OdqApplication.h
    OdqMainWindow.h
    OdqCustomization.h
    OdqAppProps.h
    OdqPropertyDialog.h
    OdqPropertyPalette.h
)

if (NOT IOS AND NOT ANDROID)
set( ODADrawingsExplorer_MOC_HDRS
     ${ODADrawingsExplorer_MOC_HDRS}
     OdqPlotDialog.h
  )
else (NOT IOS AND NOT ANDROID)
  include_directories(${TA_ROOT}/include)
endif (NOT IOS AND NOT ANDROID)

add_definitions(-DODA_PROP_SERVICES_UNLINKED_RXINIT
    -DODA_EX_APP_SERVICES_UNLINKED_RXINIT
    -DODA_QT_NO_CAUSTIC_OPENRL
    -DODA_QT_NO_QZIP
    -DNOMINMAX
)

if (MACOS_X86 OR MACOS_X64)
  add_definitions(-DQ_OS_MAC)
endif()

SET( ODADrawingsExplorer_RESOURCES_RCS
  data/data.qrc
)

# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
include_directories(
                    ${TDRAWING_ROOT}/Examples/Qt/Common
                    ${TKERNEL_ROOT}/Examples/Common
                    ${TKERNEL_ROOT}/Include
                    ${TDRAWING_ROOT}/Include
                    ${TDRAWING_ROOT}/Examples/Common
                    ${TKERNEL_ROOT}/Extensions/ExServices
                    ${TKERNEL_ROOT}/Extensions/ExRender
                    ${TDRAWING_ROOT}/Examples/Common
                    ${TDRAWING_ROOT}/Include
                    ${TDRAWING_ROOT}/Examples/Common
                    ${TDRAWING_ROOT}/Extensions/ExServices
)

if (QT4)
  # this command will generate rules that will run rcc on all files from SAMPLE_RCS
  # in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
  qt4_add_resources(ODADrawingsExplorer_RC_SRCS ${ODADrawingsExplorer_RESOURCES_RCS})

  # run moc
  if(MACOS_X64 OR MACOS_X86)
    qt4_wrap_cpp(ODADrawingsExplorer_MOC_SRCS ${ODADrawingsExplorer_MOC_HDRS} OPTIONS -DQ_OS_MAC)
  else()
    qt4_wrap_cpp(ODADrawingsExplorer_MOC_SRCS ${ODADrawingsExplorer_MOC_HDRS})
  endif()

  tdrawing_sources(ODADrawingsExplorer ${ODADrawingsExplorer_SRCS} ${ODADrawingsExplorer_MOC_SRCS} ${ODADrawingsExplorer_RC_SRCS})
  if (MSVC)
    set(QT_LIBRARIES ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})
  endif (MSVC)

elseif (QT5)

  qt5_add_resources(ODADrawingsExplorer_RC_SRCS ${ODADrawingsExplorer_RESOURCES_RCS})
  tdrawing_sources(ODADrawingsExplorer ${ODADrawingsExplorer_SRCS} ${ODADrawingsExplorer_MOC_HDRS} ${ODADrawingsExplorer_RC_SRCS})
  set(QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5PrintSupport_LIBRARIES} ${Qt5Core_LIBRARIES} )
  if (MSVC)
    set(QT_LIBRARIES ${QT_LIBRARIES} Qt5::WinMain )
  endif (MSVC)

endif ()

if(ANDROID)
  set(ADDITIONAL_GL_LIBS GLESv2 EGL android)
  tdrawing_module( ODADrawingsExplorer ${TA_COMMONEX_LIB} ${TA_AECSCHEDULE_LIB} ${TA_AECARCHDACHBASE_LIB} ${TA_AECAREACALCULATIONBASE_LIB}
              ${TA_AECSTRUCTUREBASE_LIB} ${TA_AECARCHBASE_LIB} ${TA_AECSCHEDULEDATA_LIB} ${TA_AECBASE_LIB} 
              ${TA_AECGEOMETRY_LIB} ${FACET_MODELER_LIB} ${TD_DGNDB_LIB} ${TG_DGN7IO_LIB} ${TG_MODELERGEOMETRY_LIB} ${TG_PS_TOOLKIT_LIB} ${TG_DB_LIB} 
              ${TD_QT_CONSOLE_LIB} ${TD_QT_PLOTSV_LIB} ${TD_QT_PROPSYSTEM_LIB} ${TD_QT_VIEWSYSTEM_LIB} ${TD_QT_GLES2_LIB} 
              ${TR_VEC_GL2_LIB} ${TD_TXV_GLES2_LIB} ${TR_GL2_LIB} ${TR_VEC_LIB} ${TR_BASE_LIB}
              ${TD_MODELER_LIB} ${TCOMPONENTS_BREPMODELER_LIB} ${TD_BREPBUILDERFILLER_LIB}
              ${TD_BREPRENDERER_LIB} ${TD_BR_LIB} ${TD_ACISBLDR_LIB}  ${TD_EXLIB} ${TD_DR_EXLIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} 
              ${TD_GS_LIB} ${TD_GI_LIB} ${TD_SPATIALINDEX_LIB} ${TD_RDIMBLK_LIB} ${TD_FIELDEVAL_LIB} ${TD_BREPBUILDER_LIB} ${TD_GE_LIB} 
              ${TD_ROOT_LIB} ${ODA_ADD_SISL} ${TH_WCHAR_LIB} ${TD_ALLOC_LIB} ${TH_FT_LIB} ${TH_CONDITIONAL_LIBCRYPTO}
              ${TH_TINYXML_LIB} ${TH_ZLIB_LIB} ${TH_OLESS_LIB} ${QT_LIBRARIES} ${ODA_PLATFORM_LIBS} ${ODA_OPENGL_LIBS} ${ADDITIONAL_GL_LIBS} android log gcc
  )
  oda_qt_set_definition(ODADrawingsExplorer "TD_USE_QT_LIB")
  oda_qt_set_cxxflags(ODADrawingsExplorer "")
else(ANDROID)
  tdrawing_executable(ODADrawingsExplorer ${TD_EXLIB} ${TD_DR_EXLIB} ${TD_KEY_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} 
                ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_SPATIALINDEX_LIB}
                ${TD_ROOT_LIB} ${ODA_ADD_SISL} ${TD_ALLOC_LIB} ${TH_CONDITIONAL_LIBCRYPTO} ${QT_LIBRARIES}
  )
endif(ANDROID)

if (QT5)
  qt5_use_modules(ODADrawingsExplorer Core Widgets Xml PrintSupport)
endif(QT5)

if(MSVC)
  set_target_properties(ODADrawingsExplorer PROPERTIES WIN32_EXECUTABLE 1)
if (NOT QT4)
  set_target_properties(ODADrawingsExplorer PROPERTIES COMPILE_FLAGS_REMOVE "/Zc:wchar_t-")
endif (NOT QT4)
elseif (MACOS_X86 OR MACOS_X64)
  oda_set_macos_bundle_properties(ODADrawingsExplorer odalogo "${CMAKE_CURRENT_SOURCE_DIR}/../how_to_build_qt_mac_xcode/data")
elseif (ANDROID)
  add_custom_command (TARGET ODADrawingsExplorer POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/data/android-sources/res/drawable-mdpi")
  add_custom_command (TARGET ODADrawingsExplorer POST_BUILD COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/data/odalogo.png" "${CMAKE_CURRENT_SOURCE_DIR}/data/android-sources/res/drawable-mdpi")
  oda_qt_create_apk(ODADrawingsExplorer)
endif(MSVC)


tdrawing_project_group(ODADrawingsExplorer "Examples/Qt")
