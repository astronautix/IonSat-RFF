# Introduction

This C++ project is part of the Astronautix IonSat project, which aims to develop an experimental 3U cubesat platform for an electric propulsion payload.

![Polytechnique logo](doc/logo.png) ![CSEP logo](doc/logo_csep.png)

This repository hosts the RFF (Realtime Flight Framework), developped for french Ecole Polytechnique's student space center and its cubesat IonSat. It was created as a framework to help with realtime embeddedded development by wrapping the RTOS and a custom event-driven library into a simple C++ library. It is powered by the Meson build system generator for ease of use. 


## Getting started

The full documentation (tutorials, reference documentation, examples and design philosophy) can be found as Markdown files in the *doc* library. As a suggested learning path, you should start by looking at the tutorials and a few example project before delving in the design philosophy and full reference documentation. 


## Tutorials

1) [Basics](doc/tutorials/basics.md)
2) [Compilation and cross-compilation](doc/tutorials/compilation.md)
3) [Real-time OS wrapper](doc/tutorials/RTOS.md)


## Examples

1) [Hello world !](doc/examples/hello/)
2) [Blinker](doc/examples/blinker/)
3) [Synchronization](doc/examples/synchronization/)
4) [Cross-compilation](doc/examples/cross-compilations/)

## Design

1) [Architecture](doc/design/architecture.md)
2) [RTOS wrapper](doc/design/RTOS.md)
3) [Event handlers](doc/design/event-handlers.md)
4) [Modules](doc/design/modules.md)
5) [Testing](doc/design/testing.md)

## Reference

[Full reference documentation](https://astronautix.github.io/IonSat-RFF/index.html)