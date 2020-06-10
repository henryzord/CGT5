# Computer Graphics


## Instructions
1. Install the required packages:
```
sudo apt install build-essential cmake freeglut3 freeglut3-dev libjpeg-dev
```
2. Navigate to `build` folder and run `cmake ..` to create compilation instructions.
3. Run `cmake --build .` to build the executables.

All executables will be in the `build/examples` folder.


## Exercise 1
Solved in file `examples/example_exercise1.cpp`. Requires two command-line parameters:
* `<image_path>`: the path to the image that will be used, and
* `<n_seconds>`: the time, in seconds, for the image to move across the screen.

Example: run `./examples/exercise1 ../data/box.jpg 3.5`


## Exercises 2-4
Solved in file `examples/example_exercise2.cpp`:
* Exercise 2: `is_convex`.
* Exercise 3: `is_insie_convex`.
* Exercise 4: `is_inside_concave`.

Example: run `./examples/exercise2`.
