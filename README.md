# Flow

_Flow_ is a TUI brainstorming note application. It is a simple text input application that encourages pushing many short-formed notes many times.

Upon launching the program, a 'session' is created. All notes created during this session will be located in a new folder pertaining to that session. Each note's filename will be the timestamp of when it was pushed.

## Usage
- `Tab` to push current note to session and create a new one
- `Ctrl-C` to quit

## Config
```$XDG_CONFIG_HOME/flow/config.toml
path = "~/flow"
```

`path` determines where the session folders (and notes) will be written to.

## Setting Up Project
Clone all required third party libraries:
```sh
make setup # Sets up project by doing git submodule stuff
make # build and run project
```
