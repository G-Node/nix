How to contribute to nix
===========================

This document gives some informationa how to contribute to the pandora/nix project.


Contributing
------------

If you want to contribute to the project please create a fork of the
repository. When you are done implementing a new feature/fixing a bug
please send pull requests.

If you experience bugs in the library or find flaws/errors in the documentation
please create an **issue** and tag them accordingly.


Code style
----------------

   - We use a slightly modified K&R style in which we use 4 spaces
   instead of tabs for indentation.
   - Method names are written in camelCase.
   - Getter and setter methods for properties are named
   according to the property name (e.g. setting the name of an entity is
   done with name(const string &name) and retrieved with name()).
   Methods that delete entities are named with deleteXyz(). Those that
   remove references to other entities (but do not delete) are
   named removeXyz().

   - Add Doxygen docstrings to methods.


Interface design
----------------


Testing
-------
   
   - Unit test can be found in the test sub directory.
   - Every implemented method must be tested.
   - Please make sure that all tests pass before merging/sending pull
   requests. 

