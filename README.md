# Real-time Handwritten Chat - Information Network II by NITIC

Real-time handwritten chat program and its development environment.

## Setup

### environment

You can use [`devcontainer`](https://github.com/microsoft/vscode-dev-containers) to setup the development environment.

Before start, you need to clone this repository and pre-setup by running the following command:

```bash
git clone "https://github.com/dino3616/real-time-handwritten-chat"
cd "real-time-handwritten-chat"
cp ".env.template" ".env"
```

You need to press `Shift` + `Cmd` (`Ctrl`) + `P`, and type `Dev Containers: Open Folder in Container...`. Then, select the root directory of this repository.
Now all you have to do is sit back and wait! (This process takes a very long time, so please be patient. 🍵)

### compile and run

Successed to build devcontainer, you can compile and run each program by running the following command:

#### `chat.c`

```bash
gcc ./src/chat.c -o ./bin/chat && ./bin/chat
```

#### `draw.c`

```bash
gcc ./src/draw.c -o ./bin/draw -lX11 && ./bin/draw
```
