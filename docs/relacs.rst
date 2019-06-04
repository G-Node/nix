.. toctree::
   :maxdepth: 1


Integration into relacs
=======================

.. |tux| image:: ./images/relacstux.png
   :width: 100px

|tux| `Relacs <relacs.net>`__ is a highly custamizable recording tool for
electrophysiology. It one use-case showing how to integrate automated
data storage and data annotation into the recording process.

Relacs overview
---------------

In the `Neuroethology group <www.neuroetho.uni-tuebingen.de>`__ relacs
is used to record the activity of single neurons during
electrophysiological experiments.

.. figure:: ./images/relacs_ephys.png
   :alt: relacs user interface
   :align: left
   :figwidth: 100 %

   The relacs user interface. The ui is divided into four panels. Top
   left: Filter and detector plugins that are used to detect events in
   the recorded data (such as action-potentials, spikes). Top right:
   Main panel showing the four recorded traces (membrane potential,
   fish's electric organ discharge global, same at a reference
   location, the stimulus applied during the experiment) yellow dots
   indicate detected events. Bottom left: diverse control panels, or
   other displays. Bottom right: Space the currently active research
   protocol (RePro) can use to display data or interact with the
   experimenter.

Several traces are recorded in parallel (usually sampled with 20, 40,
or 100 kHz per channel 16 bit, depending on experimental requirements).

1. The neuron's membrane voltage (V-1).
2. The fish's electric organ discharge (EOD), global measurement.
3. The local measurement of the EOD (LocalEOD-1).
4. The stimulus put into the tank (GlobalEFieldStimulus).
5. The times of detected spikes (Spikes-1).
6. Sometimes the times of EOD discharges and detected chirps.


Metadata
--------

Relacs knows a lot of metadata that are either configured before the
experiment was started (for example the parameters used when measuring
a firing rate vs intensity curve, the left figure below) or has to be
entered by the user when storing the data (right figure below) or are
derived from the recorded data (average firing rate, the EOD
frequency, etc).

.. |options| image:: ./images/relacs_ephys_options.png
   :width: 50 %

.. |dialog| image:: ./images/relacs_ephys_mdata.png
   :width: 50 %

+----------------------+
| |options| | |dialog| |
+----------------------+


A recording session
-------------------

Once the recording of a neuron has been established and we are ready
to start stimulating either pre-defined macros are used to control the
experiments or the experimenter starts research protocols (RePros)
manually.

When recording the data time is not necessarily real time. That is,
the data is only recorded when an active research protocol (RePro)
sets the acquisition active and thus the data time is only runs when
the acquisition is active. The figure below illustrates this.

.. figure:: ./images/relacs_timeline.png
   :alt: data time vs. real time
   :align: left
   :figwidth: 100 %

   A recording session becomes active once the acquisition is switched
   on (red bar). Data acquisition, however, is only active, and data
   is stored in the NIX file, when the currently running RePros
   demands it. In this example session there are two RePros active,
   the so called "Baseline" (green) and the "fi-curve" (blue). Within
   the time the fi-curve RePro is active three stimuli are presented
   (light blue). For each stimulus presentation the fi-curve RePro
   applies a stimulus with a different intensity (contrast, blue
   numbers).

Representation in NIX
---------------------

So much for the background. In the following we will illustrate how
such data is persisted in NIX files and how to work with the
data. Since relacs is programmed in C++ the *nixio* c++ library is
used for this. The code below uses the python implementation *nixpy*.

.. figure:: ./images/relacs_tagging.png
   :alt: tagging repros and stimuli
   :align: left
   :figwidth: 100 %

   All four recorded traces and all event traces (only the spike
   events are shown here) are stored in ``nix::DataArrays``. Whenever
   a RePro is active a ``nix::Tag`` is created that stores the start
   time and temporal extent (in data time, see above) and links to
   metadata ``nix::Section`` that stores the static RePro parameters,
   i.e. those that do not change during this RePro run. A RePro may
   apply certain stimuli (the fi-curve RePro) and these stimulus times
   and extents are stored in a ``nix::MultiTag`` entity. It further
   links to some more metadata entries. Some paramters, in this case
   the stimulus "contrast" changes with each stimulus
   presentation. This information is not stored in the metadata but is
   considered a *feature* of the stimulus presentation. That is, we
   create a ``nix::DataArray`` to store these numbers, one for each
   stimulus presentation, and link it to the ``nix::MultiTag`` as an
   **indexed** feature (see `adding features
   <tagging.html#adding-features>`__ for details).


Working with relacs-flavored NIX files
--------------------------------------
