#To change the output file name, or adjust what files are included in the ouput, see the "pep9cpudefs.pri" file

#Clean path
defineReplace(cpl){
    #Adjust the input path so that the correct slashes are used for the host shell & OS
    return($$system_path($$1))
}
#Clean path with force quote
defineReplace(cpq){
    return(\"$$cpl($$1)\")
}


#Prevent Windows from trying to parse the project three times per build.
#This interferes with the deployment script, and makes debugging hard since Qt attempts to debug the optimized program.
CONFIG -= debug_and_release \
    debug_and_release_target
QMAKE_MAC_SDK = macosx10.13

QtDir = $$clean_path($$[QT_INSTALL_LIBS]/..)
QtInstallerBin=$$clean_path($$QtDir/../../tools/Qtinstallerframework/3.0/bin)

!CONFIG(debug,debug|release):macx{
    #For some reason, the release flag is set in both debug and release.
    #So, the above Config(...) makes it so a disk image is only built in release mode.

    #Create necessary directory structure for disk image.
    QMAKE_POST_LINK += $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer/);
    #Copy over the executable and bundle it with its dependencies
    QMAKE_POST_LINK += $${QMAKE_COPY_DIR} $$cpq($$OUT_PWD/$$TARGET".app") $$cpq($$OUT_PWD/Installer/);
    QMAKE_POST_LINK += $$cpq($$QtDir/bin/macdeployqt) $$cpq($$OUT_PWD/Installer/$$TARGET".app");
    #Use HDIUtil to make a folder into a read/write image
    QMAKE_POST_LINK += hdiutil create -volname $$TARGET -srcfolder $$cpq($$OUT_PWD/Installer) -attach -ov -format UDRW $$TARGET"Temp.dmg";
    #Link from the read/write image to the machine's Applications folder
    QMAKE_POST_LINK += ln -s /Applications /Volumes/$$TARGET/Applications;
    #Write all data files to image
    for(name,UNIVERSAL_DATA){
        QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PATH_PREFIX/$$name) /Volumes/$$TARGET &
    }
    for(name,MAC_DATA){
        QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PATH_PREFIX/$$name) /Volumes/$$TARGET &
    }
    #Unmount the image, and create a new compressed, readonly image.
    QMAKE_POST_LINK += hdiutil detach /Volumes/$$TARGET;
    QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$OUT_PWD/$$TARGET"Temp".dmg) $$cpq($$OUT_PWD/$$TARGET"Temp2".dmg);
    QMAKE_POST_LINK += hdiutil convert -format UDBZ -o $$cpq($$OUT_PWD/$$OUTPUT_INSTALLER_NAME".dmg") $$cpq($$OUT_PWD/$$TARGET"Temp2".dmg);
    #Remove the temporary read/write image.
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$cpq($$OUT_PWD/$$TARGET"Temp".dmg);
    QMAKE_POST_LINK += $${QMAKE_DEL_FILE} $$cpq($$OUT_PWD/$$TARGET"Temp2".dmg);
    #If QMAKE_POST_LINK stops working in a future version, QMAKE provides another way to add custom targets.
    #Use the method described in "Adding Custom Targets" on http://doc.qt.io/qt-5/qmake-advanced-usage.html.
    #Our deployment tool will be called anytime the application is sucessfully linked in release mode.
}

#Otherwise if the target is windows, but no installer framework exists
else:!CONFIG(debug,debug|release):win32:!exists($$QtInstallerBin/repogen.exe){
    warning("Aborting installer creations, since QT Installer Framework 3.0 is not installed.")
    warning("Please run the QT maintence tool and install QT Installer Framework 3.0.")
}
    #Otherwise build the installer for windows as normal.
else:!CONFIG(debug,debug|release):win32{
    #Directory where the repogen tool will put its output
    repoDir=$$cpq($$OUT_PWD/Repository/win32)
    #Create installer directory structure
    #These will be ignored if the target already exists
    QMAKE_POST_LINK += $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer) & \
        $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer/packages) & \
        $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer/packages/$$TARGET) & \
        $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer/packages/$$TARGET/meta) & \
        $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer/packages/$$TARGET/data) & \
        $${QMAKE_MKDIR} $$cpq($$OUT_PWD/Installer/config) &

    #Create a directory for update information
    !exists($$repoDir){
        QMAKE_POST_LINK += $${QMAKE_MKDIR} $$cpq($$repoDir) &
    }

    #Copy over files needed to create installer
    QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PWD/config/configwin32.xml) $$cpq($$OUT_PWD/Installer/config/config.xml) & \ #Copy Platform dependant config file
        $${QMAKE_COPY} $$cpq($$PWD/config/control.js) $$cpq($$OUT_PWD/Installer/config) & \ #Copy over installer control script
        $${QMAKE_COPY_DIR} $$cpq($$PWD/packages/$$TARGET) $$cpq($$OUT_PWD/Installer/packages/$$TARGET/meta) &  #Copy over all meta info files in the target packages directory

    #Copy over needed icons as set in defs file
    for(name,UNIVERSAL_ICONS){
        QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PATH_PREFIX/$$name) $$cpq($$OUT_PWD/Installer/config) &
    }
    for(name,WINDOWS_ICONS){
        QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PATH_PREFIX/$$name) $$cpq($$OUT_PWD/Installer/config) &
    }
    #Copy over additional data specified in defs file
    for(name,UNIVERSAL_DATA){
        QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PATH_PREFIX/$$name) $$cpq($$OUT_PWD/Installer/packages/$$TARGET/data) &
    }
    for(name,WINDOWS_DATA){
        QMAKE_POST_LINK += $${QMAKE_COPY} $$cpq($$PATH_PREFIX/$$name) $$cpq($$OUT_PWD/Installer/packages/$$TARGET/data) &
    }


    #Copy over executable to data directory
    QMAKE_POST_LINK +=  $${QMAKE_COPY} $$cpq($$OUT_PWD/$$TARGET".exe") $$cpq($$OUT_PWD/Installer/packages/$$TARGET/data) &

    #Execute windeployqt to copy needed binaries (dlls, etc).
    #See documentation here:
    #http://doc.qt.io/qt-5/windows-deployment.html
    QMAKE_POST_LINK += $$cpq($$QtDir/bin/windeployqt) --no-translations --no-system-d3d-compiler $$cpq($$OUT_PWD/Installer/packages/$$TARGET/data/$$TARGET".exe") &


    #The following two lines invoke QT Installer Framework executables. See the following link
    #for documentation on what the different comman line flags do.
    #http://doc.qt.io/qtinstallerframework/ifw-tools.html

    #Execute repository creator
    QMAKE_POST_LINK += $$cpq($$QtInstallerBin/repogen) --update-new-components -p $$cpq($$OUT_PWD/Installer/packages) $$repoDir &

    #Create installer using the qt binary creator
    QMAKE_POST_LINK += $$cpq($$QtInstallerBin/binarycreator) -c $$cpq($$OUT_PWD/Installer/config/config.xml) -p $$cpq($$OUT_PWD/Installer/packages) \
    $$cpq($$OUT_PWD/Installer/$$OUTPUT_INSTALLER_NAME) &

}

#Since there is no native QT deploy tool for Linux, one must be added in the project configuration
#This condition is to make sure that a tool was provided as an argument to qmake
else:linux:isEmpty(LINUX_DEPLOY){
    warning("Attempting a Linux build, but no path to the build tool was provided")
}

#Then linuxdeployqt is available, and it should be used to make a working installer for linux.
else:linux{
    message("This is where the linux build code will go")
}
