# FAQ

## Q: Why sampling interval in the sampled dimension, not sampling rate?
Because the interval is the more **general** term. It can also be
applied to dimensions that do not extend in time but for example
space.

## Q: What does this error mean? “Cannot open a nonexistent file in ReadOnly mode”
Arises when the provided file path is invalid and you try to open it
with the nix::FileMode::ReadOnly mode. Solution: provide the path to
an existing file to be opened in ReadOnly mode or use the
nix::FileMode::ReadWrite or nix::FileMode::Overwrite flag to create a
new one.
