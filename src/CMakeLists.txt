#include_directories(
#    "${CMAKE_SOURCE_DIR}/src/common"
#    "${CMAKE_SOURCE_DIR}/src/auth"
#)
#include_directories("${CMAKE_BINARY_DIR}/src/common")

set(IMWRITER_SOURCES
    main.cpp
    mainwindow.cpp
    imwriter.cpp
    devicemodel.cpp
    imagelistmodel.cpp
    image.cpp
    job.cpp
    copylocalfilejob.cpp
    bucket.cpp
    bucketstream.cpp
    bucketfactory.cpp
    worker.cpp
    reader.cpp
    writer.cpp
    functions.cpp
    checksumchecker.cpp
    downloader.cpp
    copyremoteimagejob.cpp
    copyimagejob.cpp
    lazyloader.cpp
    imagerepository.cpp
)

set(MTEST_SOURCES
    mtest.cpp
    )

add_executable(imwriter ${IMWRITER_SOURCES})
target_link_libraries(imwriter 
   KF5::XmlGui 
   KF5::Solid
   KF5::I18n
   KF5::Gpgmepp
   KF5::QGpgme
   crypto
   )

add_executable(mtest ${MTEST_SOURCES})
target_link_libraries(mtest
   KF5::XmlGui 
   KF5::Solid
   KF5::I18n)

qt5_use_modules(imwriter DBus Network Qml)
qt5_use_modules(mtest DBus Network Qml)
install(TARGETS imwriter DESTINATION "${CMAKE_INSTALL_BINDIR}")

install(FILES imwriterui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/imwriter)
install(FILES imagerepository.xml DESTINATION  ${KDE_INSTALL_DATADIR}/imwriter)

#install(DIRECTORY "icons" DESTINATION "${DATA_INSTALL_DIR}")

add_subdirectory(icons)
