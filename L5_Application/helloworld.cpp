/*
 * helloworld.cpp
 *
 *  Created on: Sep 4, 2015
 *      Author: Bhargava
 */
#include "command_handler.hpp"
#include "string.h"


CMD_HANDLER_FUNC(cowsay)
{
    /* cmdParams is a str passed to you after user's command.
     * If command was "newcmd test 123" then cmdParams will be "test 123".
     *
     * output is a CharDev class where the command came from, so
     * we can use this to output a reply message.
     * See "handlers.cpp" for more examples
     */

        output.printf("| %s |\n", cmdParams()); //Print the parameters received along with the command;
        //Print the rest of the cow.
        output.printf("  \\ ^__^\n");
        output.printf("    (oo)\\_______\n");
        output.printf("    (__)\\       )\\/\\\n");
        output.printf("        ||----w |\n");
        output.printf("        ||     ||\n");

    return true; /* return true if command was successful */
}

