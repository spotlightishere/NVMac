# The actual NVIDIA source we are building with.
# TODO(spotlightishere): Is there a better way for us to do this?
SOURCE_DIR := $(PROJECT_DIR)/Support/open-gpu-kernel-modules/src/nvidia

# The output directory of our library.
NV_KERNEL_A := $(BUILT_PRODUCTS_DIR)/nv-kernel.a

# Include the genuine Makefile.
include $(SOURCE_DIR)/Makefile

# macOS needs two defines for the source to build.
CFLAGS += -DNV_UNIX=1
CFLAGS += -DNV_BSD=1

# Create a fake library for Xcode to use.
faux_nv_kernel_library: $(OBJS)
	# Unlike the default `nv-kernel.o` action,
	# we can just link all objects.
	ar rcs $(NV_KERNEL_A) $(OBJS)
