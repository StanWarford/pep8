function Component(){
    gui.pageWidgetByObjectName("LicenseAgreementPage").entered.connect(changeLicenseLabels);
    installer.installationFinished.connect(this, Component.prototype.installationFinishedPageIsShown);
    installer.finishButtonClicked.connect(this, Component.prototype.installationFinished);

    //gui.pageWidgetByObjectName("Introduction").entered.connect(changeLicenseLabels);
}

Component.prototype.createOperations = function(){
    component.createOperations();
    if(installer.value("os") == "win"){
        component.addOperation("Execute", "@TargetDir@\\vc_redist.x64.exe","/layout","/passive", "/norestart","/quiet");
        component.addOperation("CreateShortcut", "@TargetDir@/Pep8.exe", "@StartMenuDir@/Pep8.lnk",
                    "workingDirectory=@TargetDir@","description=Run Pep8");
    }
}
Component.prototype.installationFinishedPageIsShown = function(){
    /*
    Attempt at adding another checkbox to add a desktop shortcut
    try {
        if (installer.isInstaller() && installer.status == QInstaller.Success) {
            installer.addWizardPageItem( component, "QCheckBox", installer.InstallationFinished );
        }
    } catch(e) {
        console.log(e);
    }*/
}

Component.prototype.installationFinished = function(){

}
changeLicenseLabels = function()
{
    //page = gui.pageWidgetByObjectName("LicenseAgreementPage");
    //page.AcceptLicenseLabel.setText("Yes I do!");
    //page.RejectLicenseLabel.setText("No I don't!");
}
