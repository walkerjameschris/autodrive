# <img src="https://raw.githubusercontent.com/walkerjameschris/autodrive/refs/heads/main/img/car.png" height="25" vertical> autodrive

A from-scratch implementation of Q-learning in C++ used to train a 2D car
how to drive around a simple racetrack. The system works by simulating
epochs many times. For each epoch, the car is *rewarded* for travelling
a distance. However, if it runs off the track it crashes and is penalized.

These rewards and penalties are used to develop an in-memory Q-table. This
is a hashmap which **relates state to action**. In this simulation, the 
state is the car's sensors which measure the distance from the car to the
edge of the road on pre-derermined angles. This state is related to Q-values
which are fed through a series of functions to determine the best action.

When the car is training, it updates it Q-table at each step. When it is
evaluated, it simply references the Q-table to make choices. I also
implemented my own random number generator to generate floats and integers,
just for fun!

# Build Instructions

This project depends on `SFML` to display the simulation window. You can visit
the SFML link below in _Resources_ for install instructions. Apart from this
one dependency, this follows a typical CMake workflow:

1. Create `build/` directory in this repo
2. Navigate to build and run `cmake ..`
3. Run `make`
4. Run the binary

On Linux that is (assuming you are in the base directory of this repo):

```sh
mkdir build
cd build
cmake ..
make
./autodrive
```

> [!IMPORTANT]
> `autodrive` can only be run from the `build/` directory. This is because
> it loads files from other directories in the project in a relative fashion.

# Running Simulations

Simply running the binary will launch a train and evaluation cycle. There 
are a few hyperparameters described in `main.cpp` which can be tweaked,
but their default settings work well.

<img src="https://raw.githubusercontent.com/walkerjameschris/autodrive/main/img/autodrive.png" height="225" />

# Resources

* [SFML](https://www.sfml-dev.org/)
* [CMake](https://cmake.org/cmake/help/latest/guide/tutorial/index.html)
