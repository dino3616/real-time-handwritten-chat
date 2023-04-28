# Real-time Handwritten Chat - Information Network II by NITIC

Real-time handwritten chat program and its development environment.

## Setup

### environment

You can use [`Dev Containers`](https://github.com/microsoft/vscode-dev-containers) to setup the development environment.

Before start, you need to clone this repository and pre-setup by running the following command:

```bash
git clone "https://github.com/dino3616/real-time-handwritten-chat"
cd "real-time-handwritten-chat"
cp ".env.template" ".env"
```

You need to press `Shift` + `Cmd` (`Ctrl`) + `P`, and type `Dev Containers: Open Folder in Container...`. Then, select the root directory of this repository.
Now all you have to do is sit back and wait! (This process takes a very long time, so please be patient. 🍵)

### compile and run

Successed to build devcontainer, you can compile and run the program by running the following command:

```bash
make dev
```

### open the program

If there are no errors, you can open and view the program by [`Dev Containers desktop-light features`](https://github.com/microsoft/vscode-dev-containers/blob/main/script-library/docs/desktop-lite.md).
Let's go step by step.

First step, you need to access to `http://localhost:6080` in your browser. This port is specified in `.devcontainer/devcontainer.json > features > ghcr.io/devcontainers/features/desktop-lite > webPort`.  
Then, you need to enter the password `vscode` to login. This password is also specified in `.devcontainer/devcontainer.json > features > ghcr.io/devcontainers/features/desktop-lite > password`.  
After that, you can see your program that is running in your browser! 🥳

## Scripts

This repository provides the following scripts to help your development.

|   Command    | Description                                         |
| :----------: | :-------------------------------------------------- |
| `make build` | Compile the program.                                |
|  `make dev`  | Compile and run the program with a new binary file. |
| `make start` | Run the program with a exist binary file.           |
| `make clean` | Delete the build files.                             |
