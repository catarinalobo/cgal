# Created by the script cgal_create_cmake_script
# This is the CMake script for compiling a CGAL application.


project( Generator_Demo )

cmake_minimum_required( VERSION 2.8.10 )

set( CMAKE_CXX_STANDARD 14 )

find_package( CGAL QUIET COMPONENTS Core )

find_package( Qt4 QUIET )

if ( CGAL_FOUND AND QT4_FOUND )

  include( ${CGAL_USE_FILE} )

  include( ${QT_USE_FILE} )

  include_directories( BEFORE "../../include" )

  qt4_wrap_cpp( MainWindow_moc MainWindow.h )

  add_executable( ogons_generators_2 ogons_generators_2.cpp ${MainWindow_moc} )

  target_link_libraries( ogons_generators_2 ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )

  target_link_libraries( ogons_generators_2 ${QT_LIBRARIES} )

else()
  
    message( STATUS "This program requires the CGAL and Qt4 libraries, and will not be compiled." )
  
endif()

