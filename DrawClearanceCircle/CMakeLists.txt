# OdaQtConsole

set( OdaQtConsole_SRCS
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqInterfaces.h
    ${TKERNEL_ROOT}/Examples/Common/ExDynamicModule.h 
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqAudit.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqTdSaveAsFilters.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/OdqClipData.h
    ${TDRAWING_ROOT}/Examples/Common/SysVarPE.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/PropServices.h
    ${TDRAWING_ROOT}/Examples/Qt/Common/ExAppServices.h
    ConsoleModule.cpp
    ConsoleCommands.cpp

    ConsoleReactors.cpp
    CommandContextImpl.cpp
    OdqConsole.cpp 
    OdqConsoleTab.cpp 
    OdqCommandLineEdit.cpp
    OdqAudit.cpp
    SysVarPEImpl.cpp
    OdqGripManager.cpp
    OdqSnapManager.cpp
    OdqDragTracker.cpp
    ConsoleModule.h
    OdqDragTracker.h
    ConsoleCommands.h
    ConsoleReactors.h
    CommandContextImpl.h
    SysVarPEImpl.h
    OdqGripManager.h
    OdqSnapManager.h
    OdqSysVarDefs.h
    ${TDRAWING_ROOT}/Examples/Editor/ExGripManager.h
    ${TDRAWING_ROOT}/Examples/Editor/ExGripManager.cpp
    ${TDRAWING_ROOT}/Examples/Editor/OSnapManager.h
    ${TDRAWING_ROOT}/Examples/Editor/OSnapManager.cpp
    ${TDRAWING_ROOT}/Extensions/DbConstraints/DbAssocManager.h
)

if(NOT ANDROID AND NOT IOS)
set( OdaQtConsole_SRCS
    ${OdaQtConsole_SRCS}
    ConsoleAppLoadCommand.cpp
)
endif(NOT ANDROID AND NOT IOS)

set( OdaQtConsole_MOC_HDRS
    OdqConsole.h
    OdqConsoleTab.h
    OdqCommandLineEdit.h
    OdqGripManager.h
)

add_definitions(-DODA_SYS_VAR_PE_UNLINKED_RXINIT
    -DODA_PROP_SERVICES_UNLINKED_RXINIT
    -DODA_EX_APP_SERVICES_UNLINKED_RXINIT
    -DNOMINMAX
)

if(MSVC)
  tdrawing_sources(${TD_QT_CONSOLE_LIB}
    data/OdqConsole.rc
  )
  remove_definitions(-D_CRTDBG_MAP_ALLOC)
endif(MSVC)

  
# Add the include directories for the Qt 5 Widgets module to
# the compile lines.
include_directories(
                    ${TDRAWING_ROOT}/Examples/Qt/Common
                    ${TKERNEL_ROOT}/Examples/Common
                    ${TDRAWING_ROOT}/Examples/Common
                    ${TKERNEL_ROOT}/Include
                    ${TDRAWING_ROOT}/Include
                    ${TKERNEL_ROOT}/Extensions/ExServices
                    ${TDRAWING_ROOT}/Extensions/ExServices
                    ${TDRAWING_ROOT}/Examples/Editor
                    ${TDRAWING_ROOT}/Extensions/DbConstraints
)

if (QT4)
  if(MACOS_X64 OR MACOS_X86)
    qt4_wrap_cpp(OdaQtConsole_MOC_SRCS ${OdaQtConsole_MOC_HDRS} OPTIONS -DQ_OS_MAC)
  else()
    qt4_wrap_cpp(OdaQtConsole_MOC_SRCS ${OdaQtConsole_MOC_HDRS})
  endif()
  tdrawing_sources(${TD_QT_CONSOLE_LIB} ${OdaQtConsole_SRCS} ${OdaQtConsole_MOC_SRCS})
elseif (QT5)
  if(APPLE)
    add_definitions(-DQ_OS_MAC)
  endif()
  tdrawing_sources(${TD_QT_CONSOLE_LIB} ${OdaQtConsole_SRCS} ${OdaQtConsole_MOC_HDRS})
  set (QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES})
endif (QT4)

tdrawing_tx_component(${TD_QT_CONSOLE_LIB} ${TD_EXLIB} ${TD_DB_CONSTRAINTS_LIB} ${TD_DR_EXLIB} ${TD_DB_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_DBROOT_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${QT_LIBRARIES})

if (QT5)
  qt5_use_modules(${TD_QT_CONSOLE_LIB} Core Widgets)
  if(MSVC)
    set_target_properties (${TD_QT_CONSOLE_LIB} PROPERTIES COMPILE_FLAGS_REMOVE "/Zc:wchar_t-")
  endif(MSVC)
endif(QT5)

oda_qt_set_definition(${TD_QT_CONSOLE_LIB} "")
oda_qt_set_cxxflags(${TD_QT_CONSOLE_LIB} "")

tdrawing_project_group(${TD_QT_CONSOLE_LIB} "Examples/Qt")
