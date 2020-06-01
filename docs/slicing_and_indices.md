# Slicing, Indexing and Data retrieval

## Converting positions to indices

A position is converted to an index in several steps:

1. Scaling if approriate. This applies to Sampled and Range Dimensions if they contain units. **Note:** Incompatible units will always lead to an error!
2. Converting to an index depending on a) the dimension type and b) the *PositionMatching* rule.

Five possible rules of *PositionMatching* (we always assuming ascending order in the dimension):

1. **Less**: the index points to the last entry that is less than position.
2. **LessOrEqual**: the index points to the last entry that is less or equal to position.
3. **Equal**: the index points to the entry that exatly matches position.
4. **GreaterOrEqual**: the index points to the first entry that is greater or equal to position.
5. **Greater**: the index points to the first entry that is greater than position.

For the different DimensionTypes slightly different rules apply:

### SampledDimension

Assume a SampledDimension with *sampling_interval* = 1 and *offset* = -1
x = [-1, 0., 1, 2, 3, 4, 5, ... n]
The table below shows three example positions `p = -2, -1, 0` combined with each `PositionMatching` rule and the expected result.
| p  | index | PositionRule   | validity |
|----|-------|----------------|----------|
| -2 |       |  Less          |  false   |
| -2 |       |  LessOrEqual   |  false   |
| -2 |       |  Equal         |  false   |
| -2 |   0   |  GreaterOrEqual |  true    |
| -2 |   0   |  Greater       |  true    |
|    |       |                |          |
| -1 |       |  Less          |  false   |
| -1 |   0   |  LessOrEqual   |  true    |
| -1 |   0   |  Equal         |  true    |
| -1 |   0   |  GreaterOrEqual |  true    |
| -1 |   1   |  Greater       |  true    |
|    |       |                |          |
| 0  |   0   |  Less          |  true    |
| 0  |   1   |  LessOrEqual   |  true    |
| 0  |   1   |  Equal         |  true    |
| 0  |   1   |  GreaterOrEqual |  true    |
| 0  |   2   |  Greater       |  true    |

A position is invalid if it is less than offset and PositionRule is *Less*, *LessOrEqual*, or *Equal*.

### RangeDimension

Assume a *RangeDimension* with the following ticks:
ticks = {-1.9, 0, 3, 5.9}

| p  | index | PositionRule    | validity |
|----|-------|-----------------|----------|
| -2 |       |  Less           |  false   |
| -2 |       |  LessOrEqual    |  false   |
| -2 |       |  Equal          |  false   |
| -2 |   0   |  GreaterOrEqual |  true    |
| -2 |   0   |  Greater        |  true    |
|    |       |                 |          |
| -1 |   0   |  Less           |  true    |
| -1 |   0   |  LessOrEqual    |  true    |
| -1 |       |  Equal          |  false   |
| -1 |   1   |  GreaterOrEqual |  true    |
| -1 |   1   |  Greater        |  true    |
|    |       |                 |          |
| 0  |   0   |  Less           |  true    |
| 0  |   1   |  LessOrEqual    |  true    |
| 0  |   1   |  Equal          |  true    |
| 0  |   1   |  GreaterOrEqual |  true    |
| 0  |   2   |  Greater        |  true    |
|    |       |                 |          |
| 6  |   3   |  Less           |  true    |
| 6  |   3   |  LessOrEqual    |  true    |
| 6  |       |  Equal          |  false   |
| 6  |       |  GreaterOrEqual |  false   |
| 6  |       |  Greater        |  false   |

### SetDimension

Assume a *SetDimension* **without** labels:

| p  | index | PositionRule    | validity |
|----|-------|-----------------|----------|
| -2 |       |  Less           |  false   |
| -2 |       |  LessOrEqual    |  false   |
| -2 |       |  Equal          |  false   |
| -2 |   0   |  GreaterOrEqual |  true    |
| -2 |   0   |  Greater        |  true    |
|    |       |                 |          |
| 0  |   0   |  Less           |  true    |
| 0  |   0   |  LessOrEqual    |  true    |
| 0  |   0   |  Equal          |  true    |
| 0  |   0   |  GreaterOrEqual |  true    |
| 0  |   1   |  Greater        |  true    |
|    |       |                 |          |
| 6  |   6   |  Less           |  true    |
| 6  |   6   |  LessOrEqual    |  true    |
| 6  |   6   |  Equal          |  true    |
| 6  |   6   |  GreaterOrEqual |  true    |
| 6  |   7   |  Greater        |  true    |

Positions that are positive numbers will allways give a valid index.

Assume a *SetDimension* **with** labels:
labels = {"A", "B", "C"}

