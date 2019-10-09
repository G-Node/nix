# NIX file validity

## Hard vs Soft rules

The NIXIO libraries enforce the general NIX structure and requirements.  For instance, all data objects (Block, Group, DataArray, etc) must always have a name and a type and all the libraries and tools that can create NIX files enforce these rules.  Beyond the rules that the library enforces, there are also certain that are the responsibility of the user to follow.  These rules are not required in a working NIX file, but violating them is often indication that a file is _semantically invalid_ or simply incomplete.  We can separate all the validity rules into two categories, _hard_ and _soft_ rules, depending on their severity.

_Hard rules_ include the ones enforced by the library mentioned above, but also some rules that the users are expected to follow.  An example of these user-dependent _hard rules_ is that the number of Dimension objects attached to a DataArray must match the number of dimensions in the data itself.  Violating this rule makes the DataArray _semantically invalid_, but it does not prevent the use of the file in any way.

_Soft rules_ are less important than _hard rules_, as the name suggests.  Violating them does not make a file invalid, neither functionally nor semantically, but may limit certain functionality in the library or may be an indication of incomplete data descriptions.  An example of a _soft rule_ is the absence of `units` in a DataArray, or the use of a non-SI unit.  This rule violation prevents the acquisition of data from a DataArray through a Tag or MultiTag object when the units of the objects are scaled versions of the each other.

This document lists both the _hard rules_ and _soft rules_ that are required for creating semantically valid and complete NIX files, separated by object type.

## The NIX validator

The NIXIO libraries implement a validator that can check a file for both soft and hard rule violations and inconsistencies.  The output of these validators label hard rule violations as _errors_ and soft rule violations as _warnings_.

## Hard rules

### File objects
- Creation date must be set: HDF5 attribute `created_at`.

### Entities
- ID must be set: HDF5 attribute `entity_id`.
- Creation date must be set: HDF5 attribute `created_at`.
- Name must be set: HDF5 attribute `name`.
- Type must be set: HDF5 attribute `type`.

### Block and Group objects
- Rules for [Entities](#entities) apply.

### DataArray objects
- Rules for [Entities](#entities) apply.
- Data type must be set: HDF5 attribute `dtype`.
- Number of attached Dimension objects must match the number of dimensions in the data.
- Number of Ticks in any attached RangeDimension objects must match the length of the corresponding dimension in the data.
- Number of Labels in any attached SetDimension objects must match the length of the corresponding dimension in the data.

#### Dimension objects
- Dimension index must be positive: HDF5 attribute `index`.
- Dimension type (attribute) must match Dimension object type: HDF5 attribute `dimension_type`.
- RangeDimension rules:
  - Ticks must be set: HDF5 dataset `ticks`.
  - Ticks must be sorted in increasing order.
  - Unit must be set to an atomic SI unit: HDF5 attribute `unit`.
- SampledDimension rules:
  - Sampling interval must be set and have a positive value: HDF5 attribute `sampling_interval`.
  - Unit must be set to an atomic SI unit: HDF5 attribute `unit`.

### Tag objects
- Rules for [Entities](#entities) apply.
- Position must be set: HDF5 dataset `position`.
- If the Tag contains references, the length of `position` must match the number of dimensions in all the referenced DataArray objects.
- If the Tag contains an extent, the length of `extent` must match the number of dimensions in all the referenced DataArray objects (and the length of `position`).
- Units must be set to an atomic SI unit: HDF5 dataset `unit`.
- Number of units must match the number of dimensions in all the referenced DataArray objects.
- Units must be convertible to the corresponding units in all the referenced DataArray objects.

### MultiTag objects
- Rules for [Tag objects](#tag-objects) apply.
- Number of Positions must match number of Extents.
- Each element in the Positions and Extents arrays must match the number of dimensions in all the referenced DataArray objects.
- Multiple Positions and Extents are specified, so the first dimension of the Positions and Extents arrays does not need to match any other property of the data.

#### Feature objects
- Rules for [Entities](#entities) apply (excluding `name` and `type`).
- Data reference must be set: HDF5 group `data`.
- LinkType must be set and have a valid value: HDF5 attribute `link_type`.

### Source objects
- Rules for [Entities](#entities) apply.

### Section objects
- Rules for [Entities](#entities) apply.

#### Property objects
- Rules for [Entities](#entities) apply (excluding `type`).

## Soft rules

### File objects
- Version should be set: HDF5 attribute `version`.
- Format should be set: HDF5 attribute `format`.

### DataArray objects
- Unit should be SI or composite of SI units.
- If Polynomial Coefficients are set, the expansion origin should also be set and vice versa.

#### Dimension objects
- SampledDimension rules:
  - If Offset is set a valid unit must also be set: HDF5 attribute `offset`.

### Property objects
- If Value is set, it should also have a Unit: HDF5 attribute `unit`.
