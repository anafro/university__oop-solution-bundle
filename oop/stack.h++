/*
 * -----------------------------------------------
 * Stack
 * -----------------------------------------------
 * The stack is a FIFO container.
 *
 * @since OOP 1.0.0.0
 * @author Anatoly Frolov - contact@anafro.ru
 */

#pragma once

#include <stdexcept>

namespace PROJECT_NAME {
    template<typename T>
    class Stack {
        class Node {
            Node* next;
            T value;
        public:
            explicit Node(T initial_value, Node* initial_next) : value(initial_value), next(initial_next) {
                //
            }

            Node* get_next() {
                return next;
            }

            T& get_value() {
                return value;
            }
        };
        Node* head = nullptr;
    public:
        Stack() = default;

        void push(T value) {
            head = new Node(value, head);
        }

        T pop() {
            if(!has_elements())
                throw std::runtime_error("Cannot pop an element from an empty stack.");

            T stack_top = top();
            head = head->get_next();

            return stack_top;
        }

        T top() {
            return head->get_value();
        }

        bool has_elements() {
            return head != nullptr;
        }
    };
}