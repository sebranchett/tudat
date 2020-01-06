.. _creatingDockerImage:

Creating a Tudat Docker image
=============================
To install and use Docker for Tudat, please see `Running Tudat using Docker <../../installation/setupDocker.html>`_.

This documentation describes how to create a Dockerfile, use the Dockerfile to create an image and how to load the image to DockerHub.

The current documentation was tested on a standard TU Delft Faculty Managed Server (Virtual Machine or VM for short):

- Ubuntu 18.04 LTS (64bits)
- 1 processor
- 2 GB internal memory
- Root disk 20 GB, Swap disk 2 GB
- 1 network interface

Creating a Dockerfile
~~~~~~~~~~~~~~~~~~~~~
In a clean directory, create a plain text file called ``Dockerfile`` with the following content::

   FROM ubuntu:<version>
   
   RUN apt-get update &&                  \
       apt-get upgrade -y &&              \
       apt-get install -y                 \
               build-essential cmake      \
               git                        \
               vim                        \
               x11-apps                   \
               qtcreator                &&\
       apt-get clean
   
   RUN git clone https://github.com/Tudat/tudatBundle.git
   WORKDIR tudatBundle
   RUN git checkout -b dev-in-docker <commit hash>
   RUN git submodule update --init --recursive
   
   RUN cmake CMakeLists.txt
   RUN cmake --build . --target all
   RUN ctest
   
   CMD ["/bin/bash"]

Replace <version> with the Ubuntu version you wish to use (e.g. 18.04) and <commit hash> with the hash of the tudatBundle commit you wish to use (https://github.com/Tudat/tudatBundle/commits/master, e.g. 6451ba5).

The first line makes sure you start from the correct version of the operating system. The update and upgrade  lines ensure that the software packages available to your operating system are up to date and that any installed packages are upgraded to the latest version.

The next block installs the packages you need to run Tudat. ``build-essential cmake`` installs everything you need to run cmake for C++ projects. ``git`` is used to copy the Tudat source code and for version control. ``vim`` is included so that you can edit files. ``x11-apps`` includes xeyes, which can be useful when debugging GUI (graphical user interface) problems. ``qtcreator`` is useful when developing code in Tudat.

``git`` is then used to make a copy of the Tudat software. After moving to the tudatBundle directory, ``git`` is used again to check out the specified commit version (from the master branch) and then to update submodules of the Tudat package.

``cmake`` is used to generate the tudatBundle project build system and to build the project. Once the project has been built, ``ctest`` is used to perform the standard Tudat tests.

The last line gives you a shell when you run a Docker container.

Creating the Docker image
~~~~~~~~~~~~~~~~~~~~~~~~~
To build an image, first check that your Docker environment has been installed successfully: the command ``docker --version`` should give a version number and build hash. Then create a Dockerfile (without a filename extension) as described above.

Here is an example of how to create the image::

   docker build --label OS="Ubuntu 18.04" --label Tudat_branch=main --label Tudat_commit=20191112 -t <repository>:<tagname> .

By default, Docker will read the Dockerfile in the current directory. The labels are optional (``--label <label name>=<label value>``) and can be inspected with::

   docker inspect -f "{{json .Config.Labels }}" <repository>:<tagname>
Note that, if a build fails for some reason, for example if a test fails, the image will not be tagged properly. Also the container, used to create the image, will not be removed automatically.

If you want to retag and relabel an image (with hash ``12ae80a0b804`` in this example), you can create a new image by creating an extra labelled layer on top of the original image. For example::

   echo "FROM 12ae80a0b804" | docker build --label OS="Ubuntu 18.04" --label Tudat_branch=main --label tudat_commit=20191112 -t <repository>:<tagname> -

Loading the image to DockerHub
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For more information read: https://ropenscilabs.github.io/r-docker-tutorial/04-Dockerhub.html.

If you don’t already have one, create an account on DockerHub: https://hub.docker.com/ 
If you don’t already have a repository, sign into your account, click on the ``dockerhub`` whale and then on the ``Create Repository +`` button. Make your repository public so that other people can use your image.
Back on the command line::

   docker login --username=<username>

follow the instructions and then tag the image you want to upload before pushing::

   docker tag <hash of image> <username>/<repository>:<tagname>
   docker push <username>/<repository>:<tagname>

You can find an example here: https://hub.docker.com/r/sebranchett/tudat/tags.

It is now good practice to test your image on different operating systems. 
See `Running a Tudat Docker container from an Image <../../installation/setupDocker.html#running-a-tudat-docker-container-from-an-image>`_.
