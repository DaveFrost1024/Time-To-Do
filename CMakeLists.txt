cmake_minimum_required (VERSION 3.1...3.13)

project(Time-To-Do)

find_package(PkgConfig)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0) # look into FindPkgConfig.cmake,
pkg_check_modules(GLIB REQUIRED glib-2.0)

add_executable(Time-To-Do
    TimetdApplication.h
    TimetdAppwindow.h
    TimetdAppprefs.h
    main.cc
    TimetdApplication.cc
    TimetdAppwindow.cc
    TimetdAppprefs.cc
    timetodo.gresource.xml
    org.gtkmm.timetodo.gschema.xml
    app_menu.glade
    window.glade
    prefs.glade
    timeToDo.css
    resources.c
    Time-To-Do.desktop)

execute_process (
    COMMAND bash -c "../setup.sh")

target_include_directories(Time-To-Do PUBLIC
    ${GTKMM_INCLUDE_DIRS}
    ${GLIB_INCLUDE_DIRS})

target_link_libraries(Time-To-Do PUBLIC
    ${GTKMM_LIBRARIES}
    ${GLIB_LIBRARIES})
