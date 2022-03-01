#include <bigint_command_executor.h++>
#include "auth.h++"

using namespace PROJECT_NAME;

int main() {
    ConsoleLogger logger;

    // Command Executor
    logger.info("-- Big Integer Command Executor --");
    bigint_command_executor executor;

    // Adding commands with typos
    executor.push_command("SUBSTRACT 2934923949");
    executor.push_command("MULTYPLY 2934923949");
    executor.push_command("ADDD 2934923949");

    // Adding valid commands
    executor.push_command("SUBTRACT 2934923949");
    executor.push_command("MULTIPLY 2934923949");
    executor.push_command("ADD 2934923949");


    executor.push_random_command(20);
    executor.run();


    // Auth Gate
    logger.info("");
    logger.info("-- Auth Gate --");
    auth service;
    service.sign_up();
    service.sign_in();
}
