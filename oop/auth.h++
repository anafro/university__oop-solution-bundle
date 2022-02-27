/*
 * -----------------------------------------------
 * Authentication
 * -----------------------------------------------
 * Provides an interface for login and register
 *
 * @since OOP 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include "logger.h++"
#include "csv.h++"
#include "dictionary.h++"
#include <bcrypt.h>
#include <regex>

namespace PROJECT_NAME {
    class auth final {
        std::string user_database_filename = "./users.csv";
        int min_password_length = 12;
        int min_username_length = 4;
        int sign_in_password_enter_attempts = 3;
        std::vector<std::pair<std::string, std::string>> password_validation_rules;
        ConsoleLogger logger;
    protected:
        auto load_existing_users() {
            return CSV().parse_file(user_database_filename, true);
        }

        void save_users(const std::vector<std::vector<std::string>>& user_database) {
            CSV().export_file(user_database_filename, user_database);
        }
    public:
        auth() {
            password_validation_rules.emplace_back("Should contain at least 1 capital latin character", "[A-Z]+");
            password_validation_rules.emplace_back("Should contain at least 1 lower-cased latin character", "[a-z]+");
            password_validation_rules.emplace_back("Should contain at least 1 digit", "[0-9]+");
            password_validation_rules.emplace_back("Should contain at least 1 special symbol (@!?#$%^&^*._())", "[@!?#$%^&^*._()]+");
        }

        void sign_in() {
            auto existing_users = load_existing_users();

            std::string user_name;

            while(true) {
                logger.log("Enter your username: ");
                std::getline(std::cin, user_name);

                if(std::all_of(existing_users.begin(), existing_users.end(), [&](const auto &existing_user) { return existing_user[0] != user_name; })) {
                    logger.error("Account with specified username does not exist.");
                    continue;
                } else {
                    break;
                }
            }

            std::vector<std::string> user_credits = *std::find_if(existing_users.begin(), existing_users.end(), [&](const auto& existing_user) {
                return existing_user[0] == user_name;
            });

            for(int attempts_left = sign_in_password_enter_attempts - 1; attempts_left >= 0; attempts_left--) {
                logger.log("Enter a password: ");

                std::string entered_password;
                std::getline(std::cin, entered_password);

                if(bcrypt::validatePassword(entered_password, user_credits[1])) {
                    logger.log("Access granted!");
                    break;
                } else {
                    if(attempts_left == 0) {
                        logger.error("Access denied - you entered password "s + std::to_string(sign_in_password_enter_attempts) + " wrongly!");
                    } else {
                        logger.error("Entered password is incorrect! You have "s + std::to_string(attempts_left) + " attempts left to enter the right one.");
                    }
                }
            }
        }

        void sign_up() {
            auto existing_users = load_existing_users();

            while(true) {
                logger.log("Enter a name for your brand-new account: ");

                std::string user_name;
                std::getline(std::cin, user_name);

                if(!existing_users.empty() && std::any_of(existing_users.begin(), existing_users.end(), [&](const auto& existing_user) { return existing_user[0] == user_name; })) {
                    logger.error("Unfortunately, somebody took this name. Please, choose something else");
                    continue;
                } else if(user_name.length() < min_username_length) {
                    logger.error("Your nickname should contain at least "s + std::to_string(min_username_length) + " characters, but you entered only "s + std::to_string(user_name.length()));
                    continue;
                }

                std::string user_password;

                while(true) {
                    logger.info("Enter a password: ");
                    std::getline(std::cin, user_password);

                    logger.info("Please, enter your password again: ");
                    std::string user_password_again;
                    std::getline(std::cin, user_password_again);

                    if(user_password != user_password_again) {
                        logger.error("Passwords are not the same!");
                    } else if(user_password.length() <= min_password_length) {
                        logger.error("Your password should contain at least "s + std::to_string(min_password_length) + " characters!");
                    } else {
                        bool validation_failed = false;
                        for(const auto& [validation_message, validation_regex] : password_validation_rules) {
                            if(!std::regex_search(user_password, std::regex(validation_regex))) {
                                logger.error("Your password does not follow the rule: "s + validation_message);
                                validation_failed = true;
                            }
                        }
                        if(validation_failed) {
                            logger.error("Some of the rules are not followed. They are listed above");
                        } else break;
                    }

                    logger.log("Please, enter your passwords again. Ensure that they are following the rules and you will be able to remember it!");
                }
                logger.log("Successfully signed up!");
                existing_users.push_back(std::vector<std::string> {user_name, bcrypt::generateHash(user_password) });
                save_users(existing_users);
                break;
            }
        }
    };
}
