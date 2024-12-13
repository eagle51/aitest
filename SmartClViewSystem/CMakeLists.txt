# OdaQtViewSystem

set( OdaQtViewSystem_SRCS
  ViewSystemModule.cpp
  ViewSystemCommands.cpp
  OdqViewSystem.cpp
  OdqView.cpp 
  ../Common/OdqInterfaces.h
  ${TKERNEL_ROOT}/Examples/Common/ExDynamicModule.h 
  ViewSystemModule.h
  ViewSystemCommands.h
  ${TDRAWING_ROOT}/Examples/Editor/ExGripManager.h
  ${TDRAWING_ROOT}/Examples/Editor/ExGripManager.cpp
)

set( OdaQtViewSystem_MOC_HDRS
  OdqViewSystem.h
  OdqView.h
)

set(QT_USE_QTOPENGL TRUE)

include_directories(
                    ${TDRAWING_ROOT}/Examples/Qt/Common
                    ${TKERNEL_ROOT}/Examples/Common
                    ${TDRAWING_ROOT}/Examples/Common
                    ${TKERNEL_ROOT}/Include
                    ${TDRAWING_ROOT}/Include
                    ${TKERNEL_ROOT}/Extensions/ExRender
                    ${TKERNEL_ROOT}/Extensions/ExServices
                    ${TDRAWING_ROOT}/Extensions/ExServices
                    ${TDRAWING_ROOT}/Examples/Editor
)

if(MSVC)
  tdrawing_sources(${TD_QT_VIEWSYSTEM_LIB}
    data/OdqViewSystem.rc
  )
  remove_definitions(-D_CRTDBG_MAP_ALLOC)
endif(MSVC)

if (QT4)
  qt4_wrap_cpp(OdaQtViewSystem_MOC_SRCS ${OdaQtViewSystem_MOC_HDRS})
  tdrawing_sources(${TD_QT_VIEWSYSTEM_LIB} ${OdaQtViewSystem_SRCS} ${OdaQtViewSystem_MOC_SRCS} ${OdaQtViewSystem_RC_SRCS})
elseif (QT5)
  add_definitions(-DNOMINMAX)
  tdrawing_sources(${TD_QT_VIEWSYSTEM_LIB} ${OdaQtViewSystem_SRCS} ${OdaQtViewSystem_MOC_HDRS} ${OdaQtViewSystem_MOC_SRCS})
  set (QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5OpenGL_LIBRARIES})
endif (QT4)

tdrawing_tx_component(${TD_QT_VIEWSYSTEM_LIB} ${TD_EXLIB} ${TD_DR_EXLIB} ${TD_KEY_LIB} ${TD_DB_LIB} ${TD_DBROOT_LIB} 
                                  ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_SPATIALINDEX_LIB}
                                  ${TD_ROOT_LIB} ${ODA_ADD_SISL} ${TD_ALLOC_LIB} ${QT_LIBRARIES} ${ODA_OPENGL_LIBS}
)

if (QT5)
  qt5_use_modules(${TD_QT_VIEWSYSTEM_LIB} Core Widgets OpenGL)
  if(MSVC)
    set_target_properties (${TD_QT_VIEWSYSTEM_LIB} PROPERTIES COMPILE_FLAGS_REMOVE "/Zc:wchar_t-")
  endif(MSVC)
  
  oda_qt_set_definition(${TD_QT_VIEWSYSTEM_LIB} "")
  oda_qt_set_cxxflags(${TD_QT_VIEWSYSTEM_LIB} "")
endif(QT5)

tdrawing_project_group(${TD_QT_VIEWSYSTEM_LIB} "Examples/Qt")
