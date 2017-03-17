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
   The first line of the message should not exceed **50** characters and the 2nd line should be empty.
   If you want to add further text you can do so from the 3rd line on without limitations.
   If possible reference fixed issues in the commit message (e.g. "fixes #101").

3. When done with the implementation, compile and test the code with clang **and** g++.
   If your work includes a new function or class please write a small unit test for it.

4. Send us a pull request with your changes.
   The pull request message should explain the changes and reference the *issue* addressed by your code.
   Your pull request will be reviewed by one of our team members.
   Pull requests should never be merged by the author of the contribution, but by another team member.
   Merge conflicts or errors reported by travis should be resolved by the original author before the request is merged.


Google Summer of Code contributors
----------------------------------

Please see the corresponding [Google Summer of Code](GSoC.md) file if you are interested in contributing as part of the GSoC programme.


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


Testing
-------

* Unit test can be found in the test sub directory.

* Provide a unit test for every class, method or function.

* Please make sure that all tests pass before merging/sending pull requests.

* Since continuous integration for the windows platform is still work in progress, contributors should
  build and test code that contains a significant amount of changes also under windows (see: Win32.md).


Code style
----------

* We use a slightly modified K&R style in which we use 4 spaces instead of tabs for indentation.

* Method names are written in camelCase.

* Add Doxygen comments to methods (with javadoc like markup).

* In doubt just look at some existing files and adjust your code style and naming scheme accordingly.


Conventions for names
---------------------

Naming of getter- and setter-methods for properties follows one of two possible patterns:

* Pattern for simple-type properties (numeric, string, single entity):
  Getter and setter methods for properties are named according to the property name (e.g. setting the name of an
  entity is done with *name(const string &name)* and retrieved with *name()*).

* Pattern for properties that store multiple entities (as vector):
  The following methods may exist, where *Entity* is replaced by the actual entities names: *getEntity*,
  *getEntities*, *hasEntity*, *addEntity*, *createEntity*, *deleteEntity*,
  *removeEntity*, *findEntities*.

  * *getEntity*: always exists and is a single entity getter.

  * *entities*: always exists and is a multiple entity getter with optional filter parameter.

  * *hasEntity*: must exist if the property is optional, i.e. does not have to be set, may exist otherwise.

  * *addEntity*: exists where references to entities are stored in a property, adds reference to an existing entity.

  * *createEntity*: always exists (entity factory), returns reference to a new entity.

  * *removeEntity*: exists where references to entities stored in property, removes reference to an existing entity
    but does not delete it

  * *deleteEntity*: always exists, deletes entity.

  * *findEntities*: exists only for the fiew entities that reference each other internally, e.g. as parent or child
    and thus build trees. Walks these trees and returns entities matching the given filter.


Design patterns
---------------

Class hierarchy in NIX exists in two branches: the interface branch in which all interface classes
start with "I" and the templated container branch in which an interface class is passed through as
template to the root *ImplContainer* (implementation container).
On top of both branches follows the actual user API class for interacting with a NIX entity, e.g. *Block*.

The front-end user API classes are however just wrapper or container (as indicated by their inheritance
from *ImplContainer*), meaning they just reference specific back-end implementations (e.g. HDF5) that
do the actual work for all front-end methods (by which they get called).
Thus, for each front facing API entity like *Block* an implementation specific entity like *BlockHDF5*
exists, that implements the same interface (*IBlock*) as *Block* for the respective back-end.
Other then the front-end API classes that inherit *ImplContainer* currently also implement the interface
(e.g. *IBlock*) these specific implementations inherit only along the interface branch.


Entity relations
----------------

The NIX data entities can have different types of relationships. For example a *Block* can have different
*Sections* and *Sources,* while *Sections* and *Sources* can have different child *Sections* or *Sources* respectively
and so on.
Entities can be restricted to 1-n relationship, but in some contexts also allow for n-m relationships.
Sub-entities with 1-n relationship are accessible from their parent entity through *getEntity()* or *entities()* methods,
where *Entity* is the name of the entity, e.g. *getSource* or *sources*.
