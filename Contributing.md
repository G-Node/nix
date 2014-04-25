How to contribute to NIX
========================

This document gives some information about how to contribute to the NIX project.


Contributing
------------

If you want to contribute to the project please first create a fork of the repository on GitHub.
When you are done with implementing a new feature or with fixing a bug, please send 
us a pull request.

If you contribute to the project regularly, it would be very much appreciated if you 
would stick to the following development workflow:

1. Select an *issue* from the issue tracker that you want to work on and assign the issue to your account. 
   If the *issue* is about a relatively complex matter or requires larger API changes the description of the 
   *issue* or its respective discussion should contain a brief concept about how the solution will look like.

2. During the implementation of the feature or bug-fix add your changes in small atomic commits.
   Commit messages should be short but expressive. 
   The first line of the message should not exceed **50** characters.
   If possible reference fixed issues in the commit message (e.g. "fixes #101"). 

3. When done with the implementation, compile and test the code with clang **and** g++. 
   If your work includes a new function or class please write a small unit test for it.
   
4. Send us a pull request with your changes. 
   The pull request message should explain the changes and reference the *issue* addressed by your code.
   Your pull request will be reviewed by one of our team members.


The issue tracker
-----------------

Please try to avoid duplicates of issues. If you encounter duplicated issues, please close all of them except 
one, reference the closed issues in the one that is left open and add missing information from the closed issues 
(if necessary) to the remaining issue.

Assign meaningful tags to newly crated issues and if possible assign them to milestones.


Reviewing pull requests
-----------------------

Every code (even small contributions from core developers) should be added to the project via pull requests.
Before reviewing a pull request it should pass all builds and tests on travis-ci.
Each pull request that passes all builds and tests should be reviewed by at least one of the core developers.
If a contribution is rather complex or leads to significant API changes, the respective pull request should be 
reviewed by two other developers.
In such cases the first reviewer or the contributor should request a second review in a comment.


Code style
----------

* We use a slightly modified K&R style in which we use 4 spaces instead of tabs for indentation.

* Method names are written in camelCase.

* Getter and setter methods for properties are named according to the property name (e.g. 
  setting the name of an entity is done with *name(const string &name)* and retrieved with *name()*).
  Methods that delete entities are named with *deleteXyz()*. 
  Those that remove references to other entities (but do not delete) are named *removeXyz()*.

* Add Doxygen comments to methods (with javadoc like markup)

* In doubt just look at some existing files and adjust your code style and naming scheme accordingly.


Testing
-------
   
* Unit test can be found in the test sub directory.

* Provide a unit test for every class, method or function.

* Please make sure that all tests pass before merging/sending pull requests.

* Since continuous integration for the windows platform is still work in progress, contributors should 
  build and test code that contains a significant amount of changes also under windows (see: Win32.md). 


Design patterns
---------------

Class hierarchy in NIX exists in two branches: the interface branch in which all interface classes 
start with "I" and the templated container branch in which an interface class is passed through as 
template to the root "ImplContainer" ("implementation container"). 
On top of both branches follows the actual user API class for interacting with a NIX entity, e.g. *Block*.

The user API classes are however just wrapper / container (as indicated by their inheritance from 
*ImplContainer*) while the actual work is done by the specific implementations (e.g. HDF5). 
For each front facing API entity like *Block* an implementation specific entity like *BlockHDF5* exists, 
that implements the same interface (*IBlock*) as *Block* for the respective backend. 
Other then the front facing API classes these specific implementations inherit only along the interface branch.

When the user calls a method in *Block* the corresponding method is called in *BlockHDF5* - or any other 
specific implementation, depending on which template parameter *Block* was given.


Entity relations
----------------

The NIX data entities can have different types of relationships. For example a *Block* can have different 
*Sections* and *Sources,* while *Sections* and *Sources* can have different child *Sections* or *Sources* respectively 
and so on. 
In general the entities can have 1-n or n-m relationships. 
Sub-entities with 1-n relationship are accessible from their parent entity through *getEntity()* or *entities()* methods, 
where *Entity* is the name of the entity, e.g. *getSource* or *sources*.




