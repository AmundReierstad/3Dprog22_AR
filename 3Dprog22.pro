QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17

TARGET      = 3D-programmering

SOURCES += main.cpp \
    camera.cpp \
    cube.cpp \
    disc.cpp \
    door.cpp \
    graph.cpp \
    gridworld.cpp \
    ground_oblig3.cpp \
    heightmapsurface.cpp \
    house.cpp \
    interactiveobject.cpp \
    light.cpp \
    logger.cpp \
    mainwindow.cpp \
    minkvadrat.cpp \
    octahedronball.cpp \
    pickup.cpp \
    quadtree.cpp \
    renderwindow.cpp \
    shader.cpp \
    stb_image.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

HEADERS += \
    camera.h \
    cube.h \
    disc.h \
    door.h \
    graph.h \
    gridworld.h \
    ground_oblig3.h \
    heightmapsurface.h \
    house.h \
    interactiveobject.h \
    light.h \
    logger.h \
    mainwindow.h \
    minkvadrat.h \
    octahedronball.h \
    pickup.h \
    quadtree.h \
    renderwindow.h \
    shader.h \
    stb_image.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h \
    xyz.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    PhongFragmentShader.frag \
    PhongVertexShader.vert \
    ReadMe \
    TriangleSurfaceFile.txt \
    plainshader.frag \
    plainshader.vert
