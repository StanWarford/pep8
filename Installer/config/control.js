function func() {

}
function Controller() {
    var widget = gui.pageById(QInstaller.Introduction); // get the introduction wizard page
    if (widget != null){
        widget.packageManagerCoreTypeChanged.connect(onPackageManagerCoreTypeChanged);
    }
}

Controller.prototype.IntroductionPageCallback  = function(){
    var widget = gui.currentPageWidget();
    if (widget != null) {
		//Prevent add or remove package button from showing up by setting it's text to "", disabling it, and hiding it.
        console.log(Object.getOwnPropertyNames(widget));
        widget.findChild("PackageManagerRadioButton").visible = false;
        widget.findChild("PackageManagerRadioButton").enabled = false;
        widget.findChild("PackageManagerRadioButton").text = "";
        widget.findChild("UninstallerRadioButton").text = "Uninstall XML Tester"
        widget.findChild("UpdaterRadioButton").text = "Update XML Tester"
    }

}

Controller.prototype.LicenseAgreementPageCallback = function(){
    var widget = gui.currentPageWidget();
    if (widget != null) {
		//Stub out code to allow changing default of license agreement.
		//Helpful in debug to change to true.
        widget.AcceptLicenseRadioButton.checked = false;
    }
}

Controller.prototype.FinishedPageCallback = function(){
}

onPackageManagerCoreTypeChanged = function(){

	
    var widget = gui.pageById(QInstaller.Introduction);
    if (widget != null) {
		//Any time any event comes in, the PackageManagerRadioButton tries to re-enable itself.
		//So, every time a new button is clicked, make sure that PackageManagerRadioButton wasn't re-enabled.
        widget.findChild("PackageManagerRadioButton").visible = false;
        if(widget.findChild("PackageManagerRadioButton").checked==true){
			//If somehow PackageManagerRadioButton got checked, uncheck it and default to uninstall
            widget.findChild("UninstallerRadioButton").checked = true;
        }
    }
}
