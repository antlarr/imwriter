# imwriter

Downloads and writes raw disk images and operating systems to usb drives
The idea is to have a tool that every distribution can use to download
and write usb images. 

# Warning

This version is not yet finished and is not suitable for regular users.

Only try it if you're a developer who want to help with the development.

The backend works but the most of the GUI is missing (for example, progress
is reported on stdout instead of a nice progress bar, which is in my TODO).

# What works

- Downloads disk image & write it to usb disk on-the-fly
- Downloads checksum file from the distribution site
- Checks the checksum of written data, also on-the-fly 
- Checks if the PGP signature of the downloaded checksum file is valid (if 
  present)
- Progress percentage (on stdout)
- All reports of checksum and pgp validity are reported on stdout
- Downloads the list of available images from an xml file in the distribution
  site although by default uses a local one that gets installed.

# What doesn't work (yet)

- GUI to see the progress of a write operation and messages like
  checksum/signature validation 
- GUI to cancel a write operation while running
- GUI to filter available images by vendor/arch/version ...
- Disable devices being used
- Parallel writing of different (or same) images to different devices 
  (this should work in fact, but haven't tested it yet).
- Confirm question when writing
- Combobox to select distribution and have a builtin list of
  imagesrepository.xml files for each distribution
- Drop an imagesrepository.xml file to images section to use it.
- Drop a file to a device to start writing it without downloading
- Cache downloaded images/option to cache all images (useful for distribution 
  booths at congresses)
- Allow an <iconurl> tag in the imagerepository.xml file to allow distributions
  to customize the used icon instead of using one of the predefined icons

# Note

In order for gpg verification to work, suse and opensuse build keys
have to be imported. Usually this is acomplished with the following
two commands:

gpg --import /usr/lib/rpm/gnupg/keys/gpg-pubkey-307e3d54-4be01a65.asc
gpg --import /usr/lib/rpm/gnupg/keys/gpg-pubkey-3dbdc284-53674dd4.asc

if any of the keys can't be found, be sure to check the
openSUSE-build-key package is installed and import the keys it contains

# Requisites

## openSUSE:

zypper install kcoreaddons-devel ki18n-devel gpgmepp5-devel solid-devel extra-cmake-modules libQt5DBus-devel libopenssl-devel 

## Debian:

apt-get install libkf5coreaddons-dev libkf5i18n-dev libkf5gpgmepp-dev libkf5solid-dev extra-cmake-modules libssl-dev

# How to Build and Install

mkdir build && cd build && cmake .. && make && sudo make install

# How to test

Just run imwriter

drag an image and drop it over a usb device (note that this application
doesn't ask for confirmation yet, it'll write wherever you tell it to write
without hesitating)

# How to Uninstall

cd build && sudo make uninstall


