# Minishell

**Minishell** is a minimalistic shell project developed as part of the 42 curriculum. It replicates essential features of a Unix-like shell, providing hands-on experience with system programming concepts like process management, piping, and signal handling.

---

## Table of Contents

1. [Features](#features)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Project Requirements](#project-requirements)
5. [Project Architecture](#project-architecture)
6. [Acknowledgments](#acknowledgments)
7. [License](#license)

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

Standard commands: **ls, grep**, etc.

Built-ins:
```bash
export MY_VAR="value"
echo $MY_VAR
unset MY_VAR
```

Redirections and pipelines:
```bash
cat file.txt | grep "text" > result.txt
```

Heredoc:
```bash
cat << EOF
This is a heredoc example.
EOF
```

To exit, type **exit** or press **Ctrl+D**


---

## Project Requirements

This project adheres to the following constraints:

Must be written in C.
Follow the Norm coding standard.
Only allowed to use the following functions:
read, write, malloc, free, exit, fork, wait, execve, open, close, signal, kill, stat, lstat, fstat, pipe, dup, dup2, opendir, readdir, closedir, strerror, and perror.
No external libraries are permitted.

---

## Project Architecture
```
minishell/
├── docs/						# Documentation and test functions
├── include/					# Header files
├── libft/						# My library
├── src/						# Source code files
├── Makefile					# Build script
├── README.md					# Project documentation
├── script_for_fds.sh			# Test script for fds
└── script-run-all-mand.sh		# Test script for leaks and fds
```

---

## Acknowledgments

Acknowledgments
This project is part of the 42 School curriculum. Special thanks to:

The 42 community for support and insights.
My peers for helpful discussions and feedback.

Written by omartela and irychkov.
