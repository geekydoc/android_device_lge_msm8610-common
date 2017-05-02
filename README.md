This branch is to be used for building TWRP recovery for LG L Fino

### BUILD INSTRUCTIONS

It is suggested that TWRP is built in a seperate build environment, using one of the available TWRP minimal manifests at Omnirom's Github repositories. The LineageOS minimal manifest is preferred:

* Enter the source directory and initialize the repository:
```
repo init -u git://github.com/minimal-manifest-twrp/platform_manifest_twrp_lineageos.git -b twrp-14.1 
```
* Then download the device specific TWRP local manifest:
```
curl --create-dirs -L -o .repo/local_manifests/local_manifest.xml -O -L https://raw.githubusercontent.com/geekydoc/android_local_manifest/twrp-14.1/local_manifest.xml
```
* Sync the sources:
```
repo sync
```
* Enter the source directory
* Start the build:
```
. build/envsetup.sh
lunch lineage_[device]-userdebug
mka recoveryimage
```
Replacing [device] in the command above with l70pn or l70pds.
