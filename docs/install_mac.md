Installation macOS
-------------------------

The easiest way to get NIX is via [homebrew](http://brew.sh).
We are included in [homebrew science](http://brew.sh/homebrew-science/) as `nixio`.
Install nix, including missing dependencies, with:

```shell
brew install homebrew/science/nixio

# OR: tap into homebrew-science
brew tap homebrew/science
brew install nixio

# If you want the latest development version:
# NB: replace 'install', with 'reinstall' to upgrade
#     an existing installation
brew install --HEAD nixio
```
