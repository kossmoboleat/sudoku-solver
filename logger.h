#pragma once

#include <iostream>
#include <QString>

// A singleton that decides how log messages are handled.
class Logger
{
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log( QString const &rMessage ) {
        #ifndef QT_NO_DEBUG
            std::cerr << rMessage.toStdString() << std::endl;
        #endif
    }

private:
    Logger();
};
