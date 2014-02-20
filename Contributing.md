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


Design patterns
---------------

Class hierarchy in Pandora exists in two branches: the interface branch in which all interface classes start with "I" and the templated container branch in which an interface class is passed through as template to the root "ImplContainer" ("implementation container"). On top of both branches follows the actual user API class for interacting with a Pandora entity, e.g. "Block".

The user API classes are however just wrapper / container (as indicated by their inheritance from "ImplContainer") while the actual work is done by the specific implementations (e.g. HDF5). For each user API entity like "Block" an implementation specific entity like "BlockHDF5" exists, that implements the same interface ("IBlock") as "Block". Other then the user API classes these specific implementations inherit only along the interface branch.

When the user calls a method in "Block" the corresponding method is called in "BlockHDF5" - or any other specific implementation, depending on which template parameter "Block" was given.

Entity relations
----------------

The Pandora data entities can have different types of relationships. For example a Block can have different Sections and Sources, while Sections and Sources can have different "child" Sections or Sources respectively and so on. In general the entities can have 1-n or n-m relationships. 
Sub-entities with 1-n relationship are accessible from their parent entity through "getEntity" and "entities" methods, where "Entity" = the name of the entity, e.g. "getSource" and "sources".


Testing
-------
   
   - Unit test can be found in the test sub directory.
   - Every implemented method must be tested.
   - Please make sure that all tests pass before merging/sending pull
   requests. 

