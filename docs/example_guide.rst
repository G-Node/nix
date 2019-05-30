.. toctree::
   :maxdepth: 1


Building and executing examples
===============================

Most code snippets of the tutorials should be standalone and runnable
if the nix library and its dependencies have been :doc:`installed<installation>`.

Download or copy-paste the code examples into a file (here
*example.cpp*) and build it.

MacOs
-----

You need to have the c++ compiler clang installed. If you
installed nix via homebrew, the library will be located in */usr/local/Cellar/*

.. code-block:: bash

   > clang++ -std=c++11 -lnix -L/usr/local/Cellar/ -O2 example.cpp -o example
   > ./example

Linux
-----

Under Linux the exact library location depends on the
distribution. gcc should be able to find it in the distribution's
default folder.

.. code-block:: bash

   > g++ -std=c++11 -lnix example.cpp -o example
   > ./example

Windows
-------

TODO
