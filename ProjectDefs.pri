#Installer Configuration
#Pep9 Version
PEPVER=82
#Name of the installation tool
OUTPUT_INSTALLER_NAME=$$TARGET"-Installer-"$$PEPVER
#Internal version number, unused as of 5/2/2018
VERSION_NUMBER = "1_0"
#If you want to use a DMG based installer for Mac, put anything in this field
#If you want to use a QT Installer Framework Base Installer, set the field to ""
MAC_USE_DMG_INSTALLER = "true"

#Data Configuration
#Look for all files below starting in this directory
PATH_PREFIX = $$PWD
#Each of the following variables should be a space separated list
UNIVERSAL_ICONS = ""
UNIVERSAL_DATA = "LICENSE"
MAC_ICONS = "" #No special icons for Mac
MAC_DATA = "" #No additional data for Mac
WINDOWS_ICONS = "images/icon.ico" "images/Pep8-icon.png"
WINDOWS_DATA = "" #No additional data for Windows
LINUX_ICONS = "" #No implementation for Linux
LINUX_DATA = "" #No implementation for Linux

#One of your target packages must always be $$TARGET
TARGET_PACKAGES.PACKAGES = $$TARGET
#For every extra package you would like to add
#give it a PACKAGE_NAME
#extTarg1.PACKAGE_NAME = "Data"
#And a space seperated list of files, based off of PWD
#Eventually, the operating system will need to be included as an extra set of data
#extTarg1.DATA = "Resources/MyExtraData.zip"
#And append it to TARGET_PACKAGES.EXTRA_DATA
#TARGET_PACKAGES.EXTRA_DATA += extTarg1

for(item,TARGET_PACKAGES.EXTRA_DATA){
    TARGET_PACKAGES.PACKAGES += $$eval($$item"."PACKAGE_NAME)
}
