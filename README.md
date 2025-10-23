# Minishell

## 🧠 Objective
This project is part of the 42 core curriculum and aims to recreate a simplified version of a Unix shell using low-level system calls in C.  
It reproduces essential shell behavior (parsing commands, handling pipes, redirections, environment variables, and signals) while maintaining stability and correct memory management under all conditions.

Developed entirely solo, this Minishell was built from scratch with a focus on **process control, memory safety, and clean architecture**, while respecting all 42 project restrictions (no external parsing or memory libraries).

---

## ⚙️ Features
- **Command parsing** supporting arguments, quotes, and escape sequences  
- **Environment variable expansion** with `$VAR` syntax and special handling of `$?`  
- **Built-in commands**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`  
- **Redirections**: `<`, `>`, `>>`, and `<<` (heredoc with signal handling)  
- **Pipes**: support for multiple chained commands (`cmd1 | cmd2 | cmd3`)  
- **Signal management**: real-time handling of `SIGINT` and `SIGQUIT`, consistent with bash behavior  
- **Error handling**: precise messages for syntax errors, permission issues, and command not found  
- **Memory safety**: no leaks or dangling pointers (validated through testing with `valgrind` and `leaks` for MAC)  
- **Robust architecture**: modular components separating parsing, execution, and built-in logic  

---

## 🧩 Project Structure

```bash
src/
├── parser/             # Recursive parser that builds syntax trees
├── lexer/              # Tokenization of raw command input
├── exec/               # Command execution layer
│   ├── builtins/       # Built-in command implementations (cd, echo, export...)
│   ├── external_program/ # Handling of external binaries via execve
├── expandvar/          # Environment variable expansion logic
├── signal/             # Interactive signal behavior and terminal control
└── main/               # Entry point, initialization, and shared utilities
```

The project was designed to keep clear separation between **syntax interpretation** and **command execution**, allowing easy debugging and logical flow.

---

## 🚀 Compilation

Make targets:

- make — builds the binary
- make clean — removes object files
- make fclean — removes all binaries and object files
- make re — rebuilds everything from scratch

---

## 💻 Usage

Run the shell:
```bash
./minishell
minishell$ echo "Hello, $USER"
minishell$ ls -l | grep minishell | wc -l
minishell$ cat < infile | grep foo > outfile
minishell$ export VAR=test && echo $VAR
minishell$ ./script.sh
minishell$ exit
```
---

## 🧠 Challenges Faced

- **Parsing complexity:** managing nested quotes, mixed redirections, and variable expansion while preserving syntax correctness.
- **Process synchronization:** ensuring that each child process inherits the correct environment, file descriptors, and exit status.
- **Signal consistency:** reproducing bash-like interactive behavior, including correct handling of Ctrl-C and Ctrl-\.
- **Memory management:** avoiding leaks across dynamic parsing trees and environment variable expansions.
- **Error recovery:** handling invalid syntax and failed commands gracefully without breaking the shell state.

---

## ✅ Key Learnings & Solutions

- **Parser architecture:** designed a recursive parsing strategy with a clean tokenization layer to handle multiple edge cases in command interpretation.
- **Execution flow:** implemented a well-defined fork/execve model with controlled pipe duplication (dup2) and closure of unused descriptors.
- **Environment variables** are stored in an array of arrays structure (e.g., char **envp) to allow fast lookup, modification, and export
- **Heredoc handling:** created a signal-safe heredoc routine that respects interrupt signals (SIGINT) without corrupting global state.
- **Code organization:** modularized every major subsystem, ensuring reusability and clarity, even under tight 42 project constraints.

---

## 🧩 Technical Focus

- Process management (fork, execve, pipe, dup2, waitpid)
- Signal handling (sigaction, readline integration)
- Error propagation and exit codes consistent with bash

---

## 🔗 Related 42 Projects

- Libft — foundational library used for memory and string handling
- get_next_line — used as inspiration for input buffering logic
- pipex — first introduction to Unix pipes and process chaining

---

This Minishell was developed entirely solo, with full ownership of architecture, debugging, and design decisions.
Beyond meeting the project’s requirements, the goal was to understand how a shell truly works internally, how processes communicate, how input is interpreted, and how to manage complex state in a constrained C environment.
