# SampleCode
Sample-Code for SOFT356

## Versions
Visual studios 2019 - 16.3.10
glew-2.1.0
freeglut-3.2.1

### Instructions
Upon running the program, you will be prompted to type in the name of a file. The parser only supports objs and the objs must be placed inside the media folder. The texture file must be the same name as the obj and in png format. You dont need to add the file extention .obj or .png as this is done automatically. Currently, the loader can render the cube but is unable to successfully wrap the texture. As far as I can see, everything is being read in correctly which suggests that there is some logic error on my part when it comes to binding the buffers. Upon testing, the program seems to be able to sort and store all of the pouf values although it doesnt display. This is possibly due to the camera being too close to the world centre meaning the camera is inside the rendered object.

#### How it works
1. Starts off by asking for user input in regards to the file name they wish to open.
2. Loops until the user inputs a valid file name.
3. Parses the file and stores the data.
4. Checks if the faces are made of squares or triangles.
5. If the faces are squares, it breaks the squares into triangles.
6. Loads the buffers to be rendered.
