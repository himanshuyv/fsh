# Description

## fsh
fsh, a shell written in C.

## Requirements
`C` and `make` are required to build fsh.

## Installation instructions
1. This repository should be cloned.
2. `cd` into the cloned repository.
3. Run `make`
4. Run the shell with `./fsh`

# Assumptions
1. Input to the shell does not exceed 2048 bytes.
2. Maximum of 32 commands per line of input.
3. Maximum of 128 arguments for a command.
4. Username and sysname do not exceed 64 characters.
5. Paths to files and directories do not exceed 4096 characters.
6. Any line of input containing any `pastevents` command are not stored in `pastevents` history.
