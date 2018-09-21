Installation macOS
-------------------------


The easiest way to get NIX is via [homebrew](http://brew.sh).
G-Node homebrew formulae are maintained in the [G-Node tap](https://github.com/g-node/homebrew-pkg).
Install nix, including missing dependencies, with:

```shell

# tap the repository 
brew tap g-node/pkg

# install with
brew install g-node/pkg/nixio

# If you want the latest development version:
brew install --HEAD g-node/pkg/nixio

# to upgrade an existing installation to the latest release 
# replace 'install', with 'reinstall'

```
