.. _setupDocker:

Running Tudat using Docker
==========================
If you need a quick introduction to Docker, here is a 10 minute read: https://docs.docker.com/engine/docker-overview/

The absolute bare essentials:

- Docker allows you to run a computer system in a so-called container. A container can run on a laptop, on a server or in the cloud. A container is isolated from other processes and containers running on the same hardware. This means it can use a different operating system from the host hardware.
- Docker works with images. An image defines a computer configuration, including the operating system and installed software. An image is built up of layers.
- You can’t work in the Docker image itself. You need to run a Docker container from an image. It’s possible to run more than one container from the same image. When connecting to the container, you can define network configurations and storage volumes.

Install Docker Community Edition (CE) on your computer
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
You can find instructions to install Docker here:
https://docs.docker.com/install/.
Note that there are different installation procedures, depending on the computer platform you are using.

You can check that your Docker environment has been installed successfully with the command ``docker --version``. This should give a version number and build hash.

Some useful commands
~~~~~~~~~~~~~~~~~~~~

- List images with: ``docker image ls``
- Remove an image: ``docker image rm <hash of image>``
- List all containers, including those that have stopped: ``docker container ls -a``
- Remove a container with: ``docker container rm <hash of container>``

Running a Tudat Docker container from an Image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
A Docker image of the TudatBundle has been created for you here:
https://hub.docker.com/r/sebranchett/tudat/tags. Check the tag name of the version you wish to use, probably the most recent one. You can access an image using the format::

<username>/<repository>:<tagname>

where ``<username>`` is 'sebranchett', ``<repository>`` is 'tudat' and ``tagname`` is the name of the tag, or version, you wish to use.

Pull the image you wish to use to your local computer::

   docker pull <username>/<repository>:<tagname>

Running on Linux
****************
You can run a container from your image with this command::

   docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" -it --rm <username>/<repository>:<tagname>

If you want to know how and why this works, read this post: https://medium.com/@SaravSun/running-gui-applications-inside-docker-containers-83d65c0db110
The ``-it`` flag lets you work in the container interactively and the ``--rm`` flag deletes the container (but not the image) once you ``exit`` the container. You may want to remove the ``--rm`` flag if you want to return to the container, although getting a GUI to work again may not be possible.

Running on a Virtual Machine (VM)
*********************************
You need to ``export DISPLAY=localhost:0`` BEFORE you ssh into the VM. You also need to specify the ``–XY`` flag when using ssh as follows::

   ssh -XY netID@myvmbastion.tudelft.nl

Once you are logged in again, you can run the docker container as for a regular Linux computer::

   docker run --net=host --env="DISPLAY" --volume="$HOME/.Xauthority:/root/.Xauthority:rw" -it --rm <username>/<repository>:<tagname>

Running on Windows
******************
You need to perform an extra step. These posts explain in detail:
https://dev.to/darksmile92/run-gui-app-in-linux-docker-container-on-windows-host-4kde
https://stackoverflow.com/questions/40024892/windows-10-docker-host-display-gui-application-from-linux-container

You need an X11 display server for Microsoft Windows. In the links above they use VcXsrv and Xming. This document was tested using Xming.

It is important that you enable ``No Access Control``. For Xming this can be done by performing a default installation and then modifying the properties (right-click on Xming shortcut). Add the ``-ac`` flag to the Target definition so that it reads something like::

   "C:\Program Files (x86)\Xming\Xming.exe" :0 -clipboard -multiwindow –ac

When you want to run Docker, FIRST start your X11 display server with ``No Access Control``, then (stop and) start you Docker environment (Docker Quickstart Icon). Look up the IP address of your ``Ethernet adapter VirtualBox Host-Only Network`` using the command ``ipconfig``. There may be 2 and you may have to try both before things work. Set you DISPLAY environment using::

   export DISPLAY="<IP address>:0"

for example: ``export DISPLAY="192.168.99.1:0"``
You can run the docker container as follows::

   docker run --env="DISPLAY" -it --rm <username>/<repository>:<tagname>

Note that running Docker on Windows 7 can use up a lot of your C: drive. You can periodically remove the folder ``C:\Users\<username>\.docker``. This will remove all your local images and containers, but will not uninstall Docker.


Running on Mac
**************
You need to perform an extra step. This post explains in detail:
https://sourabhbajaj.com/blog/2017/02/07/gui-applications-docker-mac/

You need to install a display server such as XQuartz and run it with ``Allow connections from network clients``. You can then set the IP environment variable to the IP address of your local machine.

You can run the docker container as follows::

   docker run --env DISPLAY=$IP:0 --volume=/tmp/.X11-unix:/tmp/.X11-unix -it --rm <username>/<repository>:<tagname>

** NOT TESTED **

For all platforms
*****************
Once you are up and running in a Docker container, you can check if the GUI is working properly by typing ``xeyes``. You should see 2 eyes that follow your mouse around the screen.

If that works you can close xeyes and launch Qt Creator by typing ``qtcreator``.

You can rerun the standard tests by typing ``ctest`` on the command line in the tudatBundle directory. It will take some time to run them all.

Type ``exit`` to exit the container. If you used the ``--rm`` flag, this will automatically delete the container, leaving the image intact.
