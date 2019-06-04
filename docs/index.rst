.. nixio documentation master file, created by
   sphinx-quickstart on Tue May 28 16:57:14 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

===========================
NIXIO library documentation
===========================

|Build Status| |Build status| |Coverage Status|

.. |Build Status| image:: https://travis-ci.org/G-Node/nix.svg?branch=master
   :target: https://travis-ci.org/G-Node/nix
.. |Build status| image:: https://ci.appveyor.com/api/projects/status/1qlcasjg2fpqotig/branch/master?svg=true
   :target: https://ci.appveyor.com/project/G-Node/nix/branch/master
.. |Coverage Status| image:: https://coveralls.io/repos/G-Node/nix/badge.svg?branch=master
   :target: https://coveralls.io/r/G-Node/nix?branch=master



C++ library for storing scientific data in the *NIX* data model.

.. raw:: html

   <!-- The *NIX* project started as an initiative within the -->

.. raw:: html

   <!-- Electrophysiology Task Force a part of the -->

.. raw:: html

   <!-- [INCF](http://www.incf.org/) Data sharing Program.   -->

The *NIX* data model allows to store fully annotated scientific
datasets, i.e. the data together with its metadata within the same
container. Our aim is to achieve standardization by providing a
common/generic data structure for a multitude of data types. See the
`wiki <https://github.com/G-Node/nix/wiki>`__ or the
`introduction <./getting_started.md>`__ for more information.

.. raw:: html

   <!-- NIX originates from neurosciences but the generic nature of the
   underlying data model makes it applicable to a much wider range. In
   fact we would be happy to know of datasets that cannot be represented
   in NIX. -->

The current implementations store the actual data using the
`HDF5 <http://www.hdfgroup.org/>`__ file format as a storage backend.

*NIX* emerged from the activities of the Electrophysiology Task Force of
the `INCF <http://www.incf.org>`__ Datasharing Program (2010-2015). It
is a registered research resource with the
`RRID:SCR_016196 <https://scicrunch.org/resources/Any/record/nlx_144509-1/SCR_016196/resolver?q=SCR_016196&l=SCR_016196>`__.

Contributing
============

Any kind of contribution is welcome! This includes reporting bugs and
issues. If you want to contribute to code or documentation please refer
to the `contributing
guide <https://github.com/G-Node/nix/blob/master/CONTRIBUTING.md>`__.

License
=======

This project is open source published under the BSD-3 license see
`license file <https://github.com/G-Node/nix/blob/master/LICENSE>`__ for
details.

Citing
======

If you use *NIX*, it would be much appreciated if you would cite it in
publications with its identifier RRID:SCR_016196 and/or the reference:

*Stoewer A, Kellner CJ, Benda J, Wachtler T and Grewe J (2014). File
format and library for neuroscience data and metadata. Front.
Neuroinform. Conference Abstract: Neuroinformatics 2014. doi:
10.3389/conf.fninf.2014.18.00027*

Referenced by
=============

-  Dragly et al (2018)
   `doi:10.3389/fninf.2018.000169 <https://doi.org/10.3389/fninf.2018.000169>`__
-  Papez et al (2017)
   `doi:10.3389/fninf.2017.00024 <https://doi.org/10.3389/fninf.2017.00024>`__
-  Grewe et al (2017)
   `doi:10.1073/pnas.1615561114 <https://doi.org/10.1073/pnas.1615561114>`__
-  Vanek et al (2016)
   `doi:10.1109/informatics.2015.7377849 <https://doi.org/10.1109/informatics.2015.7377849>`__
-  Rübel et al (2016)
   `doi:10.3389/fninf.2016.00048 <https://doi.org/10.3389/fninf.2016.00048>`__
-  Denker et al (2016)
   `doi:10.1007/978-3-319-50862-7_5 <https://doi.org/doi:10.1007/978-3-319-50862-7_5>`__
-  Teeters et al (2015)
   `doi:10.1016/j.neuron.2015.10.025 <https://doi.org/doi:10.1016/j.neuron.2015.10.025>`__

--------------

.. toctree::
   installation
   ecosystem
   :maxdepth: 1
   :caption: Getting started:

.. toctree::
   getting_started
   data_model
   relacs
   :maxdepth: 1
   :caption: Introduction:

.. toctree::
   contact
   faq
   :caption: Troubleshooting:
   :maxdepth: 1

.. toctree::
   versioning
   release_instructions
   :maxdepth: 1
   :caption: Misc

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
