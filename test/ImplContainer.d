#!/usr/sbin/dtrace -s

#pragma D option quiet

pid$target::*ImplContainer(*):entry
{
  @["ImplContainer::ctor_total"] = count();
}

pid$target::*ImplContainer(std*shared_ptr*):entry
{
  @["ImplContainer::shrd_ctor_total"] = count();
}

pid$target::*ImplContainer(nix??base??ImplContainer*):entry
{
  @["ImplContainer::ctor_copy"] = count();
}

pid$target::*ImplContainer():entry
{
  @["ImplContainer::ctor_empty"] = count();
}

pid$target::*ImplContainer(std*shared_ptr*const?):entry
{
  @["ImplContainer::shrd_ctor_copy"] = count();

  /* this will print the stack trace how we got here */
  /* ustack(); */
}

/* File */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IFile???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["File::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IFile??const?):entry
{
  @["File::shrd_ctor_copy"] = count();
  /* ustack(); */
}

/* Block */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IBlock???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Block::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IBlock??const?):entry
{
  @["Block::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* Property */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IProperty???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Property::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IProperty??const?):entry
{
  @["Property::shrd_ctor_copy"] = count();
  /* ustack(); */
}

/* Dimension */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IDimension???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Dimension::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IDimension??const?):entry
{
  @["Dimension::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* RangeDimension */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IRangeDimension???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Dimension[Range]::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IRangeDimension??const?):entry
{
  @["Dimension[Range]::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* SetDimension */
pid$target::*ImplContainer(std*shared_ptr?nix??base??ISetDimension???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Dimension[Set]::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??ISetDimension??const?):entry
{
  @["Dimension[Set]::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* SampledDimension */
pid$target::*ImplContainer(std*shared_ptr?nix??base??ISampledDimension???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Dimension[Sampled]::shrd_ctor_move"] = count();
}

/*
pid$target::*ImplContainer(std*shared_ptr?nix??base??ISampledDimension??const?):entry
{
  @["Dimension[Sampled]::shrd_ctor_copy"] = count();
}
*/


/* Section */
pid$target::*ImplContainer(std*shared_ptr?nix??base??ISection???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Section::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??ISection??const?):entry
{
  @["Section::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* DataArray */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IDataArray???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["DataArray::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IDataArray??const?):entry
{
  @["DataArray::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* SimpleTag */
pid$target::*ImplContainer(std*shared_ptr?nix??base??ISimpleTag???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["SimpleTag::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??ISimpleTag??const?):entry
{
  @["SimpleTag::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* Source */
pid$target::*ImplContainer(std*shared_ptr?nix??base??ISource???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Source::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??ISource??const?):entry
{
  @["Source::shrd_ctor_copy"] = count();
  /* ustack(); */
}


/* Representation */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IRepresentation???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["Representation::shrd_ctor_move"] = count();
}

/*
pid$target::*ImplContainer(std*shared_ptr?nix??base??*Representation??const?):entry
{
  @["Representation::shrd_ctor_copy"] = count();
}
*/

/* DataTag */
pid$target::*ImplContainer(std*shared_ptr?nix??base??IDataTag???):entry
{
  @["ImplContainer::shrd_ctor_move"] = count();
  @["DataTag::shrd_ctor_move"] = count();
}

pid$target::*ImplContainer(std*shared_ptr?nix??base??IDataTag??const?):entry
{
  @["DataTag::shrd_ctor_copy"] = count();
  /* ustack(); */
}