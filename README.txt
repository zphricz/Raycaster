
                             SIMPLE RAYCASTER
Overview
--------

It's a raycaster. Run it like this:

    ./raycaster

Map Format
----------

The first section of the map defines the shape of the map. Each character in
this section defines one cell on the map. Characters extending left to right on
a line will be layed out along the x axis on the map. Characters extending up
and down on new lines will be layed out along the y axis of the map. The
coordinate (0, 0) is at the very top left corner of the very top left cell.
X values increase to the right and y values increase downwards. Each cell has
a character to signify its type. These can be any character that is not a '\n'
or '\0' character. ' ' characters represent the lack of a cell in that location
on the map. A sequecnce of two '\n' characters in a row signifies the end of the
map section.

The next section defines the starting position and orientation of the player.
It is three separate floating point values that specifiy the x, y, and direction
(in degrees) respectively. The direction is specifies clockwise with respect to
the positive x-axis. Please note that because coordinate (0, 0) corresponds to
the top left hand corner of the top left hand cell, the coordinate (1, 1) will
accordingly refer to the bottom right hand corner of that same cell. To start
the player outside of that top left hand cell, you will have to specifiy the
starting coordinate with both x and y values greater than 1.

The next section is a collection of two rgb values. These are values in the
range [0, 255] that specify first the color of the cieling and then the color of
the floor.

The final section is a collection of rgba values along with an index value. The
index value comes first and maps the corresponding rgba value to a cell type on
the map defined in the first section. The rgba value for a cell type follows its
index.

Authors
-------

Zack Hricz

