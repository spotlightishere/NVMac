//
//  shim_kernel.h
//  libnv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-17.
//

#ifndef shim_kernel_h
#define shim_kernel_h

#pragma mark - Imports
// IOKit types are within IOKit.
#include <IOKit/IOBufferMemoryDescriptor.h>
#include <IOKit/IOCommandPool.h>
#include <IOKit/IODMACommand.h>
#include <IOKit/IOLib.h>
#include <IOKit/pci/IOPCIDevice.h>

// OS types are present within <libkern/c++>.
#include <libkern/c++/OSDictionary.h>
#include <libkern/c++/OSNumber.h>
#include <libkern/c++/OSString.h>

// `strcpy`/[...]
#include <libkern/OSTypes.h>
#include <libkern/libkern.h>

#pragma mark - C types

// In actual kernel code, `mask_task_self`
// represents the entire kernel, not our own KEXT task.
// We'll want to instead use `current_task`.
#define nvd_task current_task

// This should be present in <mach/mach_traps.h>,
// but for some reason, this header refuses to import.
kern_return_t pid_for_task(task_t task, int32_t* pid);

#endif
