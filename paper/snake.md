# Test case 1 : QT
## Intro

## Steps followed
### Docker
I chose to cross compile inside a container to save myself from future headaches. It also provides a fairly simple way of automating the process.

#### Dockerfile

- Image:
    I decided to go with ubuntu LTS as my image as it is supposed to be a fairly stable experience.

    ```Dockerfile
    FROM: ubuntu:20.04
    ```

- Install dependencies:
    1. Run apt update to make sure the package list is up to date

    2. Install all requirements listed on the MXE website

    ```Dockerfile
    # install requirements
    RUN apt-get update
    RUN apt-get install -y --no-install-recommends \
       package-list 
    ```
- Python issue: When running the docker file you will run into an issue with python. This is because the binary for python is called `python3` but a lot of applications still use `python`.
                To fix this issue we simply create a symbolic link from `usr/bin/python3` to `usr/bin/python`
    ```Dockerfile
    RUN ln -s /usr/bin/python3 /usr/bin/python
    ```

- Building MXE
    1. For organizing purposes we create a folder `build` in which we will do anything related to building.

    2. We clone MXE from the git repository into the build folder.

    3. First we have to install the packages, doing this in a seperate step ensures the downloads are cached and won't be redone if we change anything below this point.
        In my case this is `qtbase` for the core of qt and also `qtmultimedia` because I play a sound file.
        I went with `qt5` in the actual build

    4. Now we build mxe with the necesary packages. I disabled ccache because it would only slow the process down in this case. 
    We set the target to 64 bit windows.

    5. When the build process is complete we have to add MXE to path

    6. Then we add an alias for the cross-compiling binary we just made called `qmake`. This just makes it easier to use.
    
    ```Dockerfile
    # clone mxe to build directory
    RUN mkdir /build
    WORKDIR /build
    RUN git clone https://github.com/mxe/mxe.git
    
    # Download the necesary packages
    # This commented line should work but I ran using qt5 because I had it enabled for testing
    # The issue turned out to be something code related but it built and I didn't want to recompile for a small optimization 
    # RUN cd mxe && make download-qtbase download-qtmultimedia --jobs=2
    # I am running with 4 jobs to speed up the process

    RUN cd mxe && make download-qt5 --jobs=4

    # Build the downloaded packages

    RUN cd mxe && make qt5 MXE_TARGETS="x86_64-w64-mingw32.static" MXE_USE_CCACHE=

    
    # Add mxe to path
    ENV PATH /build/mxe/usr/bin:$PATH
    
    
    # Add alias for qmake
    RUN ln -s /build/mxe/usr/bin/x86_64-w64-mingw32.static-qmake-qt5 /build/mxe/usr/bin/qmake

    ```
- Copying source
    1. We first create a folder to copy the source code to.

    2. Now we copy our source code into the container.

    3. We set the new source folder as our working directory
- Building the project
    - We are now ready to build the actual qt project

        1. We run qmake since that is what we created our project with

        2. Then we run make once qmake has completed to build the final executable

    ```Dockerfile
    # COPY source files to image
    RUN mkdir /src
    COPY . /src
    
    # Set workdir to /src for ease
    WORKDIR /src
    
    #Build project
    RUN qmake
    RUN make
    ```

#### Script
   To make my life easier I created a script that runs the Dockerfile and extracts the exe once it has been built. 

   ```Bash
    #!/bin/bash -x
    #
    # This simple script builds the the application for Windows
    # using a cross compiler.
    #
    
    # Build the image
    docker build -t qt .
    
    # Now extract the generated binary
    #create build directory if it doesnt exist
    mkdir -p exe
    docker run qt cat release/snake.exe > exe/snake.exe

   ```
