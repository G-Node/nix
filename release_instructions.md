Creating a NIX release
----------------------

*Creating Debian Packages and publishing on Launchpad*

This is at least one possible way of doing it...

**Preparatory stuff**

- install all the packages you need for creating debian packages (see ubuntu packaging guideline
- create PGP Key and activate it on Launchpad
- check out the debian branch and merge the master
- change the debian/changelog file


**Building source packages**


- Create an empty directory somewhere.
````
   cd ~/releases
   mkdir nix
   cd nix
````

- Get the tarball. E.g. nix-1.1.0.tar.gz file from github (not *.zip).
   
- Unpack the tarball within the new folder
````
	   tar -xvf nix-1.1.0.tar.gz
````

- Copy the debian folder from the original folder into the source folder.
````
	   cp -r coding/nix/debian realeases/nix/nix-1.1.0/
````

- Have dh_make create the ../nix_1.1.0.orig.tar.gz file. It will ask for the package type just give 's'.
````	
	cd nix-1.1.0
	dh_make -f  ../nix-1.1.0.tar.gz 
````

- Create the source package and have it signed.
````
	debuild -S -sa

````

- Upload the package to launchpad and keep your fingers crossed.
````
	cd ..
	dput ppa:gnode/nix nix_1.1.0-5_source.changes
````


***Packages for multiple ubuntu series***
- Change the changelog to name the series you want 
```` 
     nix (1.1.0-5) xenial; urgency=medium

       * Set to version 1.1.0
````
You will have to increment the version number for each upload to launchpad (1.1.0-5 will change to 1.1.0-6).

- Upload and wait :)


***Packages for nixpy***

Packaging *nixpy* is similar except for the naming. The tarball you
download from github will be called "nixpy-1.1.0.tar.gz". The package,
however, is call *python-nix*. It is easiest to rename the tarball:

````
	mv nixpy-1.1.0.tar.gz python-nix-1.1.0.tar.gz
````

The rest is the same as before:

- Unpack tarball.
- Copy debian folder.
- If needed update changelog.
- Have ````dh_make``` build the orig tarball.
- Create singed source package with ```debuild```.
- upload source package to Launchpad. 
````dput ppa:gnode/nix python-nix_1.1.0-3_source.changes````