| p  | index | PositionRule    | validity |
|----|-------|-----------------|----------|
| -2 |       |  Less           |  false   |
| -2 |       |  LessOrEqual    |  false   |
| -2 |       |  Equal          |  false   |
| -2 |   0   |  GreaterOrEqual |  true    |
| -2 |   0   |  Greater        |  true    |
|    |       |                 |          |
| 0  |   0   |  Less           |  true    |
| 0  |   0   |  LessOrEqual    |  true    |
| 0  |   0   |  Equal          |  true    |
| 0  |   0   |  GreaterOrEqual |  true    |
| 0  |   1   |  Greater        |  true    |
|    |       |                 |          |
| 6  |   2   |  Less           |  true    |
| 6  |   2   |  LessOrEqual    |  true    |
| 6  |       |  Equal          |  false   |
| 6  |       |  GreaterOrEqual |  false   |
| 6  |       |  Greater        |  false   |

Positions that exceed the label count will be invalid unless *Less* or *LessOrEqual* rules are applied.

## Ranges along dimensions

In version 1.4.x ranges along a dimension are **inclusive** that is, start and end position are included in the range:
$$range = [start ... end].$$

Numpy, for example, does not include the end position:
$$range = [start ... end)$$

With NIX version 1.4.5 (nixpy 1.4.9x) we allow to choose which behavior is desired. The *RangeMatch* enum contains flags to control this behavior.

### *RangeMatching* enumeration

1. *RangeMatch::Inclusive*: all positions (pos) that match the relation $$start\_pos <= pos <= end\_pos$$ are included in the range.
2. *RangeMatch::Exclusive*: all positions (pos) that match the relation $$start\_pos <= pos < end\_pos$$ are included in the range.

To find indices, libraries apply *PositionMatch::GreaterOrEqual* to the start position and *PositionMatch::LessOrEqual* (*RangeMatch::Inclusive*) or *PositionMatch::Less* (*RangeMatch::Exclusive*) to the end_position.

**Note:** Versions < 1.5 apply  *RangeMatch::Inclusive* by default, versions >= 1.5 will apply *RangeMatch::Exclusive* by default!

### Validity of ranges

Validity of ranges depends on the dimension type and the RangeMatch rule. The current implementation in NIX will swap start and end positions if end < start. This swapping is done **before** an index is calculated.
*RangeMatch::Exclusive* might lead to the situation that the end index is less than the start index. This will lead to an **invalid** range. If start and end indices are the same the range is **valid**.

## Data retrieval via Tag and MultiTag

*Tag*s and *MultiTag*s refer to *DataArrays* to tag points or regions in the data. Position(s) and extent(s) of the *Tag/MTag* are applied to all *DataArrays* that are linked to the *Tag/MTag* as references.

The *Tags* may provide less position entries than the referenced DataArray has dimensions. This implies that the full extent of the "remaining" dimension(s) is tagged. **Note:** A Tag without a single position/extent entry is **invalid**!

### Example case

A *DataArray* ("Recordings") stores data from several parallel recordings. The data is regularly sampled in time. Dimensionality of the DataArray is 2, it thus has two dimension descriptors; one *SampledDimension*s (the time) and one *SetDimension* (the recording channels; dimensions = [SampledDimension(sampling_interval=0.1, label="time", unit="s"), SetDimension(labels=["Channel A" ,"Channel B", "Channel C"])].

A *Tag* is used to annotate the temporal segment starting at time 1.0 with an extent of 1s in which some condition is ON. The condition affects only channel B.

"Condition ON": Tag with position [1.0, 1], extent [1.0, 0], references={"Recordings"}. Position and extent are both 2-element vectors. One entry for each dimension of the data. In pre 1.5 libraries, the extent along the second dimension, i.e. the recording channels, needs to be 0.0. Since 1.5, an extent of 1 would (in the default behavior) also only tag channel B.

When calling *Tag::taggedData* functions a data slice of the shape (11, 1) would be returned when using the *RangeMatch::Inclusive* behavior (shape of (10, 1) if *RangeMatch::Exclusive* is used). **Note:** even if the extent is 0.0 along the second dimension, one entry along this dimension is returned.

When tagging the same DataArray with a second Tag that provides only information for one dimension the tagged region is implcitely assumed to include the full extent of the second dimension:

"Condition B": Tag with position [0.5] and extent[0.5] reference=["Recordings"].
Retrieving data from the Tag will return a slice with the shape (6, 3) for *RangeMatch::Inclusive* and (5, 3) for *RangeMatch::Exclusive*.

Methods to return data slices of abritray DataArrays (e.g. *util::dataSlice* method) apply the same rules.

### Features

The "features" attached to a *Tag* are similar to "references" but the semantic meaning is slightly different. Features describe properties of the tagged region in the referenced Data. This could be, for example, a stimulus, or an analysed feature of the tagged region.

Features are linked to the *Tag/MTag* with a certain *LinkType*. There are three link types:

1. *Tagged*: Position and extent of the Tag also apply to the feature.
2. *Untagged*: The full extent of the linked data is a characteristic of tagged region in the references.
3. *Indexed*: Usually only applied with *MultiTags* which tag multiple positions/region. For each tagged position, there is an entry in the first dimension of the linked feature.

Feature data can can be read out using the *featureData* functions on Tag and MultiTag.
