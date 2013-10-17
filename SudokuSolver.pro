QT       += core gui widgets

TARGET    = SudokuSolver
TEMPLATE  = app

SOURCES  += main.cpp\
            mainwindow.cpp \
            solver.cpp \
            logger.cpp

HEADERS  += mainwindow.h \
            solver.h \
            common.h \
            decision.h \
            logger.h
