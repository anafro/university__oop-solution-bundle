#include <logger.h++>

namespace PROJECT_NAME {
    MessageFormatter::MessageFormatter(std::string message_format) : message_format(std::move(message_format)) {
        //
    }

    std::string MessageFormatter::format(const std::string& message, LogLevel& level) const {
        std::string formatted_message = message_format;

        if(formatted_message.find(TIME_REPLACING_TAG) != std::string::npos) {
            std::stringstream stringed_time;
            auto timestamp = std::time(nullptr);
            auto time_object = std::localtime(&timestamp);
            stringed_time << std::put_time(time_object, "%H:%M:%S");
            formatted_message.replace(formatted_message.find(TIME_REPLACING_TAG), TIME_REPLACING_TAG.length(), stringed_time.str());
        }

        if(formatted_message.find(LEVEL_REPLACING_TAG) != std::string::npos) {
            formatted_message.replace(formatted_message.find(LEVEL_REPLACING_TAG), LEVEL_REPLACING_TAG.length(), level.get_tag());
        }

        if(formatted_message.find(MESSAGE_REPLACING_TAG) != std::string::npos) {
            formatted_message.replace(formatted_message.find(MESSAGE_REPLACING_TAG), MESSAGE_REPLACING_TAG.length(), message);
        }

        return formatted_message + '\n';
    }

    AbstractLogger::AbstractLogger(MessageFormatter formatter) : formatter(std::move(formatter)) {
        //
    }

    void AbstractLogger::info(const std::string& message) {
        log(message, INFO);
    }

    void AbstractLogger::warning(const std::string& message) {
        log(message, WARNING);
    }

    void AbstractLogger::error(const std::string& message) {
        log(message, ERROR);
    }

    ConsoleLogger::ConsoleLogger(MessageFormatter formatter) : AbstractLogger(std::move(formatter)) {
        //
    }

    void ConsoleLogger::log(const std::string &message, LogLevel level) {
        std::cout << formatter.format(message, level);
    };

    FileLogger::FileLogger(const std::string& logger_filename, MessageFormatter formatter) : AbstractLogger(std::move(formatter)) {
        file_output_stream = std::ofstream(logger_filename);
    }

    void FileLogger::log(const std::string &message, LogLevel level) {
        file_output_stream << formatter.format(message, level);
    };

    DoubleLogger::DoubleLogger(const MessageFormatter& formatter, const std::string& logger_filename) {
        if(!logger_filename.empty())
            file_logger = std::make_unique<FileLogger>(logger_filename, formatter);

        console_logger = std::make_unique<ConsoleLogger>(formatter);
    }

    void DoubleLogger::log(const std::string &message, LogLevel level) {
        if(file_logger)
            file_logger->log(message, level);

        if(console_logger)
            console_logger->log(message, level);
    }

    FileLogger LoggerFactory::create_file_logger(const std::string& logger_filename, const MessageFormatter& formatter) {
        return FileLogger(logger_filename, formatter);
    }

    ConsoleLogger LoggerFactory::create_console_logger(const MessageFormatter& formatter) {
        return ConsoleLogger(formatter);
    }

    DoubleLogger LoggerFactory::create_double_logger(const MessageFormatter& formatter, const std::string& logger_filename) {
        return DoubleLogger(formatter, logger_filename);
    }
}