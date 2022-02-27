# The Logging Module
#   This module is used for printing messages while
#   building stage.


function(raw_log status text_message)
    message(${status} "[${LOGGER_PREFIX}, ${status}] ${text_message}")
endfunction()

function(log text_message)
    raw_log(STATUS ${text_message})
endfunction()

function(warning text_message)
    raw_log(WARNING ${text_message})
endfunction()

function(error text_message)
    raw_log(FATAL_ERROR ${text_message})
endfunction()