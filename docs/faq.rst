.. toctree::
   :maxdepth: 1

FAQ
===

Q: Why sampling interval in the sampled dimension, not sampling rate?
---------------------------------------------------------------------

Because the interval is the more **general** term. It can also be
applied to dimensions that do not extend in time but for example space.

Q: What does this error mean? “Cannot open a nonexistent file in ReadOnly mode”
-------------------------------------------------------------------------------

Arises when the provided file path is invalid and you try to open it
with the nix::FileMode::ReadOnly mode. Solution: provide the path to an
existing file to be opened in ``nix::ReadOnly`` mode or use the
``nix::FileMode::ReadWrite`` or ``nix::FileMode::Overwrite`` flag to
create a new one.

Q: Is it ok to use non-SI units?
--------------------------------

Well, let’s say it is tolerated but not advised. When using SI and
compounds of SI units we can try to scale according to the provided
units which is not supported/not necessarily possible for non-SI units.
It is largely safe to use non-SI units when providing metadata or
specifying the unit of the data stored in a *DataArray*. It may become
problematic in the context of *Dimensions*. Some functions support
reading data with the positions provided by the *Tags* in these cases we
rely on scalability of the units.

If you feel this is unjustified, feel free to improve our unit-handling
system.

Q: Can I use unicode?
---------------------

Unfortunately, unicode is so far not supported. In order to specify
microseconds you can, and have to, write “us”.

Q: Why does a *DataArray* not automatically load the data from file?
--------------------------------------------------------------------

This would be very convenient, indeed. Otoh, the *NIX* entities, such as
the *DataArray*, are designed to be lightweight objects that can be
created quickly and do not eat up the RAM. At opening time we have no
idea about the actual data size. In our use-cases a single
*DataArray* can contain GB of data. Reading these would take some time
and increase memory demand.

It further allows for partial loading of data. A *DataArray* that
contains multiple long signals can be very large and it would be
inefficient loading the entire data when you need only a
single signal, or a few data segments.
