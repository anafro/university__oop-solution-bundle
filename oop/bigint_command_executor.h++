/*
 * -----------------------------------------------
 * Big Integer Command Executor
 * -----------------------------------------------
 * This module executes a bunch of random commands
 * to a big integer
 *
 * @since 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include "bigint.h++"
#include "stack.h++"
#include "logger.h++"
#include "dictionary.h++"
#include <functional>
#include <utils/strings.h++>
#include <iostream>

#define register_command(NAME, OPERATOR) registered_commands.put(#NAME, [](auto& destination, const auto& operand) { \
destination OPERATOR ## = operand;\
})

namespace PROJECT_NAME {
    class bigint_command_executor {
        class bigint_command_executor_command {
            std::string operation;
            bigint value;
        public:
            bigint_command_executor_command(const std::string& raw_command) {
                auto entries = split_string(raw_command, " ");

                if(entries.size() != 2)
                    throw std::invalid_argument("String '" + raw_command + "' cannot be used as a big integer command executor command.");

                operation = entries[0];
                value = entries[1];
            }

            [[nodiscard]]
            const std::string& get_operation() const {
                return operation;
            }

            [[nodiscard]]
            const bigint& get_value() const {
                return value;
            }
        };

        bigint command_execution_result;
        ConsoleLogger logger;
        Dictionary<std::string, std::function<void(bigint&, const bigint&)>> registered_commands;
        Stack<bigint_command_executor_command> command_stack;
    public:
        bigint_command_executor() {
            register_command(ADD, +);
            register_command(SUB, -);
            register_command(MUL, *);
        }

        void push_command(const std::string& command) {
            command_stack.push({ command });
        }

        void push_random_command(unsigned int command_count = 1) {
            for(int i = 0; i < command_count; i++) {
                int command_index = rand() % registered_commands.size();
                command_stack.push({ registered_commands.get_keys()[command_index] + " " + bigint::random().to_string() });
            }
        }

        const bigint& run() {
            std::string initial_command_executor_value;
            logger.info("Please, enter an initial value for the big integer command executor: ");
            std::cin >> command_execution_result;

            int commands_executed_count = 0, commands_failed_count = 0;

            while(command_stack.has_elements()) {
                auto executing_command = command_stack.pop();
                if(registered_commands.has(executing_command.get_operation())) {
                    registered_commands[executing_command.get_operation()](command_execution_result, executing_command.get_value());
                    commands_executed_count++;
                } else {
                    std::string suggested_operation = registered_commands.get_keys()[0];

                    double maximal_similarity = 0;
                    for(const auto& existing_command_operation : registered_commands.get_keys())  {
                        if(similarity(executing_command.get_operation(), existing_command_operation) > maximal_similarity) {
                            suggested_operation = existing_command_operation;
                            maximal_similarity = similarity(executing_command.get_operation(), existing_command_operation);
                        }
                    }

                    logger.error("Operation "s + executing_command.get_operation() + " is not provided. Did you mean " + suggested_operation + "?");
                    commands_failed_count++;
                }
            }

            logger.info("Execution is completed ("s + std::to_string(commands_executed_count) + " commands executed successfully" + (commands_failed_count == 0 ? ""s : ", but "s + std::to_string(commands_failed_count) + " failed") + ")");
            logger.info("The result of execution: "s + command_execution_result.to_string());
            return command_execution_result;
        }

        const bigint& get_result() {
            return command_execution_result;
        }
    };
}
