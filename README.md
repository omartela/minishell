# Minishell

**Minishell** is a minimalistic shell project developed as part of the 42 curriculum. It replicates essential features of a Unix-like shell, providing hands-on experience with system programming concepts like process management, piping, and signal handling.

---

## Project Details

- **Total Time Spent**: 68 days
- **Total Lines of Code**: 6214
- **Commits**: 477
- **Programming Language**: C

The team worked closely together to build a fully functional shell, overcoming challenges and continuously refining the architecture.

Created by [Omartela](https://github.com/omartela) and [RychkovIurii](https://github.com/RychkovIurii).

---

## Table of Contents

0. [Project Details](#project-details)
1. [Features](#features)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Project Requirements](#project-requirements)
5. [Project Architecture](#project-architecture)
6. [Challenges and Contributions](#challenges-and-contributions)
7. [Acknowledgments](#acknowledgments)

---

## Features

- **Built-in Commands**: Implements:
  - `echo`, `cd`, `pwd`, `export`, `unset`, `env`, and `exit`.
- **Command Execution**: Executes system commands and binaries located in the system's `PATH`.
- **Environment Variables**: Supports dynamic management of environment variables.
- **Pipelines and Redirections**:
  - Handles pipes (`|`) for command chaining.
  - Supports input/output redirection (`<`, `>`, `>>`).
  - Implements heredoc (`<<`) functionality.
- **Signal Handling**: Gracefully handles `Ctrl+C`, `Ctrl+D`, and `Ctrl+\`.

---

## Installation

1. Clone the repository:
	```bash
	git clone https://github.com/omartela/minishell.git
	cd minishell
	```

2. Compile the project:
	```bash
	make
	```

3. Run the shell:
	```bash
	./minishell
	```

---

## Usage

Launch Minishell by running:
```bash
./minishell
```

#### Examples of supported commands:

Standard commands: `ls`, `grep`, etc.

Built-ins:
```bash
export MY_VAR="value"
echo $MY_VAR
> value
unset MY_VAR

export MY_VAR="value"
export MY_VAR+="thisisappended"
echo $MY_VAR
> valuethisisappended
unset MY_VAR

export MY_VAR1="value" MY_VAR2="value2"
echo $MYVAR1 $MYVAR2
>value value2
unset MY_VAR1 MY_VAR2

echo $?
> 0
"exit code of previous command"

echo HELLO WORLD
> HELLO WORLD
echo $_
> WORLD
"$_ is previous command or last argument"

echo $SHLVL
> 1
./minishell
echo $SHLVL
> 2

export $MY_VAR=firstlevel
echo $MY_VAR
> firstlevel
./minishell
echo $MY_VAR
> firstlevel


```

Redirections and pipelines:
```bash
cat file.txt | grep "text" > result.txt

ls |
cat
"output will result of ls command. Minishell handles open pipe."

export append=">>"
echo hi > outfile.txt
echo there $append outfile.txt
cat outfile
> hi
> there
"in our minishell redirections are expandable, bash does not handle it"
```

Quotes:
```bash
echo "hello
world"
> hello
> world

export MY_VAR=value
echo '$MY_VAR'
>$MY_VAR

export MY_VAR=value
echo "$MY_VAR"
>value

export MY_VAR=value
echo "'$MY_VAR'"
>value

export MY_VAR=value
echo '"$MY_VAR"'
>"$MY_VAR"
```

Heredoc:
```bash
cat << EOF
This is a heredoc example.
EOF
```


To exit, type `exit` or press `Ctrl+D`


---

## Project Requirements

This project adheres to the following constraints:

Must be written in C.
Follow the Norm coding standard.
Only allowed to use the following functions:
**read, write, malloc, free, exit, fork, wait, execve, open, close, signal, kill, stat, lstat, fstat, pipe, dup, dup2, opendir, readdir, closedir, strerror, and perror**.
No external libraries are permitted.

---

## Project Architecture
```
minishell/
├── docs/						# Documentation and test functions
├── include/					# Header file
├── libft/						# My library
├── src/						# Source code files
├── Makefile					# Build script
├── README.md					# Project documentation
├── script_for_fds.sh			# Test script for fds
└── script-run-all-mand.sh		# Test script for leaks and fds
```

---

## Challenges and Contributions

**Challenges and Contributions**

During the development of Minishell, we encountered several challenges and also made key decisions regarding design and implementation. Some notable points include:
Execution of Built-ins

We initially designed our built-ins to be executed in the parent process to ensure that we could modify memory directly. However, we realized that child processes, while ideal for command execution, could not affect the parent process's memory due to the process isolation in Unix. This limitation led us to build a mechanism where we execute built-ins in the parent process but allow for execution within a child process when needed.

EOF Signal to Stop readline by Closing File Descriptor:
Handling the EOF signal in Minishell required implementing a mechanism to gracefully stop the readline function. We achieved this by closing the file descriptor, ensuring proper termination without unexpected behavior.

Signal handling was dynamically adapted depending on whether the process was the parent or a child. This differentiation ensured robust handling of interruptions and termination signals, aligning with expected shell behavior.

Proper Return Values from minishell:
Ensuring that signals terminated processes with the correct return values was critical for compatibility with Unix conventions. This required mapping termination causes to appropriate exit codes, providing consistent feedback to the user and other processes.


**Collaborative Contributions**

Omartela developed a robust parser for environment variable expansion ($), built-in commands, and implemented advanced features such as export and env. His work was foundational to the success of the shell's built-ins.

Omartela in collaboration with RychkovIurii also worked on implementing signal handling and memory management, ensuring the shell could handle signals like Ctrl+C, Ctrl+D, and Ctrl+\ gracefully and that memory was efficiently managed.

RychkovIurii focused on pipes, parsing, redirections, heredocs, execution, and file descriptor management (fds). His contributions ensured smooth handling of multiple commands and their interaction, such as redirecting inputs and outputs, managing pipe flows, and handling heredoc inputs.

---

## Acknowledgments

Acknowledgments
This project is part of the 42 School curriculum. Special thanks to:

The 42 community for support and insights.
My peers for helpful discussions and feedback.

Written by [Omartela](https://github.com/omartela) and [RychkovIurii](https://github.com/RychkovIurii).

