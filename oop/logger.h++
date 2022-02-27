/*
 * -----------------------------------------------
 * Logger
 * -----------------------------------------------
 * Logging is useful to display technical information
 * about runtime - what is going on, what errors are
 * occurred and so on.
 *
 * @since 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include <sstream>
#include <iostream>
#include <ostream>
#include <string>
#include <utility>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <stdexcept>

#define RegisterLogLevel(NAME) const LogLevel NAME(#NAME)

using namespace std::string_literals;

namespace PROJECT_NAME {
    class LogLevel final {
        std::string tag;
    public:
        LogLevel(std::string tag) : tag(std::move(tag)) {
            //
        }

        [[nodiscard]]
        const std::string& get_tag() {
            return tag;
        }
    };

    RegisterLogLevel(INFO);
    RegisterLogLevel(WARNING);
    RegisterLogLevel(ERROR);

    class MessageFormatter {
        const std::string TIME_REPLACING_TAG = "%time";
        const std::string LEVEL_REPLACING_TAG = "%level";
        const std::string MESSAGE_REPLACING_TAG = "%message";
        std::string message_format;
    public:
        /**
         * Creates a new message formatter with passed format
         *
         * @see TIME_REPLACING_TAG, LEVEL_REPLACING_TAG, MESSAGE_REPLACING_TAG
         * @param message_format
         */
        explicit MessageFormatter(std::string message_format);

        /**
         * Formats the message.
         *
         * @param message Message that should be formatted
         * @param level The level of logging
         * @return The formatted message
         */
        std::string format(const std::string& message, LogLevel& level) const;
    };

    [[maybe_unused]]
    const MessageFormatter DEFAULT_FORMATTER("[%time]:  [%level] %message"),
                           KEEP_ONLY_MESSAGE_FORMATTER("%message"),
                           MESSAGE_AND_LOG_LEVEL_FORMATTER("[%level]: %message");

    class AbstractLogger {
    protected:
        const MessageFormatter formatter;
    public:
        /**
         * Initializes a base of loggers.
         *
         * @param formatter The logger message formatter
         */
        AbstractLogger(MessageFormatter formatter = DEFAULT_FORMATTER);

        /**
         * Logs the message.
         *
         * @param message The message
         * @param level The log level
         */
        virtual void log(const std::string& message, LogLevel level = INFO) = 0;

        /**
         * Logs an error. This is an equivalent of log(message, INFO).
         *
         * @param message The message
         */
        virtual void info(const std::string& message);

        /**
         * Logs an error. This is an equivalent of log(message, WARNING).
         *
         * @param message The message
         */
        virtual void warning(const std::string& message);

        /**
         * Logs an error. This is an equivalent of log(message, ERROR).
         *
         * @param message The message
         */
        virtual void error(const std::string& message);
    };

    class ConsoleLogger : public AbstractLogger {
    public:
        /**
         * Creates a new console logger with a message formatter.
         *
         * @param formatter The logger message formatter
         */
        explicit ConsoleLogger(MessageFormatter formatter = DEFAULT_FORMATTER);

        /**
         * Logs a message to a console.
         *
         * @param message The message
         * @param level The log level
         */
        void log(const std::string &message, LogLevel level = INFO) override;
    };

    class FileLogger : public AbstractLogger {
        std::ofstream file_output_stream;
    public:
        /**
         * Creates a new file logger.
         *
         * @param logger_filename The file where logs will be written to
         * @param formatter The logger message formatter
         */
        explicit FileLogger(const std::string& logger_filename, MessageFormatter formatter = DEFAULT_FORMATTER);

        /**
         * Logs message to a file.
         *
         * @param message The message
         * @param level The log level
         */
        void log(const std::string &message, LogLevel level = INFO) override;
    };

    class DoubleLogger : AbstractLogger {
        std::unique_ptr<FileLogger> file_logger = nullptr;
        std::unique_ptr<ConsoleLogger> console_logger = nullptr;
    public:
        /**
         * Creates a new double logger, which logs to the file and to the console both.
         *
         * @param formatter The log message formatter
         * @param logger_filename The file where the logs will be written to
         */
        explicit DoubleLogger(const MessageFormatter& formatter = DEFAULT_FORMATTER, const std::string& logger_filename = "");

        /**
         * Logs to the file and to the console both.
         *
         * @param message The message
         * @param level The log level
         */
        void log(const std::string &message, LogLevel level = INFO) override;
    };

    class LoggerFactory final {
        //
    public:
        /**
         * Creates a new file logger.
         *
         * @param logger_filename The file where logs will be written to
         * @param formatter The logger message formatter
         */
        static FileLogger create_file_logger(const std::string& logger_filename, const MessageFormatter& formatter = DEFAULT_FORMATTER);

        /**
         * Creates a new console logger with a message formatter.
         *
         * @param formatter The logger message formatter
         */
        static ConsoleLogger create_console_logger(const MessageFormatter& formatter = DEFAULT_FORMATTER);

        /**
         * Creates a new double logger, which logs to the file and to the console both.
         *
         * @param formatter The log message formatter
         * @param logger_filename The file where the logs will be written to
         */
        static DoubleLogger create_double_logger(const MessageFormatter& formatter = DEFAULT_FORMATTER, const std::string& logger_filename = "");
    };
}