#include "Logger.h"

Logger::Loggers &Logger::Loggers::getInstance() {
    static Loggers instance;
    return instance;
}
