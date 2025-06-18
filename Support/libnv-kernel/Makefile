# The actual NVIDIA source we are building with.
# TODO(spotlightishere): Is there a better way for us to do this?
SOURCE_DIR := $(PROJECT_DIR)/Support/open-gpu-kernel-modules/src/nvidia

# The effective platform we're building for (e.g. `macosx`, `driverkit`).
# (This is going to break in future Xcode builds, isn't it...)
PLATFORM_NAME := $(SUPPORTED_PLATFORMS)

# The output directory of our library.
NV_KERNEL_A := $(BUILT_PRODUCTS_DIR)/nv-kernel-$(PLATFORM_NAME).a

# The directory used to store built object files.
OUTPUTDIR := $(OBJECT_FILE_DIR)
OUTPUTDIR_ABSOLUTE := $(OBJECT_FILE_DIR)

# Include the genuine Makefile.
include $(SOURCE_DIR)/Makefile

# macOS needs two defines for the source to build.
CFLAGS += -DNV_UNIX=1
CFLAGS += -DNV_BSD=1

# TODO(spotlightishere): This helps with debugging; remove eventually.
CFLAGS += -DPORT_MEM_TRACK_PRINT_LEVEL=1
CFLAGS += -DDEBUG=1
CFLAGS += -g
CFLAGS += -O0

# We may be building for DriverKit.
# Explicitly set SDKROOT.
CFLAGS += -isysroot $(SDKROOT)

# Create a fake library for Xcode to use.
faux_nv_kernel_library: $(OBJS)
	# Unlike the default `nv-kernel.o` action,
	# we can just link all objects.
	ar rcs $(NV_KERNEL_A) $(OBJS)
