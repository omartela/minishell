#!/bin/bash

# Path to your minishell executable
MINISHELL_PATH="./minishell"

# Check if minishell exists and is executable
if [ ! -x "$MINISHELL_PATH" ]; then
    echo "Minishell executable not found or not executable at $MINISHELL_PATH"
    exit 1
fi

# Path to the script containing commands
COMMANDS_FILE=~/42_minishell_tester/cmds/mand/1_builtins.sh

# Check if the command file exists
if [ ! -f "$COMMANDS_FILE" ]; then
    echo "Command file not found at $COMMANDS_FILE"
    exit 1
fi

# Track commands with leaks
LEAKY_COMMANDS=()

# Read each command from 1_builtins.sh and execute it with Valgrind
while IFS= read -r command; do
    # Skip empty lines and comments
    [[ -z "$command" || "$command" =~ ^# ]] && continue

    echo "Running command: $command"
	TEMP_OUTPUT=$(mktemp)
    # Run Valgrind with the command piped to minishell and capture the output
    valgrind --leak-check=full --trace-children=yes --gen-suppressions=all --show-leak-kinds=all \
        --suppressions=vg.supp "$MINISHELL_PATH" <<< "$command" &> "$TEMP_OUTPUT"

    # Check the output for signs of memory leaks
	if grep -Eq "definitely lost: [1-9]|indirectly lost: [1-9]|possibly lost: [1-9]|still reachable: [1-9]" "$TEMP_OUTPUT"; then
		echo "Leak detected for command: $command"
		LEAKY_COMMANDS+=("$command")
		cat $TEMP_OUTPUT
		rm "$TEMP_OUTPUT"
    fi

done < "$COMMANDS_FILE"

# Display all commands with leaks at the end
if [ ${#LEAKY_COMMANDS[@]} -gt 0 ]; then
    echo -e "\nCommands with memory leaks:"
    for cmd in "${LEAKY_COMMANDS[@]}"; do
        echo "$cmd"
    done
else
    echo -e "\nNo memory leaks detected in any commands."
fi

