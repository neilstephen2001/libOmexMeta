# libsemsim (aka libOmexMeta)
This is a "pre-pre-release" version of libsemsim. Note, the package name is
due to change to libOmexMeta. The package consists of a C++ backend, 
a C API and a Python front end. 

# Platform
Due to the Redland library dependencies being linux only, libsemsim has been 
developed on Ubuntu 18.04 and is presently linux only. Support for Windows 
and macos is a work in progress. 

# Installation
## Python front end
You can get up and running quickly using pip

    $ pip install --index-url https://test.pypi.org/simple pysemsim

Note that "pysemsim" is only available on pypi test. It has veen tested with Python
version 3.8, but should work on other Python 3 versions. 

## building from source
libsemsim uses a "superbuild" system to fully automate the building of dependencies. 
There is a CMake variable `HAVE_DEPENDENCIES` which turns on or off the superbuild. 
The first time you run cmake, you will probably want to have the `HAVE_DEPENDENCIES=ON` 
but if you rebuild you can set `HAVE_DEPENDENCIES=OFF`. 


libsbml + libcombine options 

# Documetnation

https://sys-bio.github.io/libsemsim-docs/

Note: documentation is currently out of date but being updated presently 


### Using CMake

CMake uses a separate build directory for storing compiled object files and intermediate configure files. See [the CMake manual](https://cmake.org/runningcmake/).

### macOS

Install [git](https://git-scm.com/), CMake 3.14 or greater, Homebrew. Clone the sources:

```bash
git clone https://github.com/sys-bio/libsemgen.git
```

Create and change to a build directory from which to run CMake. libsemsim requires the following dependencies:

* libSBML (with namespaces enabled)
* libCombine
* Raptor2
* Rasqal
* Swig
* Catch2 (optional, for building tests)
* libCellML (optional, not used)

On Mac, some of these can be installed as follows:

```bash
brew install zlib raptor rasqal redland
```

See the respective project pages for how to build and install these packages. Once these packages are install, invoke CMake with the following signature:

```bash
CMAKE_PREFIX_PATH=/path/to/libsbml/lib/cmake:/path/to/libcellml/lib/cmake:/path/to/libcombine/lib/cmake:/path/to/catch2/lib/cmake/Catch2:$CMAKE_PREFIX_PATH cmake -DCMAKE_INSTALL_PREFIX=/path/to/install/libsemsim /path/to/src/libsemsim
```

Then build the release target using XCode. libSemSim will automatically find the currently active Python interpreter (if available) and use it to generate bindings. If you want to use another Python interpreter, add it to your PATH.

Finally, to build the documentation, make sure you have the following packages installed: `sphinx breathe sphinxcontrib-bibtex sphinx_rtd_theme`.

#### SBML Support

In order to support loading SBML models, you will need to build and install libSBML 5.18.0 or later. Refer to the [libSBML building instructions](http://sbml.org/Software/libSBML). You may use the stable or experimental version.

#### CellML Support

In order support loading CellML models (not implemented yet), you will need to build and install libCellML from source. Refer to the [libCellML](https://libcellml.readthedocs.io/en/latest/) documentation for building instructions.

#### Building libSemSim

Create an empty "build" directory for intermediate CMake files and compiled object files. Where you put this directory is up to you. Change to the directory and run the following command (making sure to substitute the appropriate paths for your machine):

```bash
CMAKE_PREFIX_PATH=/path/to/libsbml/lib/cmake:/path/to/libcellml/lib/cmake:/path/to/libcombine/lib/cmake:$CMAKE_PREFIX_PATH PKG_CONFIG_PATH=/usr/local/Cellar/redland/1.0.17_1/lib/pkgconfig:$_PKG_CONFIG_PATH cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/path/to/libsemgen-install /path/to/libsemgen-src
make -j4 install
```

### Linux

Make sure [git](https://git-scm.com/) is installed and clone the libsemgen source:

```bash
git clone https://github.com/sys-bio/libsemgen.git
```

#### Dependencies

Install raptor, rasqal, and redland using your package manager. Instructions vary by distribution. On Fedora:

```bash
sudo dnf -y install raptor2 raptor2-devel rasqal rasqal-devel redland redland-devel
```

#### SBML Support

In order to support loading SBML models, you will need to build and install libSBML 5.18.0 or later. Refer to the [libSBML building instructions](http://sbml.org/Software/libSBML). You may use the stable or experimental version.

#### CellML Support

In order support loading CellML models (not implemented yet), you will need to build and install libCellML from source. Refer to the [libCellML](https://libcellml.readthedocs.io/en/latest/) documentation for building instructions.

#### Building libSemSim

Create an empty "build" directory for intermediate CMake files and compiled object files. Where you put this directory is up to you. Change to the directory and run the following command (making sure to substitute the appropriate paths for your machine):

```bash
CMAKE_PREFIX_PATH=/path/to/libsbml/lib/cmake:/path/to/libcellml/lib/cmake:/path/to/libcombine/lib/cmake:$CMAKE_PREFIX_PATH cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/path/to/install/libsemgen /path/to/src/libsemgen
make -j4 install
```
