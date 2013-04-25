OpenGL Game of life viewer
==========================

Qt and OpenGL 3 based Game of Life board viewer

This application shows a pre-calculated Game of Life board within an OpenGL window.
It can flawlessly show a 600×600 area with 50 ms refresh rate on a GTX 260.

Compiling information
---------------------

The following are needed to compile this project:
- Qt 5
- OpenGL 3
- C++11-aware compiler

In the project directory issue the following to compile and run the application:
    qmake && make && ./GameOfLifeOpenGL

TODOs
------

- [] You will have to replace the .lif file path in main.cpp:156
- [] Although the viewer is speedy even now, you can improve it by using OpenGL Vertex Buffer Objects (VBOs)

File format descrption
----------------------

The program uses simple structured .lif files. The asterisk (\*) symbol denotes the living cell and the period (.) denotes the dead one.
The file has a small, self-describing header section. Empty lines or lines beginning with hashmark (#) are ignored.

Example:
```
# GameOfLife.lif
width = 60
height = 13
frames = 150
speed = 100

#State no. 1
................*.........*....***...*.......*..*......**...
*....**..............*...*........*.*....*...*....**.....*..
....*..*...........**.....................*..............*..
...............*..***.....................*.............*...
...*.........*.....*.....**..............*..................
..*.....................*..*..................*..*.........*
...*..*..........*.**.*.*......*...........*....*.*.........
......*............*.....*.*......*.........*..........*....
......*....*....*......*......*.......*............*........
**.............*.......**.*..*................*......*......
.*......*................**....*.....*....**................
..*........*...*...............**..*............*...........
............*...*........*....*....***...*......*...........

#State no. 2
................*.........*....***...*.......*..*......**...
*....**..............*...*........*.*....*...*....**.....*..
....*..*...........**.....................*..............*..
...............*..***.....................*.............*...
...*.........*.....*.....**..............*..................
..*.....................*..*..................*..*.........*
...*..*..........*.**.*.*......*...........*....*.*.........
......*............*.....*.*......*.........*..........*....
......*....*....*......*......*.......*............*........
**.............*.......**.*..*................*......*......
.*......*................**....*.....*....**................
..*........*...*...............**..*............*...........
............*...*........*....*....***...*......*...........
```
