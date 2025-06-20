//
//  stubs.c
//  nv-darwin
//
//  Created by Spotlight Deveaux on 2025-06-05.
//

#if TARGET_OS_DRIVERKIT
#include <DriverKit/IOLib.h>
#else
#include <IOKit/IOLib.h>
#endif

#define STUBBED(name)                                                          \
    void name(void) {                                                          \
        IOLog("Stubbed function " #name);                                      \
    }

STUBBED(nv_acquire_fabric_mgmt_cap)
STUBBED(nv_acquire_mmap_lock)
STUBBED(nv_add_mapping_context_to_file)
STUBBED(nv_cancel_nano_timer)
STUBBED(nv_check_usermap_access_params)
STUBBED(nv_control_soc_irqs)
STUBBED(nv_create_nano_timer)
STUBBED(nv_destroy_nano_timer)
STUBBED(nv_flush_coherent_cpu_cache_range)
STUBBED(nv_get_adapter_state)
STUBBED(nv_get_all_mappings_revoked_locked)
STUBBED(nv_get_device_memory_config)
STUBBED(nv_get_egm_info)
STUBBED(nv_get_event)
STUBBED(nv_get_file_private)
STUBBED(nv_get_num_phys_pages)
STUBBED(nv_get_phys_pages)
STUBBED(nv_get_screen_info)
STUBBED(nv_get_syncpoint_aperture)
STUBBED(nv_get_updated_emu_seg)
STUBBED(nv_i2c_add_adapter)
STUBBED(nv_i2c_del_adapter)
STUBBED(nv_indicate_idle)
STUBBED(nv_indicate_not_idle)
STUBBED(nv_match_gpu_os_info)
STUBBED(nv_parms)
STUBBED(nv_post_event)
STUBBED(nv_put_file_private)
STUBBED(nv_register_peer_io_mem)
STUBBED(nv_register_phys_pages)
STUBBED(nv_register_sgt)
STUBBED(nv_register_user_pages)
STUBBED(nv_release_mmap_lock)
STUBBED(nv_revoke_gpu_mappings)
STUBBED(nv_s2idle_pm_configured)
STUBBED(nv_schedule_uvm_drain_p2p)
STUBBED(nv_schedule_uvm_isr)
STUBBED(nv_schedule_uvm_resume_p2p)
STUBBED(nv_set_safe_to_mmap_locked)
STUBBED(nv_start_nano_timer)
STUBBED(nv_start_rc_timer)
STUBBED(nv_stop_rc_timer)
STUBBED(nv_unregister_peer_io_mem)
STUBBED(nv_unregister_phys_pages)
STUBBED(nv_unregister_sgt)
STUBBED(nv_unregister_user_pages)
STUBBED(nvidia_caps_root)
STUBBED(os_alloc_pages_node)
STUBBED(os_alloc_wait_queue)
STUBBED(os_call_vgpu_vfio)
STUBBED(os_close_file)
STUBBED(os_count_tail_pages)
STUBBED(os_dump_stack)
STUBBED(os_enable_pci_req_atomics)
STUBBED(os_flush_work_queue)
STUBBED(os_free_wait_queue)
STUBBED(os_io_read_byte)
STUBBED(os_io_read_dword)
STUBBED(os_io_read_word)
STUBBED(os_io_write_byte)
STUBBED(os_io_write_dword)
STUBBED(os_io_write_word)
STUBBED(os_is_queue_flush_ongoing)
STUBBED(os_lock_user_pages)
STUBBED(os_lookup_user_io_memory)
STUBBED(os_match_mmap_offset)
STUBBED(os_numa_add_gpu_memory)
STUBBED(os_numa_memblock_size)
STUBBED(os_numa_remove_gpu_memory)
STUBBED(os_nv_cap_close_fd)
STUBBED(os_nv_cap_destroy_entry)
STUBBED(os_nv_cap_validate_and_dup_fd)
STUBBED(os_offline_page_at_address)
STUBBED(os_open_temporary_file)
STUBBED(os_queue_work_item)
STUBBED(os_read_file)
STUBBED(os_schedule)
STUBBED(os_unlock_user_pages)
STUBBED(os_wait_interruptible)
STUBBED(os_wait_uninterruptible)
STUBBED(os_wake_up)
STUBBED(os_write_file)
