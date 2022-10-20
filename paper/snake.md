# Test case 1 : QT

<!--toc:start-->
- [Test case 1 : QT](#test-case-1-qt)
  - [Intro](#intro)
  - [Steps followed](#steps-followed)
    - [Docker](#docker)
      - [Dockerfile](#dockerfile)
      - [Scripts](#scripts)
  - [The project](#the-project)
    - [Snake window](#snake-window)
  - [Link to lessons](#link-to-lessons)
    - [Toolchain](#toolchain)
    - [C library](#c-library)
    - [CPU optimization](#cpu-optimization)
  - [Screenshots](#screenshots)
  - [Conclusion](#conclusion)
  - [Extras](#extras)
<!--toc:end-->



## Intro
For my cross-compiling test-case I decided to go with snake. As I had never really made any proper games.\
This was a great chance to finally get a feel of it.

Once the project was functional I decided to set up docker for cross-compiling with MXE. Since this was the majority of
the work aswell as the main focus of the project, that is what we will mainly be talking about.


## Steps followed
### Docker
 I chose to use a docker container to do the cross-compiling.
 Theres a few clear adventages to using docker, firstly, it is generally more safe to do anything inside of a docker container.
 Some of these overlap with using a virtual machine. Such as not having to install packages on your local machine and risking dependency issues.
 Docker containers also take up less space than a virtual machine.

 Secondly, is saving time, when creating a Dockerfile, caches of your image are created.
 When you run the dockerfile the seperate steps are cached. This means that as long as no changes are made above a step,
 The step won't be rerun. For example package downloads are only run the first time as long as we don't change the base image.

 Lastly, it makes it easier to automate steps, as we can just mount our source files on the image.

#### Dockerfile

- Image:
    I decided to go with ubuntu LTS as my image as it is supposed to be a fairly stable experience.

    ```Dockerfile
    FROM: ubuntu:20.04
    ```

- Install dependencies:
    1. Run apt update to make sure the package list is up to date

    2. Install all requirements listed on the MXE website

    ```dockerfile
    # install requirements
    RUN apt update
    RUN apt install -y --no-install-recommends \
       package-list 
    ```
- Python issue: When running the docker file you will run into an issue with python. This is because the binary for python is called `python3` but a lot of applications still use `python`.
                To fix this issue we simply create a symbolic link from `usr/bin/python3` to `usr/bin/python`
    ```dockerfile
    RUN ln -s /usr/bin/python3 /usr/bin/python
    ```

- Building MXE
    1. For organizing purposes we create a folder `build` in which we will do anything related to building.

    2. We clone MXE from the git repository into the build folder.

    3. First we download the packages we will build. This way if something goes wrong, the downloads are cached and we don't have to do them again.

    4. Now we build mxe with the necesary packages. In my case this is `qtbase` for the core of qt and also `qtmultimedia` because I play a sound file.
    
    5. When the build process is complete we have to add MXE to path

    6. Then we add an alias for the cross-compiling binary we just made called `qmake`. This just makes it easier to use.
    
    ```dockerfile
    # clone mxe to build directory
    RUN mkdir /build
    WORKDIR /build
    RUN git clone https://github.com/mxe/mxe.git
    
    # Download the necesary packages

    RUN cd mxe && make download-qtbase download-qtmultimedia --jobs=2

    # Build the downloaded packages

    RUN cd mxe && make qtbase MXE_TARGETS="x86_64-w64-mingw32.static" MXE_USE_CCACHE=
    RUN cd mxe && make qtmultimedia MXE_TARGETS="x86_64-w64-mingw32.static" MXE_USE_CCACHE=
   
    # Add mxe to path
    ENV PATH /build/mxe/usr/bin:$PATH
    
    
    # Add alias for qmake
    RUN ln -s /build/mxe/usr/bin/i686-w64-mingw32.static-qmake-qt5 /build/mxe/usr/bin/qmake

    ```

#### Scripts

I made two scripts to execute for the cross compiling.

1. Build script, runs the Dockerfile to compile MXE, this should theoretically only be run once
    ```bash
    #!/bin/bash -x
    #
    # Build the image
    docker build -t qt .
    ```
2. Compile script, compiles the qt code with MXE
    ```bash
    #!/bin/bash -x
    #
    # This simple script builds the the application for Windows
    # using a cross compiler.
    #
    #build project
    
    # Mount the src of the project to /src, this way the executable won't need to be extracted from the docker image
    docker run -i --workdir="/src" -v "$(pwd):/src" qt bash -c 'qmake && make' 
    ```

## The project
The main components of my project were:

- Color picker
- High score saving
- The snake window

### Snake window
For the snake window, QGraphicsView was an obvious choice after discovering it.\
It also provides collision detection natively, so I did not have to write my own code for that.

However I did notice that this isn't the most accurate collision detection. Sometimes collision will be signaled
even when you can clearly see it did not touch. This is by a small margin however so I found it to be acceptable\
This is mainly noticible because sometimes you can pick up food that is not on the same x or y coordinate as the snake.

I attempted to create a grid like feeling by making the food spawn only on multiples of the size of the snakes body.
This isn't perfect but it does make a big difference over not controlling the spawn location.

## Link to lessons
### Toolchain
 The toolchain we are using is a cross build toolchain.
 Building on linux for a windows system.

 I used qmake as build system for my qt app, this means the Makefile is automatically generated for me.
 The Makefile is generated based on your qt `.pro` file.\
 As a consequence, after running `qmake`, you will need to run `make` to actually compile your code.

 When running `make` the C++ compiler will be used. MXE uses the MinGW compiler to cross compile the code.
 MinGW is a project which aims to support the gcc compiler on windows.

### C library
Since we are cross compiling for Windows. MinGW uses the `msvcrt` library or `libmsvcrt`.
`msvcrt` stands for `Microsoft Visual C++ Runtime`.

### CPU optimization 
You can optimize the compiling by increasing the amount of jobs. I chose not to use all my cores as I was letting this run in the background while working on other things.

## Screenshots
![](assets/Screenshot1)
![](assets/Screenshot2)
![](assets/Screenshot3)
![](assets/Screenshot4)
![](assets/Screenshot5)

## Conclusion
In the end, the actual project proved more challenging than the cross-compiling. However cross-compiling took longer
because of the compile time and having to do it more than once when making a mistake.

Once I figured out the process it was just following steps and nothing more. So the process of cross-compiling isn't actually
that difficult.


## Extras
 - Docker
 - Media playing
