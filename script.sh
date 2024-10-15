#!/bin/bash

# Path to your minishell executable
MINISHELL_PATH="./minishell"

# Check if minishell exists and is executable
if [ ! -x "$MINISHELL_PATH" ]; then
    echo "Minishell executable not found or not executable at $MINISHELL_PATH"
    exit 1
fi

# Path to the script containing commands
COMMANDS_FILE=./42_minishell_tester/cmds/mand/1_builtins.sh

# Check if the command file exists
if [ ! -f "$COMMANDS_FILE" ]; then
    echo "Command file not found at $COMMANDS_FILE"
    exit 1
fi

# Read each command from 1_builtins.sh and execute it with Valgrind
while IFS= read -r command; do
    # Skip empty lines and comments
    [[ -z "$command" || "$command" =~ ^# ]] && continue

    echo "Running command: $command"

    # Run Valgrind with the command piped to minishell
    valgrind --leak-check=full --trace-children=yes --gen-suppressions=all --show-leak-kinds=all \
        --suppressions=vg.supp "$MINISHELL_PATH" <<< "$command"

done < "$COMMANDS_FILE"
