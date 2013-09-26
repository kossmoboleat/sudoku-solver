QT       += core gui widgets

TARGET    = SudokuSolver
TEMPLATE  = app

SOURCES  += main.cpp\
            mainwindow.cpp \
            solver.cpp

HEADERS  += mainwindow.h \
            solver.h \
            common.h \
            decision.h

FORMS    += mainwindow.ui
