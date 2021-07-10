#!/bin/bash
VBoxManage="/c/Program Files/Oracle/VirtualBox/VBoxManage.exe"
VM_NAME="Windows 2000 Professional"
Y_DRIVE="/c/Users/ngeor/Projects"
BUILD_ME="${Y_DRIVE}/BUILD_ME"
BUILD_LOG="${Y_DRIVE}/BUILD.LOG"
USERNAME="Nikolaos Georgiou"
PASSWORD=

function vm_state {
    "$VBoxManage" --nologo showvminfo "$VM_NAME" --machinereadable | grep 'VMState=' | cut -d= -f2 | tr -d \"
}

function start_vm {
    echo "Starting VM"
    "$VBoxManage" --nologo startvm "$VM_NAME"
    # wait a bit
    sleep 10
}

function close_vm {
    # shutdown
    echo "Closing VM"
    "$VBoxManage" --nologo controlvm "$VM_NAME" poweroff
    "$VBoxManage" --nologo snapshot "$VM_NAME" restorecurrent
}

function is_guest_control_ready {
    local attempts=1
    while [ $attempts -lt 10 ]; do
        echo "Checking if guest control is ready, attempt $attempts"
        "$VBoxManage" --nologo guestcontrol "$VM_NAME" stat --username "$USERNAME" --password "$PASSWORD" "C:\\"
        local result=$?
        if [ $result -eq 0 ]; then
            echo "Guest control seems to be ready"
            break
        fi
        ((attempts++))
        sleep 5
    done
}

function copy_dir {
    "$VBoxManage" --nologo guestcontrol "$VM_NAME" copyto --username "$USERNAME" --password "$PASSWORD" --recursive --target-directory "C:\\MySite" /c/Users/ngeor/Projects/github/win32/
}

STATE=$(vm_state)
echo $STATE
if [[ "$STATE" = "poweroff" ]]; then
    rm -f "${BUILD_LOG}"
    start_vm
    is_guest_control_ready
    copy_dir
    # ATTEMPTS=0
    # while [ $ATTEMPTS -lt 10 ]; do
    #     echo "Waiting for the build log to be created, attempt ${ATTEMPTS}"
    #     sleep 5
    #     ((ATTEMPTS++))
    #     if [[ -r ${BUILD_LOG} ]]; then
    #         echo "Build log found, bailing out"
    #         break
    #     fi
    # done


elif [[ "$STATE" = "running" ]]; then
    close_vm
fi
