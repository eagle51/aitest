# OdaQtOpenGL

set( OdaQtOpenGL_SRCS
    ExGsQtVectorizeDevice.cpp
    ExGsQtDevice.cpp
    OpenGLModule.cpp
    ExGsQtVectorizeDevice.h 
    ExGsQtDevice.h 
    ExGsQtScreenDevice.h 
)

set( OdaQtOpenGL_MOC_HDRS
)

if(WIN32)
  tdrawing_sources(${TD_QT_OPENGL_LIB}
    data/OdqOpenGL.rc
  )
  remove_definitions(-D_CRTDBG_MAP_ALLOC)
endif(WIN32)

  
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
                    ${TKERNEL_ROOT}/Extensions/ExRender
                    ${TKERNEL_ROOT}/Extensions/ExRender/OpenGL
)

add_definitions(-DNOMINMAX)

if (QT4)
  qt4_wrap_cpp(OdaQtOpenGL_MOC_SRCS ${OdaQtOpenGL_MOC_HDRS})
  tdrawing_sources(${TD_QT_OPENGL_LIB} ${OdaQtOpenGL_SRCS} ${OdaQtOpenGL_MOC_SRCS} ${OdaQtOpenGL_RC_SRCS})
elseif (QT5)
  tdrawing_sources(${TD_QT_OPENGL_LIB} ${OdaQtOpenGL_SRCS})
  set (QT_LIBRARIES ${QT_LIBRARIES} ${Qt5Widgets_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5OpenGL_LIBRARIES})
endif (QT4)

tdrawing_txv(${TD_QT_OPENGL_LIB} ${TD_OPENGL_LIB} ${TD_GS_LIB} ${TD_GI_LIB} ${TD_GE_LIB} ${TD_ROOT_LIB} ${TD_DB_LIB}
      ${TD_DBROOT_LIB} ${TD_ROOT_LIB} ${TD_ALLOC_LIB} ${ODA_OPENGL_LIBS} ${QT_LIBRARIES})

if (QT5)
  qt5_use_modules(${TD_QT_OPENGL_LIB} Core Widgets OpenGL)
  if(MSVC)
    set_target_properties (${TD_QT_OPENGL_LIB} PROPERTIES COMPILE_FLAGS_REMOVE "/Zc:wchar_t-")
  endif(MSVC)
endif(QT5)

oda_qt_set_definition(${TD_QT_OPENGL_LIB} "TD_USE_QT_LIB")
oda_qt_set_cxxflags(${TD_QT_OPENGL_LIB} "")

tdrawing_project_group(${TD_QT_OPENGL_LIB} "Examples/Qt")
