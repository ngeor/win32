$VBoxManage = "C:\Program Files\Oracle\VirtualBox\VBoxManage.exe"
$VMName = "W2KAgent"
$VMUsername = "Nikolaos Georgiou"
$VMPassword = ""

function Get-VMState {
    $Lines = $(& $VBoxManage --nologo showvminfo $VMName --machinereadable)
    $Lines = $Lines -split [Environment]::NewLine | where {$_.StartsWith("VMState=")}
    $Lines = $Lines -join "" -split "="
    $Lines[1].Replace('"', '')
}

function Start-VM {
    & $VBoxManage --nologo startvm $VMName --type headless
}

function Close-VM {
    & $VBoxManage --nologo controlvm $VMName poweroff
    & $VBoxManage --nologo snapshot $VMName restorecurrent
}

function Wait-VM {
    sleep 30
    $attempts = 1
    while ($attempts -lt 10) {
        echo "Waiting for VM to be ready, attempt $attempts"
        & $VBoxManage --nologo guestcontrol $VMName stat `
            --username $VMUsername `
            --password `"$VMPassword`" `
            "C:\"
        if ($?) {
            break
        }
        sleep 5
        $attempts++
    }
}

function Copy-IntoVM {
    $attempts = 1
    while ($attempts -lt 10) {
        echo "Copying source into VM, attempt $attempts"
        & $VBoxManage --nologo guestcontrol $VMName copyto `
            --username $VMUsername `
            --password `"$VMPassword`" `
            --recursive `
            --target-directory C:\MySite `
            .\vc6
        if ($?) {
            break
        }
        sleep 5
        $attempts++
    }
}

function Run-Build {
    # ignoring errors in case it's a broken build
    # Write-Host instead of echo otherwise it becomes part of the result
    Write-Host "Running build in VM"
    & $VBoxManage --nologo guestcontrol $VMName run `
        --username $VMUsername `
        --password `"$VMPassword`" `
        --wait-stdout `
        --exe "C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDEV.COM" `
        -- "C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\MSDEV.COM" C:\MySite\vc6.dsw /OUT C:\MySite\build.log /MAKE ALL /REBUILD | Out-Host
    return $?
}

function Copy-OutOfVM {
    $attempts = 1
    while ($attempts -lt 10) {
        echo "Copying files out of VM, attempt $attempts"
        & $VBoxManage --nologo guestcontrol $VMName copyfrom `
            --username $VMUsername `
            --password `"$VMPassword`" `
            --recursive `
            --target-directory . `
            C:\MySite
        if ($?) {
            break
        }
        sleep 5
        $attempts++
    }
}

function Clear-OutputFolder {
    if (Test-Path "MySite") {
        del -Recurse -Force MySite
    }
}

$State = Get-VMState
if ($State -eq "poweroff") {
    echo "It is off"
    Start-VM
    Clear-OutputFolder
    Wait-VM
    Copy-IntoVM
    $BuildSucceeded = Run-Build
    Copy-OutOfVM
    Close-VM
    if ($BuildSucceeded) {
        cat .\MySite\build.log
    } else {
        Write-Warning "Build failed, please check build log below"
        cat .\MySite\build.log
        Write-Warning "Build failed, please check build log above"
        exit 1
    }
} else {
    Write-Warning "VM was not powered off, aborting"
    exit 1
    # Close-VM
}
