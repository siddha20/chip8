# chip8

A chip8 emulator written in C++.

Closely follows Cowgod's manual: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#00E0

## Building
Create a build directory and use cmake to build the project.

```
mkdir build
cd build
cmake ..
```

## Usage
Run the emulator on the command line: 

`chip8 <path to game rom>` \
`chip8debug <path to game rom>`

There are some ROMs here: https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html

## Dependencies
SDL2: https://github.com/libsdl-org/SDL/releases/tag/release-2.26.5 \
SDL2_ttf: https://github.com/libsdl-org/SDL_ttf/releases/tag/release-2.20.2

For MSVC users, drag the SDL library folders into the project root and name them SDL2 and SDL2_ttf.

