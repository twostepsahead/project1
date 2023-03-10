/* BEGIN CSTYLED */

/*
 * Copyright (c) 2009, Intel Corporation.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

/*
 * Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
 */

#include <sys/x86_archext.h>
#include <sys/vfs.h>
#include "drmP.h"
#include "drm.h"
#include "i915_drm.h"
#include "i915_drv.h"

#ifndef roundup
#define	roundup(x, y)   ((((x)+((y)-1))/(y))*(y))
#endif /* !roundup */

#define I915_GEM_GPU_DOMAINS	(~(I915_GEM_DOMAIN_CPU | I915_GEM_DOMAIN_GTT))

static timeout_id_t worktimer_id = NULL;

extern int drm_mm_init(struct drm_mm *mm,
		    unsigned long start, unsigned long size);
extern void drm_mm_put_block(struct drm_mm_node *cur);
extern int choose_addr(struct as *as, caddr_t *addrp, size_t len, offset_t off,
    int vacalign, uint_t flags);

static void
i915_gem_object_set_to_gpu_domain(struct drm_gem_object *obj,
				  uint32_t read_domains,
				  uint32_t write_domain);
static void i915_gem_object_flush_gpu_write_domain(struct drm_gem_object *obj);
static void i915_gem_object_flush_gtt_write_domain(struct drm_gem_object *obj);
static void i915_gem_object_flush_cpu_write_domain(struct drm_gem_object *obj);
static int i915_gem_object_set_to_gtt_domain(struct drm_gem_object *obj,
					     int write);
static int i915_gem_object_set_to_cpu_domain(struct drm_gem_object *obj,
					     int write);
static int i915_gem_object_set_cpu_read_domain_range(struct drm_gem_object *obj,
						     uint64_t offset,
						     uint64_t size);
static void i915_gem_object_set_to_full_cpu_read_domain(struct drm_gem_object *obj);
static void i915_gem_object_free_page_list(struct drm_gem_object *obj);
static int i915_gem_object_wait_rendering(struct drm_gem_object *obj);
static int i915_gem_object_get_page_list(struct drm_gem_object *obj);

static void
i915_gem_cleanup_ringbuffer(struct drm_device *dev);

/*ARGSUSED*/
int
i915_gem_init_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_init args;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_init *) data, sizeof(args));

	spin_lock(&dev->struct_mutex);

	if ((args.gtt_start >= args.gtt_end) ||
	    ((args.gtt_start & (PAGE_SIZE - 1)) != 0) ||
	    ((args.gtt_end & (PAGE_SIZE - 1)) != 0)) {
		spin_unlock(&dev->struct_mutex);
		DRM_ERROR("i915_gem_init_ioctel invalid arg 0x%lx args.start 0x%lx end 0x%lx", &args, args.gtt_start, args.gtt_end);
		return EINVAL;
	}

	dev->gtt_total = (uint32_t) (args.gtt_end - args.gtt_start);

	(void) drm_mm_init(&dev_priv->mm.gtt_space,
	    (unsigned long) args.gtt_start, dev->gtt_total);
	DRM_DEBUG("i915_gem_init_ioctl dev->gtt_total %x, dev_priv->mm.gtt_space 0x%x gtt_start 0x%lx", dev->gtt_total, dev_priv->mm.gtt_space, args.gtt_start);
	ASSERT(dev->gtt_total != 0);

	spin_unlock(&dev->struct_mutex);


	return 0;
}

/*ARGSUSED*/
int
i915_gem_get_aperture_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;	
	struct drm_i915_gem_get_aperture args;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	args.aper_size = (uint64_t)dev->gtt_total;
	args.aper_available_size = (args.aper_size -
				     atomic_read(&dev->pin_memory));

        ret = DRM_COPY_TO_USER((struct drm_i915_gem_get_aperture __user *) data, &args, sizeof(args));

        if ( ret != 0)
                DRM_ERROR(" i915_gem_get_aperture_ioctl error! %d", ret);

	DRM_DEBUG("i915_gem_get_aaperture_ioctl called sizeof %d, aper_size 0x%x, aper_available_size 0x%x\n", sizeof(args), dev->gtt_total, args.aper_available_size);

	return 0;
}

/**
 * Creates a new mm object and returns a handle to it.
 */
/*ARGSUSED*/
int
i915_gem_create_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_create args;
	struct drm_gem_object *obj;
	int handlep;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	DRM_COPYFROM_WITH_RETURN(&args,
	    (struct drm_i915_gem_create *) data, sizeof(args));


	args.size = (uint64_t) roundup(args.size, PAGE_SIZE);

	if (args.size == 0) {
		DRM_ERROR("Invalid obj size %d", args.size);
		return EINVAL;
	}
	/* Allocate the new object */
	obj = drm_gem_object_alloc(dev, args.size);
	if (obj == NULL) {
		DRM_ERROR("Failed to alloc obj");
		return ENOMEM;
	}

	ret = drm_gem_handle_create(fpriv, obj, &handlep);
	spin_lock(&dev->struct_mutex);
	drm_gem_object_handle_unreference(obj);
	spin_unlock(&dev->struct_mutex);
	if (ret)
		return ret;

	args.handle = handlep;

	ret = DRM_COPY_TO_USER((struct drm_i915_gem_create *) data, &args, sizeof(args));

	if ( ret != 0)
		DRM_ERROR(" gem create error! %d", ret);

	DRM_DEBUG("i915_gem_create_ioctl object name %d, size 0x%lx, list 0x%lx, obj 0x%lx",handlep, args.size, &fpriv->object_idr, obj);

	return 0;
}

/**
 * Reads data from the object referenced by handle.
 *
 * On error, the contents of *data are undefined.
 */
/*ARGSUSED*/
int
i915_gem_pread_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_pread args;
	struct drm_gem_object *obj;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	DRM_COPYFROM_WITH_RETURN(&args,
	    (struct drm_i915_gem_pread __user *) data, sizeof(args));

	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL)
		return EBADF;

	/* Bounds check source.
	 *
	 * XXX: This could use review for overflow issues...
	 */
	if (args.offset > obj->size || args.size > obj->size ||
	    args.offset + args.size > obj->size) {
		drm_gem_object_unreference(obj);
		DRM_ERROR("i915_gem_pread_ioctl invalid args");
		return EINVAL;
	}

	spin_lock(&dev->struct_mutex);

	ret = i915_gem_object_set_cpu_read_domain_range(obj, args.offset, args.size);
	if (ret != 0) {
		drm_gem_object_unreference(obj);
		spin_unlock(&dev->struct_mutex);
		DRM_ERROR("pread failed to read domain range ret %d!!!", ret);
		return EFAULT;
	}

	unsigned long unwritten = 0;
	uint32_t *user_data;
	user_data = (uint32_t *) (uintptr_t) args.data_ptr;

	unwritten = DRM_COPY_TO_USER(user_data, obj->kaddr + args.offset, args.size);
        if (unwritten) {
                ret = EFAULT;
                DRM_ERROR("i915_gem_pread error!!! unwritten %d", unwritten);
        }

	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);

	return ret;
}

/*ARGSUSED*/
static int
i915_gem_gtt_pwrite(struct drm_device *dev, struct drm_gem_object *obj,
		    struct drm_i915_gem_pwrite *args,
		    struct drm_file *file_priv)
{
	uint32_t *user_data;	
	int ret = 0;
	unsigned long unwritten = 0;

	user_data = (uint32_t *) (uintptr_t) args->data_ptr;
	spin_lock(&dev->struct_mutex);
	ret = i915_gem_object_pin(obj, 0);
	if (ret) {
		spin_unlock(&dev->struct_mutex);
		DRM_ERROR("i915_gem_gtt_pwrite failed to pin ret %d", ret);
		return ret;
	}

	ret = i915_gem_object_set_to_gtt_domain(obj, 1);
	if (ret)
		goto err;
	
	DRM_DEBUG("obj %d write domain 0x%x read domain 0x%x", obj->name, obj->write_domain, obj->read_domains);
	
	unwritten = DRM_COPY_FROM_USER(obj->kaddr + args->offset, user_data, args->size);
        if (unwritten) {
                ret = EFAULT;
                DRM_ERROR("i915_gem_gtt_pwrite error!!! unwritten %d", unwritten);
                goto err;
        }

err:
	i915_gem_object_unpin(obj);
	spin_unlock(&dev->struct_mutex);
	if (ret)
		DRM_ERROR("i915_gem_gtt_pwrite error %d", ret);
	return ret;
}

/*ARGSUSED*/
int
i915_gem_shmem_pwrite(struct drm_device *dev, struct drm_gem_object *obj,
		      struct drm_i915_gem_pwrite *args,
		      struct drm_file *file_priv)
{
	DRM_ERROR(" i915_gem_shmem_pwrite Not support");
	return -1;
}

/**
 * Writes data to the object referenced by handle.
 *
 * On error, the contents of the buffer that were to be modified are undefined.
 */
/*ARGSUSED*/
int
i915_gem_pwrite_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_pwrite args;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret = 0;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	ret = DRM_COPY_FROM_USER(&args,
            (struct drm_i915_gem_pwrite __user *) data, sizeof(args));
	if (ret)
		DRM_ERROR("i915_gem_pwrite_ioctl failed to copy from user");
	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL)
		return EBADF;
	obj_priv = obj->driver_private;
	DRM_DEBUG("i915_gem_pwrite_ioctl, obj->name %d",obj->name);

	/* Bounds check destination.
	 *
	 * XXX: This could use review for overflow issues...
	 */
	if (args.offset > obj->size || args.size > obj->size ||
	    args.offset + args.size > obj->size) {
		drm_gem_object_unreference(obj);
		DRM_ERROR("i915_gem_pwrite_ioctl invalid arg");
		return EINVAL;
	}

	/* We can only do the GTT pwrite on untiled buffers, as otherwise
	 * it would end up going through the fenced access, and we'll get
	 * different detiling behavior between reading and writing.
	 * pread/pwrite currently are reading and writing from the CPU
	 * perspective, requiring manual detiling by the client.
	 */
	if (obj_priv->tiling_mode == I915_TILING_NONE &&
	    dev->gtt_total != 0)
		ret = i915_gem_gtt_pwrite(dev, obj, &args, fpriv);
	else
		ret = i915_gem_shmem_pwrite(dev, obj, &args, fpriv);

	if (ret)
		DRM_ERROR("pwrite failed %d\n", ret);

	drm_gem_object_unreference(obj);

	return ret;
}

/**
 * Called when user space prepares to use an object with the CPU, either
 * through the mmap ioctl's mapping or a GTT mapping.
 */
/*ARGSUSED*/
int
i915_gem_set_domain_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_set_domain args;
	struct drm_gem_object *obj;
	int ret = 0;

	if (dev->driver->use_gem != 1)
		return ENODEV;

        DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_set_domain __user *) data, sizeof(args));

	uint32_t read_domains = args.read_domains;
	uint32_t write_domain = args.write_domain;

	/* Only handle setting domains to types used by the CPU. */
	if (write_domain & ~(I915_GEM_DOMAIN_CPU | I915_GEM_DOMAIN_GTT))
		ret = EINVAL;

	if (read_domains & ~(I915_GEM_DOMAIN_CPU | I915_GEM_DOMAIN_GTT))
		ret = EINVAL;

	/* Having something in the write domain implies it's in the read
	 * domain, and only that read domain.  Enforce that in the request.
	 */
	if (write_domain != 0 && read_domains != write_domain)
		ret = EINVAL;
	if (ret) {
		DRM_ERROR("set_domain invalid read or write");
		return EINVAL;
	}

	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL)
		return EBADF;

	spin_lock(&dev->struct_mutex);
	DRM_DEBUG("set_domain_ioctl %p(name %d size 0x%x), %08x %08x\n",
		 obj, obj->name, obj->size, args.read_domains, args.write_domain);

	if (read_domains & I915_GEM_DOMAIN_GTT) {
		ret = i915_gem_object_set_to_gtt_domain(obj, write_domain != 0);

		/* Silently promote "you're not bound, there was nothing to do"
		 * to success, since the client was just asking us to
		 * make sure everything was done.
		 */
		if (ret == EINVAL)
			ret = 0;
	} else {
		ret = i915_gem_object_set_to_cpu_domain(obj, write_domain != 0);
	}

	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);
	if (ret)
		DRM_ERROR("i915_set_domain_ioctl ret %d", ret);	
	return ret;
}

/**
 * Called when user space has done writes to this buffer
 */
/*ARGSUSED*/
int
i915_gem_sw_finish_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_sw_finish args;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret = 0;

	if (dev->driver->use_gem != 1)
		return ENODEV;

        DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_sw_finish __user *) data, sizeof(args));

	spin_lock(&dev->struct_mutex);
	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL) {
		spin_unlock(&dev->struct_mutex);
		return EBADF;
	}

	DRM_DEBUG("%s: sw_finish %d (%p name %d size 0x%x)\n",
		 __func__, args.handle, obj, obj->name, obj->size);

	obj_priv = obj->driver_private;
	/* Pinned buffers may be scanout, so flush the cache */
	if (obj_priv->pin_count)
	{
		i915_gem_object_flush_cpu_write_domain(obj);
	}

	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);
	return ret;
}

/**
 * Maps the contents of an object, returning the address it is mapped
 * into.
 *
 * While the mapping holds a reference on the contents of the object, it doesn't
 * imply a ref on the object itself.
 */
/*ARGSUSED*/
int
i915_gem_mmap_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_mmap args;
	struct drm_gem_object *obj;
	caddr_t vvaddr = NULL;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	DRM_COPYFROM_WITH_RETURN(
	    &args, (struct drm_i915_gem_mmap __user *)data,
	    sizeof (struct drm_i915_gem_mmap));

	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL)
		return EBADF;

	ret = ddi_devmap_segmap(fpriv->dev, (off_t)obj->map->handle,
	    ttoproc(curthread)->p_as, &vvaddr, obj->map->size,
	    PROT_ALL, PROT_ALL, MAP_SHARED, fpriv->credp);
	if (ret)
		return ret;

	spin_lock(&dev->struct_mutex);
	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);

	args.addr_ptr = (uint64_t)(uintptr_t)vvaddr;

	DRM_COPYTO_WITH_RETURN(
	    (struct drm_i915_gem_mmap __user *)data,
	    &args, sizeof (struct drm_i915_gem_mmap));

	return 0;
}

static void
i915_gem_object_free_page_list(struct drm_gem_object *obj)
{
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	if (obj_priv->page_list == NULL)
		return;

        kmem_free(obj_priv->page_list,
                 btop(obj->size) * sizeof(caddr_t));

        obj_priv->page_list = NULL;
}

static void
i915_gem_object_move_to_active(struct drm_gem_object *obj, uint32_t seqno)
{
	struct drm_device *dev = obj->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;

	/* Add a reference if we're newly entering the active list. */
	if (!obj_priv->active) {
		drm_gem_object_reference(obj);
		obj_priv->active = 1;
	}
	/* Move from whatever list we were on to the tail of execution. */
	list_move_tail(&obj_priv->list,
		       &dev_priv->mm.active_list, (caddr_t)obj_priv);
	obj_priv->last_rendering_seqno = seqno;
}

static void
i915_gem_object_move_to_flushing(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;

	list_move_tail(&obj_priv->list, &dev_priv->mm.flushing_list, (caddr_t)obj_priv);
	obj_priv->last_rendering_seqno = 0;
}

static void
i915_gem_object_move_to_inactive(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;

	if (obj_priv->pin_count != 0)
	{	
		list_del_init(&obj_priv->list);
	} else {
		list_move_tail(&obj_priv->list, &dev_priv->mm.inactive_list, (caddr_t)obj_priv);
	}
	obj_priv->last_rendering_seqno = 0;	
	if (obj_priv->active) {
		obj_priv->active = 0;
		drm_gem_object_unreference(obj);
	}
}

/**
 * Creates a new sequence number, emitting a write of it to the status page
 * plus an interrupt, which will trigger i915_user_interrupt_handler.
 *
 * Must be called with struct_lock held.
 *
 * Returned sequence numbers are nonzero on success.
 */
static uint32_t
i915_add_request(struct drm_device *dev, uint32_t flush_domains)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_request *request;
	uint32_t seqno;
	int was_empty;
	RING_LOCALS;

	request = drm_calloc(1, sizeof(*request), DRM_MEM_DRIVER);
	if (request == NULL) {
		DRM_ERROR("Failed to alloc request");
		return 0;
	}
	/* Grab the seqno we're going to make this request be, and bump the
	 * next (skipping 0 so it can be the reserved no-seqno value).
	 */
	seqno = dev_priv->mm.next_gem_seqno;
	dev_priv->mm.next_gem_seqno++;
	if (dev_priv->mm.next_gem_seqno == 0)
		dev_priv->mm.next_gem_seqno++;

	DRM_DEBUG("add_request seqno = %d dev 0x%lx", seqno, dev);

	BEGIN_LP_RING(4);
	OUT_RING(MI_STORE_DWORD_INDEX);
	OUT_RING(I915_GEM_HWS_INDEX << MI_STORE_DWORD_INDEX_SHIFT);
	OUT_RING(seqno);
        OUT_RING(0);
        ADVANCE_LP_RING();

	BEGIN_LP_RING(2);
	OUT_RING(0);
	OUT_RING(MI_USER_INTERRUPT);
	ADVANCE_LP_RING();
	
	request->seqno = seqno;
	request->emitted_jiffies = jiffies;
	was_empty = list_empty(&dev_priv->mm.request_list);
	list_add_tail(&request->list, &dev_priv->mm.request_list, (caddr_t)request);

	/* Associate any objects on the flushing list matching the write
	 * domain we're flushing with our flush.
	 */
	if (flush_domains != 0) {
		struct drm_i915_gem_object *obj_priv, *next;

		obj_priv = list_entry(dev_priv->mm.flushing_list.next, struct drm_i915_gem_object, list),
		next = list_entry(obj_priv->list.next, struct drm_i915_gem_object, list);
		for(; &obj_priv->list != &dev_priv->mm.flushing_list;
			obj_priv = next,
			next = list_entry(next->list.next, struct drm_i915_gem_object, list)) {
			struct drm_gem_object *obj = obj_priv->obj;

			if ((obj->write_domain & flush_domains) ==
			    obj->write_domain) {
				obj->write_domain = 0;
				i915_gem_object_move_to_active(obj, seqno);
			}
		}

	}

	if (was_empty && !dev_priv->mm.suspended)
	{
		/* change to delay HZ and then run work (not insert to workqueue of Linux) */ 
		worktimer_id = timeout(i915_gem_retire_work_handler, (void *) dev, DRM_HZ);
		DRM_DEBUG("i915_gem: schedule_delayed_work");
	}
	return seqno;
}

/**
 * Command execution barrier
 *
 * Ensures that all commands in the ring are finished
 * before signalling the CPU
 */
uint32_t
i915_retire_commands(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	uint32_t cmd = MI_FLUSH | MI_NO_WRITE_FLUSH;
	uint32_t flush_domains = 0;
	RING_LOCALS;

	/* The sampler always gets flushed on i965 (sigh) */
	if (IS_I965G(dev))
		flush_domains |= I915_GEM_DOMAIN_SAMPLER;
	BEGIN_LP_RING(2);
	OUT_RING(cmd);
	OUT_RING(0); /* noop */
	ADVANCE_LP_RING();

	return flush_domains;
}

/**
 * Moves buffers associated only with the given active seqno from the active
 * to inactive list, potentially freeing them.
 */
static void
i915_gem_retire_request(struct drm_device *dev,
			struct drm_i915_gem_request *request)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	/* Move any buffers on the active list that are no longer referenced
	 * by the ringbuffer to the flushing/inactive lists as appropriate.
	 */
	while (!list_empty(&dev_priv->mm.active_list)) {
		struct drm_gem_object *obj;
		struct drm_i915_gem_object *obj_priv;

		obj_priv = list_entry(dev_priv->mm.active_list.next,
					    struct drm_i915_gem_object,
					    list);
		obj = obj_priv->obj;

		/* If the seqno being retired doesn't match the oldest in the
		 * list, then the oldest in the list must still be newer than
		 * this seqno.
		 */
		if (obj_priv->last_rendering_seqno != request->seqno)
			return;

		DRM_DEBUG("%s: retire %d moves to inactive list %p\n",
			 __func__, request->seqno, obj);

		if (obj->write_domain != 0) {
			i915_gem_object_move_to_flushing(obj);
		} else {
			i915_gem_object_move_to_inactive(obj);
		}
	}
}

/**
 * Returns true if seq1 is later than seq2.
 */
static int
i915_seqno_passed(uint32_t seq1, uint32_t seq2)
{
	return (int32_t)(seq1 - seq2) >= 0;
}

uint32_t
i915_get_gem_seqno(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;

	return READ_HWSP(dev_priv, I915_GEM_HWS_INDEX);
}

/**
 * This function clears the request list as sequence numbers are passed.
 */
void
i915_gem_retire_requests(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	uint32_t seqno;

	seqno = i915_get_gem_seqno(dev);

	while (!list_empty(&dev_priv->mm.request_list)) {
		struct drm_i915_gem_request *request;
		uint32_t retiring_seqno;
		request = (struct drm_i915_gem_request *)(uintptr_t)(dev_priv->mm.request_list.next->contain_ptr);
		retiring_seqno = request->seqno;

		if (i915_seqno_passed(seqno, retiring_seqno) ||
		    dev_priv->mm.wedged) {
			i915_gem_retire_request(dev, request);

			list_del(&request->list);
			drm_free(request, sizeof(*request), DRM_MEM_DRIVER);
		} else
			break;
	}
}

void
i915_gem_retire_work_handler(void *device)
{
	struct drm_device *dev = (struct drm_device *)device;	
	drm_i915_private_t *dev_priv = dev->dev_private;

	spin_lock(&dev->struct_mutex);

	/* Return if gem idle */
	if (worktimer_id == NULL) {
		spin_unlock(&dev->struct_mutex);
		return;
	}

	i915_gem_retire_requests(dev);
	if (!dev_priv->mm.suspended && !list_empty(&dev_priv->mm.request_list))
	{	
		DRM_DEBUG("i915_gem: schedule_delayed_work");
		worktimer_id = timeout(i915_gem_retire_work_handler, (void *) dev, DRM_HZ);
	}
	spin_unlock(&dev->struct_mutex);
}

/**
 * i965_reset - reset chip after a hang
 * @dev: drm device to reset
 * @flags: reset domains
 *
 * Reset the chip.  Useful if a hang is detected.
 *
 * Procedure is fairly simple:
 *   - reset the chip using the reset reg
 *   - re-init context state
 *   - re-init hardware status page
 *   - re-init ring buffer
 *   - re-init interrupt state
 *   - re-init display
 */
void i965_reset(struct drm_device *dev, u8 flags)
{
	ddi_acc_handle_t conf_hdl;
	drm_i915_private_t *dev_priv = dev->dev_private;
	int timeout = 0;
	uint8_t gdrst;

	if (flags & GDRST_FULL)
		i915_save_display(dev);

	if (pci_config_setup(dev->dip, &conf_hdl) != DDI_SUCCESS) {
		DRM_ERROR(("i915_reset: pci_config_setup fail"));
		return;
	}

	/*
	 * Set the reset bit, wait for reset, then clear it.  Hardware
	 * will clear the status bit (bit 1) when it's actually ready
	 * for action again.
	 */
	gdrst = pci_config_get8(conf_hdl, GDRST);
	pci_config_put8(conf_hdl, GDRST, gdrst | flags);
	drv_usecwait(50);	
	pci_config_put8(conf_hdl, GDRST, gdrst | 0xfe);

	/* ...we don't want to loop forever though, 500ms should be plenty */
	do {
		drv_usecwait(100);
		gdrst = pci_config_get8(conf_hdl, GDRST);
	} while ((gdrst & 2) && (timeout++ < 5));

	/* Ok now get things going again... */

	/*
	 * Everything depends on having the GTT running, so we need to start
	 * there.  Fortunately we don't need to do this unless we reset the
	 * chip at a PCI level.
	 *
	 * Next we need to restore the context, but we don't use those
	 * yet either...
	 *
	 * Ring buffer needs to be re-initialized in the KMS case, or if X
	 * was running at the time of the reset (i.e. we weren't VT
	 * switched away).
	 */
	 if (!dev_priv->mm.suspended) {
		drm_i915_ring_buffer_t *ring = &dev_priv->ring;
		struct drm_gem_object *obj = ring->ring_obj;
		struct drm_i915_gem_object *obj_priv = obj->driver_private;
		dev_priv->mm.suspended = 0;

		/* Stop the ring if it's running. */
		I915_WRITE(PRB0_CTL, 0);
		I915_WRITE(PRB0_TAIL, 0);
		I915_WRITE(PRB0_HEAD, 0);

		/* Initialize the ring. */
		I915_WRITE(PRB0_START, obj_priv->gtt_offset);
		I915_WRITE(PRB0_CTL,
			   ((obj->size - 4096) & RING_NR_PAGES) |
			   RING_NO_REPORT |
			   RING_VALID);
		i915_kernel_lost_context(dev);

		(void) drm_irq_install(dev);
	}

	/*
	 * Display needs restore too...
	 */
	if (flags & GDRST_FULL)
		i915_restore_display(dev);
}

/**
 * Waits for a sequence number to be signaled, and cleans up the
 * request and object lists appropriately for that event.
 */
int
i915_wait_request(struct drm_device *dev, uint32_t seqno)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 ier;
	int ret = 0;

	ASSERT(seqno != 0);

	if (!i915_seqno_passed(i915_get_gem_seqno(dev), seqno)) {
		if (IS_IGDNG(dev))
			ier = I915_READ(DEIER) | I915_READ(GTIER);
		else
			ier = I915_READ(IER);
		if (!ier) {
 			DRM_ERROR("something (likely vbetool) disabled "
 				  "interrupts, re-enabling\n");
			(void) i915_driver_irq_preinstall(dev);
			i915_driver_irq_postinstall(dev);
		}

		dev_priv->mm.waiting_gem_seqno = seqno;
		i915_user_irq_on(dev);
		DRM_WAIT(ret, &dev_priv->irq_queue,
		    (i915_seqno_passed(i915_get_gem_seqno(dev), seqno) ||
				dev_priv->mm.wedged));
		i915_user_irq_off(dev);
		dev_priv->mm.waiting_gem_seqno = 0;
	}
	if (dev_priv->mm.wedged) {
		ret = EIO;
	}

	/* GPU maybe hang, reset needed*/
	if (ret == -2 && (seqno > i915_get_gem_seqno(dev))) {
		if (IS_I965G(dev)) {
			DRM_ERROR("GPU hang detected try to reset ... wait for irq_queue seqno %d, now seqno %d", seqno, i915_get_gem_seqno(dev));
			dev_priv->mm.wedged = 1;
			i965_reset(dev, GDRST_RENDER);
			i915_gem_retire_requests(dev);
			dev_priv->mm.wedged = 0;	
		}
		else
			DRM_ERROR("GPU hang detected.... reboot required");
		return 0;
	}
	/* Directly dispatch request retiring.  While we have the work queue
	 * to handle this, the waiter on a request often wants an associated
	 * buffer to have made it to the inactive list, and we would need
	 * a separate wait queue to handle that.
	 */
	if (ret == 0)
		i915_gem_retire_requests(dev);

	return ret;
}

static void
i915_gem_flush(struct drm_device *dev,
	       uint32_t invalidate_domains,
	       uint32_t flush_domains)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	uint32_t cmd;
	RING_LOCALS;

	DRM_DEBUG("%s: invalidate %08x flush %08x\n", __func__,
		  invalidate_domains, flush_domains);

	if (flush_domains & I915_GEM_DOMAIN_CPU)
		drm_agp_chipset_flush(dev);

	if ((invalidate_domains | flush_domains) & ~(I915_GEM_DOMAIN_CPU |
						     I915_GEM_DOMAIN_GTT)) {
		/*
		 * read/write caches:
		 *
		 * I915_GEM_DOMAIN_RENDER is always invalidated, but is
		 * only flushed if MI_NO_WRITE_FLUSH is unset.  On 965, it is
		 * also flushed at 2d versus 3d pipeline switches.
		 *
		 * read-only caches:
		 *
		 * I915_GEM_DOMAIN_SAMPLER is flushed on pre-965 if
		 * MI_READ_FLUSH is set, and is always flushed on 965.
		 *
		 * I915_GEM_DOMAIN_COMMAND may not exist?
		 *
		 * I915_GEM_DOMAIN_INSTRUCTION, which exists on 965, is
		 * invalidated when MI_EXE_FLUSH is set.
		 *
		 * I915_GEM_DOMAIN_VERTEX, which exists on 965, is
		 * invalidated with every MI_FLUSH.
		 *
		 * TLBs:
		 *
		 * On 965, TLBs associated with I915_GEM_DOMAIN_COMMAND
		 * and I915_GEM_DOMAIN_CPU in are invalidated at PTE write and
		 * I915_GEM_DOMAIN_RENDER and I915_GEM_DOMAIN_SAMPLER
		 * are flushed at any MI_FLUSH.
		 */

		cmd = MI_FLUSH | MI_NO_WRITE_FLUSH;
		if ((invalidate_domains|flush_domains) &
		    I915_GEM_DOMAIN_RENDER)
			cmd &= ~MI_NO_WRITE_FLUSH;
		if (!IS_I965G(dev)) {
			/*
			 * On the 965, the sampler cache always gets flushed
			 * and this bit is reserved.
			 */
			if (invalidate_domains & I915_GEM_DOMAIN_SAMPLER)
				cmd |= MI_READ_FLUSH;
		}
		if (invalidate_domains & I915_GEM_DOMAIN_INSTRUCTION)
			cmd |= MI_EXE_FLUSH;

		DRM_DEBUG("%s: queue flush %08x to ring\n", __func__, cmd);

		BEGIN_LP_RING(2);
		OUT_RING(cmd);
		OUT_RING(0); /* noop */
		ADVANCE_LP_RING();
	}
}

/**
 * Ensures that all rendering to the object has completed and the object is
 * safe to unbind from the GTT or access from the CPU.
 */
static int
i915_gem_object_wait_rendering(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	int ret, seqno;

	/* This function only exists to support waiting for existing rendering,
	 * not for emitting required flushes.
	 */

	if((obj->write_domain & I915_GEM_GPU_DOMAINS) != 0) {
		DRM_ERROR("write domain should not be GPU DOMAIN %d", obj_priv->active);
		return 0;
	}

	/* If there is rendering queued on the buffer being evicted, wait for
	 * it.
	 */
	if (obj_priv->active) {
		DRM_DEBUG("%s: object %d %p wait for seqno %08x\n",
			  __func__, obj->name, obj, obj_priv->last_rendering_seqno);

		seqno = obj_priv->last_rendering_seqno;
		if (seqno == 0) {
			DRM_DEBUG("last rendering maybe finished");
			return 0;
		}
		ret = i915_wait_request(dev, seqno);
		if (ret != 0) {
			DRM_ERROR("%s: i915_wait_request request->seqno %d now %d\n", __func__, seqno, i915_get_gem_seqno(dev));
			return ret;
		}
	}

	return 0;
}

/**
 * Unbinds an object from the GTT aperture.
 */
int
i915_gem_object_unbind(struct drm_gem_object *obj, uint32_t type)
{
	struct drm_device *dev = obj->dev;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	int ret = 0;

	if (obj_priv->gtt_space == NULL)
		return 0;

	if (obj_priv->pin_count != 0) {
		DRM_ERROR("Attempting to unbind pinned buffer\n");
		return EINVAL;
	}

	/* Wait for any rendering to complete
	 */
	ret = i915_gem_object_wait_rendering(obj);
	if (ret) {
		DRM_ERROR("wait_rendering failed: %d\n", ret);
		return ret;
	}

	/* Move the object to the CPU domain to ensure that
	 * any possible CPU writes while it's not in the GTT
	 * are flushed when we go to remap it. This will
	 * also ensure that all pending GPU writes are finished
	 * before we unbind.
	 */
	ret = i915_gem_object_set_to_cpu_domain(obj, 1);
	if (ret) {
		DRM_ERROR("set_domain failed: %d\n", ret);
		return ret;
	}

	if (!obj_priv->agp_mem) {
		(void) drm_agp_unbind_pages(dev, obj->size / PAGE_SIZE,
		    obj_priv->gtt_offset, type);
		obj_priv->agp_mem = -1;
	}

	ASSERT(!obj_priv->active);

	i915_gem_object_free_page_list(obj);

	if (obj_priv->gtt_space) {
		atomic_dec(&dev->gtt_count);
		atomic_sub(obj->size, &dev->gtt_memory);
		drm_mm_put_block(obj_priv->gtt_space);
		obj_priv->gtt_space = NULL;
	}

	/* Remove ourselves from the LRU list if present. */
	if (!list_empty(&obj_priv->list))
		list_del_init(&obj_priv->list);

	return 0;
}

static int
i915_gem_evict_something(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret = 0;

	for (;;) {
		/* If there's an inactive buffer available now, grab it
		 * and be done.
		 */
		if (!list_empty(&dev_priv->mm.inactive_list)) {
			obj_priv = list_entry(dev_priv->mm.inactive_list.next,
						    struct drm_i915_gem_object,
						    list);
			obj = obj_priv->obj;
			ASSERT(!(obj_priv->pin_count != 0));
			DRM_DEBUG("%s: evicting %d\n", __func__, obj->name);
			ASSERT(!(obj_priv->active));
			/* Wait on the rendering and unbind the buffer. */
			ret = i915_gem_object_unbind(obj, 1);
			break;
		}
		/* If we didn't get anything, but the ring is still processing
		 * things, wait for one of those things to finish and hopefully
		 * leave us a buffer to evict.
		 */
		if (!list_empty(&dev_priv->mm.request_list)) {
			struct drm_i915_gem_request *request;

			request = list_entry(dev_priv->mm.request_list.next,
						   struct drm_i915_gem_request,
						   list);

			ret = i915_wait_request(dev, request->seqno);
			if (ret) {
				break;
			}
			/* if waiting caused an object to become inactive,
			 * then loop around and wait for it. Otherwise, we
			 * assume that waiting freed and unbound something,
			 * so there should now be some space in the GTT
			 */
			if (!list_empty(&dev_priv->mm.inactive_list))
				continue;
			break;
		}

		/* If we didn't have anything on the request list but there
		 * are buffers awaiting a flush, emit one and try again.
		 * When we wait on it, those buffers waiting for that flush
		 * will get moved to inactive.
		 */
		if (!list_empty(&dev_priv->mm.flushing_list)) {
			obj_priv = list_entry(dev_priv->mm.flushing_list.next,
						    struct drm_i915_gem_object,
						    list);
			obj = obj_priv->obj;

			i915_gem_flush(dev,
				       obj->write_domain,
				       obj->write_domain);
			(void) i915_add_request(dev, obj->write_domain);

			obj = NULL;
			continue;
		}

		DRM_ERROR("inactive empty %d request empty %d "
			  "flushing empty %d\n",
			  list_empty(&dev_priv->mm.inactive_list),
			  list_empty(&dev_priv->mm.request_list),
			  list_empty(&dev_priv->mm.flushing_list));
		/* If we didn't do any of the above, there's nothing to be done
		 * and we just can't fit it in.
		 */
		return ENOMEM;
	}
	return ret;
}

static int
i915_gem_evict_everything(struct drm_device *dev)
{
	int ret;

	for (;;) {
		ret = i915_gem_evict_something(dev);
		if (ret != 0)
			break;
	}
	if (ret == ENOMEM)
		return 0;
	else
		DRM_ERROR("evict_everything ret %d", ret);
	return ret;
}

/**
 * Finds free space in the GTT aperture and binds the object there.
 */
static int
i915_gem_object_bind_to_gtt(struct drm_gem_object *obj, uint32_t alignment)
{
	struct drm_device *dev = obj->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	struct drm_mm_node *free_space;
	int page_count, ret;

	if (dev_priv->mm.suspended)
		return EBUSY;
	if (alignment == 0)
		alignment = PAGE_SIZE;
	if (alignment & (PAGE_SIZE - 1)) {
		DRM_ERROR("Invalid object alignment requested %u\n", alignment);
		return EINVAL;
	}

	if (obj_priv->gtt_space) {
		DRM_ERROR("Already bind!!");
		return 0;
	}
search_free:
	free_space = drm_mm_search_free(&dev_priv->mm.gtt_space,
					(unsigned long) obj->size, alignment, 0);
	if (free_space != NULL) {
		obj_priv->gtt_space = drm_mm_get_block(free_space, (unsigned long) obj->size,
						       alignment);
		if (obj_priv->gtt_space != NULL) {
			obj_priv->gtt_space->private = obj;
			obj_priv->gtt_offset = obj_priv->gtt_space->start;
		}
	}
	if (obj_priv->gtt_space == NULL) {
		/* If the gtt is empty and we're still having trouble
		 * fitting our object in, we're out of memory.
		 */
		if (list_empty(&dev_priv->mm.inactive_list) &&
		    list_empty(&dev_priv->mm.flushing_list) &&
		    list_empty(&dev_priv->mm.active_list)) {
			DRM_ERROR("GTT full, but LRU list empty\n");
			return ENOMEM;
		}

		ret = i915_gem_evict_something(dev);
		if (ret != 0) {
			DRM_ERROR("Failed to evict a buffer %d\n", ret);
			return ret;
		}
		goto search_free;
	}

	ret = i915_gem_object_get_page_list(obj);
	if (ret) {
		drm_mm_put_block(obj_priv->gtt_space);
		obj_priv->gtt_space = NULL;
		DRM_ERROR("bind to gtt failed to get page list");
		return ret;
	}

	page_count = obj->size / PAGE_SIZE;
	/* Create an AGP memory structure pointing at our pages, and bind it
	 * into the GTT.
	 */
	DRM_DEBUG("Binding object %d of page_count %d at gtt_offset 0x%x obj->pfnarray = 0x%lx", 
		 obj->name, page_count, obj_priv->gtt_offset, obj->pfnarray);

	obj_priv->agp_mem = drm_agp_bind_pages(dev,
					       obj->pfnarray,
					       page_count,
					       obj_priv->gtt_offset);
	if (obj_priv->agp_mem) {
		i915_gem_object_free_page_list(obj);
		drm_mm_put_block(obj_priv->gtt_space);
		obj_priv->gtt_space = NULL;
		DRM_ERROR("Failed to bind pages obj %d, obj 0x%lx", obj->name, obj);
		return ENOMEM;
	}
	atomic_inc(&dev->gtt_count);
	atomic_add(obj->size, &dev->gtt_memory);

	/* Assert that the object is not currently in any GPU domain. As it
	 * wasn't in the GTT, there shouldn't be any way it could have been in
	 * a GPU cache
	 */
	ASSERT(!(obj->read_domains & ~(I915_GEM_DOMAIN_CPU|I915_GEM_DOMAIN_GTT)));
	ASSERT(!(obj->write_domain & ~(I915_GEM_DOMAIN_CPU|I915_GEM_DOMAIN_GTT)));

	return 0;
}

void
i915_gem_clflush_object(struct drm_gem_object *obj)
{
	struct drm_i915_gem_object	*obj_priv = obj->driver_private;

	/* If we don't have a page list set up, then we're not pinned
	 * to GPU, and we can ignore the cache flush because it'll happen
	 * again at bind time.
	 */

	if (obj_priv->page_list == NULL)
		return;
	drm_clflush_pages(obj_priv->page_list, obj->size / PAGE_SIZE);
}

/** Flushes any GPU write domain for the object if it's dirty. */
static void
i915_gem_object_flush_gpu_write_domain(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;
	uint32_t seqno;

	if ((obj->write_domain & I915_GEM_GPU_DOMAINS) == 0)
		return;

	/* Queue the GPU write cache flushing we need. */
	i915_gem_flush(dev, 0, obj->write_domain);
	seqno = i915_add_request(dev, obj->write_domain);
	DRM_DEBUG("flush_gpu_write_domain seqno = %d", seqno);
	obj->write_domain = 0;
	i915_gem_object_move_to_active(obj, seqno);
}

/** Flushes the GTT write domain for the object if it's dirty. */
static void
i915_gem_object_flush_gtt_write_domain(struct drm_gem_object *obj)
{
	if (obj->write_domain != I915_GEM_DOMAIN_GTT)
		return;

	/* No actual flushing is required for the GTT write domain.   Writes
	 * to it immediately go to main memory as far as we know, so there's
	 * no chipset flush.  It also doesn't land in render cache.
	 */
	obj->write_domain = 0;
}

/** Flushes the CPU write domain for the object if it's dirty. */
static void
i915_gem_object_flush_cpu_write_domain(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;

	if (obj->write_domain != I915_GEM_DOMAIN_CPU)
		return;

	i915_gem_clflush_object(obj);
	drm_agp_chipset_flush(dev);
	obj->write_domain = 0;
}

/**
 * Moves a single object to the GTT read, and possibly write domain.
 *
 * This function returns when the move is complete, including waiting on
 * flushes to occur.
 */
static int
i915_gem_object_set_to_gtt_domain(struct drm_gem_object *obj, int write)
{
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	int ret;

	/* Not valid to be called on unbound objects. */
	if (obj_priv->gtt_space == NULL)
		return EINVAL;

	i915_gem_object_flush_gpu_write_domain(obj);
	/* Wait on any GPU rendering and flushing to occur. */
	ret = i915_gem_object_wait_rendering(obj);
	if (ret != 0) {
		DRM_ERROR("set_to_gtt_domain wait_rendering ret %d", ret);
		return ret;
	}
	/* If we're writing through the GTT domain, then CPU and GPU caches
	 * will need to be invalidated at next use.
	 */
	if (write)
		obj->read_domains &= I915_GEM_DOMAIN_GTT;
	i915_gem_object_flush_cpu_write_domain(obj);

	DRM_DEBUG("i915_gem_object_set_to_gtt_domain obj->read_domains %x ", obj->read_domains);
	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	ASSERT(!((obj->write_domain & ~I915_GEM_DOMAIN_GTT) != 0));
	obj->read_domains |= I915_GEM_DOMAIN_GTT;
	if (write) {
		obj->write_domain = I915_GEM_DOMAIN_GTT;
		obj_priv->dirty = 1;
	}

	return 0;
}

/**
 * Moves a single object to the CPU read, and possibly write domain.
 *
 * This function returns when the move is complete, including waiting on
 * flushes to occur.
 */
static int
i915_gem_object_set_to_cpu_domain(struct drm_gem_object *obj, int write)
{
	struct drm_device *dev = obj->dev;
	int ret;


	i915_gem_object_flush_gpu_write_domain(obj);
	/* Wait on any GPU rendering and flushing to occur. */

	ret = i915_gem_object_wait_rendering(obj);
	if (ret != 0)
		return ret;

	i915_gem_object_flush_gtt_write_domain(obj);

	/* If we have a partially-valid cache of the object in the CPU,
	 * finish invalidating it and free the per-page flags.
	 */
	i915_gem_object_set_to_full_cpu_read_domain(obj);

	/* Flush the CPU cache if it's still invalid. */
	if ((obj->read_domains & I915_GEM_DOMAIN_CPU) == 0) {
		i915_gem_clflush_object(obj);
		drm_agp_chipset_flush(dev);
		obj->read_domains |= I915_GEM_DOMAIN_CPU;
	}

	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	ASSERT(!((obj->write_domain & ~I915_GEM_DOMAIN_CPU) != 0));

	/* If we're writing through the CPU, then the GPU read domains will
	 * need to be invalidated at next use.
	 */
	if (write) {
		obj->read_domains &= I915_GEM_DOMAIN_CPU;
		obj->write_domain = I915_GEM_DOMAIN_CPU;
	}

	return 0;
}

/*
 * Set the next domain for the specified object. This
 * may not actually perform the necessary flushing/invaliding though,
 * as that may want to be batched with other set_domain operations
 *
 * This is (we hope) the only really tricky part of gem. The goal
 * is fairly simple -- track which caches hold bits of the object
 * and make sure they remain coherent. A few concrete examples may
 * help to explain how it works. For shorthand, we use the notation
 * (read_domains, write_domain), e.g. (CPU, CPU) to indicate the
 * a pair of read and write domain masks.
 *
 * Case 1: the batch buffer
 *
 *	1. Allocated
 *	2. Written by CPU
 *	3. Mapped to GTT
 *	4. Read by GPU
 *	5. Unmapped from GTT
 *	6. Freed
 *
 *	Let's take these a step at a time
 *
 *	1. Allocated
 *		Pages allocated from the kernel may still have
 *		cache contents, so we set them to (CPU, CPU) always.
 *	2. Written by CPU (using pwrite)
 *		The pwrite function calls set_domain (CPU, CPU) and
 *		this function does nothing (as nothing changes)
 *	3. Mapped by GTT
 *		This function asserts that the object is not
 *		currently in any GPU-based read or write domains
 *	4. Read by GPU
 *		i915_gem_execbuffer calls set_domain (COMMAND, 0).
 *		As write_domain is zero, this function adds in the
 *		current read domains (CPU+COMMAND, 0).
 *		flush_domains is set to CPU.
 *		invalidate_domains is set to COMMAND
 *		clflush is run to get data out of the CPU caches
 *		then i915_dev_set_domain calls i915_gem_flush to
 *		emit an MI_FLUSH and drm_agp_chipset_flush
 *	5. Unmapped from GTT
 *		i915_gem_object_unbind calls set_domain (CPU, CPU)
 *		flush_domains and invalidate_domains end up both zero
 *		so no flushing/invalidating happens
 *	6. Freed
 *		yay, done
 *
 * Case 2: The shared render buffer
 *
 *	1. Allocated
 *	2. Mapped to GTT
 *	3. Read/written by GPU
 *	4. set_domain to (CPU,CPU)
 *	5. Read/written by CPU
 *	6. Read/written by GPU
 *
 *	1. Allocated
 *		Same as last example, (CPU, CPU)
 *	2. Mapped to GTT
 *		Nothing changes (assertions find that it is not in the GPU)
 *	3. Read/written by GPU
 *		execbuffer calls set_domain (RENDER, RENDER)
 *		flush_domains gets CPU
 *		invalidate_domains gets GPU
 *		clflush (obj)
 *		MI_FLUSH and drm_agp_chipset_flush
 *	4. set_domain (CPU, CPU)
 *		flush_domains gets GPU
 *		invalidate_domains gets CPU
 *		wait_rendering (obj) to make sure all drawing is complete.
 *		This will include an MI_FLUSH to get the data from GPU
 *		to memory
 *		clflush (obj) to invalidate the CPU cache
 *		Another MI_FLUSH in i915_gem_flush (eliminate this somehow?)
 *	5. Read/written by CPU
 *		cache lines are loaded and dirtied
 *	6. Read written by GPU
 *		Same as last GPU access
 *
 * Case 3: The constant buffer
 *
 *	1. Allocated
 *	2. Written by CPU
 *	3. Read by GPU
 *	4. Updated (written) by CPU again
 *	5. Read by GPU
 *
 *	1. Allocated
 *		(CPU, CPU)
 *	2. Written by CPU
 *		(CPU, CPU)
 *	3. Read by GPU
 *		(CPU+RENDER, 0)
 *		flush_domains = CPU
 *		invalidate_domains = RENDER
 *		clflush (obj)
 *		MI_FLUSH
 *		drm_agp_chipset_flush
 *	4. Updated (written) by CPU again
 *		(CPU, CPU)
 *		flush_domains = 0 (no previous write domain)
 *		invalidate_domains = 0 (no new read domains)
 *	5. Read by GPU
 *		(CPU+RENDER, 0)
 *		flush_domains = CPU
 *		invalidate_domains = RENDER
 *		clflush (obj)
 *		MI_FLUSH
 *		drm_agp_chipset_flush
 */
static void 
i915_gem_object_set_to_gpu_domain(struct drm_gem_object *obj,
			    uint32_t read_domains,
			    uint32_t write_domain)
{
	struct drm_device		*dev = obj->dev;
	struct drm_i915_gem_object	*obj_priv = obj->driver_private;
	uint32_t			invalidate_domains = 0;
	uint32_t			flush_domains = 0;

	DRM_DEBUG("%s: object %p read %08x -> %08x write %08x -> %08x\n",
		 __func__, obj,
		 obj->read_domains, read_domains,
		 obj->write_domain, write_domain);
	/*
	 * If the object isn't moving to a new write domain,
	 * let the object stay in multiple read domains
	 */
	if (write_domain == 0)
		read_domains |= obj->read_domains;
	else
		obj_priv->dirty = 1;

	/*
	 * Flush the current write domain if
	 * the new read domains don't match. Invalidate
	 * any read domains which differ from the old
	 * write domain
	 */
	if (obj->write_domain && obj->write_domain != read_domains) {
		flush_domains |= obj->write_domain;
		invalidate_domains |= read_domains & ~obj->write_domain;
	}
	/*
	 * Invalidate any read caches which may have
	 * stale data. That is, any new read domains.
	 */
	invalidate_domains |= read_domains & ~obj->read_domains;
	if ((flush_domains | invalidate_domains) & I915_GEM_DOMAIN_CPU) {
		DRM_DEBUG("%s: CPU domain flush %08x invalidate %08x\n",
			 __func__, flush_domains, invalidate_domains);
	i915_gem_clflush_object(obj);
	}

	if ((write_domain | flush_domains) != 0)
		obj->write_domain = write_domain;
	obj->read_domains = read_domains;

	dev->invalidate_domains |= invalidate_domains;
	dev->flush_domains |= flush_domains;

	DRM_DEBUG("%s: read %08x write %08x invalidate %08x flush %08x\n",
		 __func__,
		 obj->read_domains, obj->write_domain,
		 dev->invalidate_domains, dev->flush_domains);

}

/**
 * Moves the object from a partially CPU read to a full one.
 *
 * Note that this only resolves i915_gem_object_set_cpu_read_domain_range(),
 * and doesn't handle transitioning from !(read_domains & I915_GEM_DOMAIN_CPU).
 */
static void
i915_gem_object_set_to_full_cpu_read_domain(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;

	if (!obj_priv->page_cpu_valid)
		return;

	/* If we're partially in the CPU read domain, finish moving it in.
	 */
	if (obj->read_domains & I915_GEM_DOMAIN_CPU) {
		int i;

		for (i = 0; i <= (obj->size - 1) / PAGE_SIZE; i++) {
			if (obj_priv->page_cpu_valid[i])
				continue;
			drm_clflush_pages(obj_priv->page_list + i, 1);
		}
		drm_agp_chipset_flush(dev);
	}

	/* Free the page_cpu_valid mappings which are now stale, whether
	 * or not we've got I915_GEM_DOMAIN_CPU.
	 */
	drm_free(obj_priv->page_cpu_valid, obj->size / PAGE_SIZE,
		 DRM_MEM_DRIVER);
	obj_priv->page_cpu_valid = NULL;
}

/**
 * Set the CPU read domain on a range of the object.
 *
 * The object ends up with I915_GEM_DOMAIN_CPU in its read flags although it's
 * not entirely valid.  The page_cpu_valid member of the object flags which
 * pages have been flushed, and will be respected by
 * i915_gem_object_set_to_cpu_domain() if it's called on to get a valid mapping
 * of the whole object.
 *
 * This function returns when the move is complete, including waiting on
 * flushes to occur.
 */
static int
i915_gem_object_set_cpu_read_domain_range(struct drm_gem_object *obj,
					  uint64_t offset, uint64_t size)
{
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	int i, ret;

	if (offset == 0 && size == obj->size)
		return i915_gem_object_set_to_cpu_domain(obj, 0);

	i915_gem_object_flush_gpu_write_domain(obj);
	/* Wait on any GPU rendering and flushing to occur. */
	ret = i915_gem_object_wait_rendering(obj);
	if (ret != 0)
		return ret;
	i915_gem_object_flush_gtt_write_domain(obj);

	/* If we're already fully in the CPU read domain, we're done. */
	if (obj_priv->page_cpu_valid == NULL &&
	    (obj->read_domains & I915_GEM_DOMAIN_CPU) != 0)
		return 0;

	/* Otherwise, create/clear the per-page CPU read domain flag if we're
	 * newly adding I915_GEM_DOMAIN_CPU
	 */
	if (obj_priv->page_cpu_valid == NULL) {
		obj_priv->page_cpu_valid = drm_calloc(1, obj->size / PAGE_SIZE,
						      DRM_MEM_DRIVER);
		if (obj_priv->page_cpu_valid == NULL)
			return ENOMEM;
	} else if ((obj->read_domains & I915_GEM_DOMAIN_CPU) == 0)
		(void) memset(obj_priv->page_cpu_valid, 0, obj->size / PAGE_SIZE);

	/* Flush the cache on any pages that are still invalid from the CPU's
	 * perspective.
	 */
	for (i = offset / PAGE_SIZE; i <= (offset + size - 1) / PAGE_SIZE;
	     i++) {
		if (obj_priv->page_cpu_valid[i])
			continue;

		drm_clflush_pages(obj_priv->page_list + i, 1);
		obj_priv->page_cpu_valid[i] = 1;
	}

	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	ASSERT(!((obj->write_domain & ~I915_GEM_DOMAIN_CPU) != 0));

	obj->read_domains |= I915_GEM_DOMAIN_CPU;

	return 0;
}

/**
 * Pin an object to the GTT and evaluate the relocations landing in it.
 */
static int
i915_gem_object_pin_and_relocate(struct drm_gem_object *obj,
				 struct drm_file *file_priv,
				 struct drm_i915_gem_exec_object *entry)
{
	struct drm_i915_gem_relocation_entry reloc;
	struct drm_i915_gem_relocation_entry __user *relocs;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	int i, ret;

	/* Choose the GTT offset for our buffer and put it there. */
	ret = i915_gem_object_pin(obj, (uint32_t) entry->alignment);
	if (ret) {
		DRM_ERROR("failed to pin");
		return ret;
	}
	entry->offset = obj_priv->gtt_offset;

	relocs = (struct drm_i915_gem_relocation_entry __user *)
		 (uintptr_t) entry->relocs_ptr;
	/* Apply the relocations, using the GTT aperture to avoid cache
	 * flushing requirements.
	 */
	for (i = 0; i < entry->relocation_count; i++) {
		struct drm_gem_object *target_obj;
		struct drm_i915_gem_object *target_obj_priv;
		uint32_t reloc_val, reloc_offset, *reloc_entry;

		ret = DRM_COPY_FROM_USER(&reloc, relocs + i, sizeof(reloc));
		if (ret != 0) {
			i915_gem_object_unpin(obj);
			DRM_ERROR("failed to copy from user");	
			return ret;
		}

		target_obj = drm_gem_object_lookup(file_priv,
						   reloc.target_handle);
		if (target_obj == NULL) {
			i915_gem_object_unpin(obj);
			return EBADF;
		}
		target_obj_priv = target_obj->driver_private;

		/* The target buffer should have appeared before us in the
		 * exec_object list, so it should have a GTT space bound by now.
		 */
		if (target_obj_priv->gtt_space == NULL) {
			DRM_ERROR("No GTT space found for object %d\n",
				  reloc.target_handle);
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			return EINVAL;
		}

		if (reloc.offset > obj->size - 4) {
			DRM_ERROR("Relocation beyond object bounds: "
				  "obj %p target %d offset %d size %d.\n",
				  obj, reloc.target_handle,
				  (int) reloc.offset, (int) obj->size);
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			return EINVAL;
		}
		if (reloc.offset & 3) {
			DRM_ERROR("Relocation not 4-byte aligned: "
				  "obj %p target %d offset %d.\n",
				  obj, reloc.target_handle,
				  (int) reloc.offset);
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			return EINVAL;
		}

		if (reloc.write_domain & I915_GEM_DOMAIN_CPU ||
		    reloc.read_domains & I915_GEM_DOMAIN_CPU) {
			DRM_ERROR("reloc with read/write CPU domains: "
				  "obj %p target %d offset %d "
				  "read %08x write %08x",
				  obj, reloc.target_handle,
				  (int) reloc.offset,
				  reloc.read_domains,
				  reloc.write_domain);
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			return EINVAL;
		}

		if (reloc.write_domain && target_obj->pending_write_domain &&
		    reloc.write_domain != target_obj->pending_write_domain) {
			DRM_ERROR("Write domain conflict: "
				  "obj %p target %d offset %d "
				  "new %08x old %08x\n",
				  obj, reloc.target_handle,
				  (int) reloc.offset,
				  reloc.write_domain,
				  target_obj->pending_write_domain);
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			return EINVAL;
		}
		DRM_DEBUG("%s: obj %p offset %08x target %d "
			 "read %08x write %08x gtt %08x "
			 "presumed %08x delta %08x\n",
			 __func__,
			 obj,
			 (int) reloc.offset,
			 (int) reloc.target_handle,
			 (int) reloc.read_domains,
			 (int) reloc.write_domain,
			 (int) target_obj_priv->gtt_offset,
			 (int) reloc.presumed_offset,
			 reloc.delta);

		target_obj->pending_read_domains |= reloc.read_domains;
		target_obj->pending_write_domain |= reloc.write_domain;

		/* If the relocation already has the right value in it, no
		 * more work needs to be done.
		 */
		if (target_obj_priv->gtt_offset == reloc.presumed_offset) {
			drm_gem_object_unreference(target_obj);
			continue;
		}

		ret = i915_gem_object_set_to_gtt_domain(obj, 1);
		if (ret != 0) {
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			return EINVAL;
		}

               /* Map the page containing the relocation we're going to
                * perform.
                */

		int reloc_base = (reloc.offset & ~(PAGE_SIZE-1));
		reloc_offset = reloc.offset & (PAGE_SIZE-1);
		reloc_entry = (uint32_t *)(uintptr_t)(obj_priv->page_list[reloc_base/PAGE_SIZE] + reloc_offset);
		reloc_val = target_obj_priv->gtt_offset + reloc.delta;
		*reloc_entry = reloc_val;

		/* Write the updated presumed offset for this entry back out
		 * to the user.
		 */
		reloc.presumed_offset = target_obj_priv->gtt_offset;
		ret = DRM_COPY_TO_USER(relocs + i, &reloc, sizeof(reloc));
		if (ret != 0) {
			drm_gem_object_unreference(target_obj);
			i915_gem_object_unpin(obj);
			DRM_ERROR("%s: Failed to copy to user ret %d", __func__, ret);
			return ret;
		}

		drm_gem_object_unreference(target_obj);
	}

	return 0;
}

/** Dispatch a batchbuffer to the ring
 */
static int
i915_dispatch_gem_execbuffer(struct drm_device *dev,
			      struct drm_i915_gem_execbuffer *exec,
			      uint64_t exec_offset)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_clip_rect __user *boxes = (struct drm_clip_rect __user *)
					     (uintptr_t) exec->cliprects_ptr;
	int nbox = exec->num_cliprects;
	int i = 0, count;
	uint64_t	exec_start, exec_len;
	RING_LOCALS;

	exec_start = exec_offset + exec->batch_start_offset;
	exec_len = exec->batch_len;

	if ((exec_start | exec_len) & 0x7) {
		DRM_ERROR("alignment\n");
		return EINVAL;
	}

	if (!exec_start) {
		DRM_ERROR("wrong arg");
		return EINVAL;
	}

	count = nbox ? nbox : 1;

	for (i = 0; i < count; i++) {
		if (i < nbox) {
			int ret = i915_emit_box(dev, boxes, i,
						exec->DR1, exec->DR4);
			if (ret) {
				DRM_ERROR("i915_emit_box %d DR1 0x%lx DRI2 0x%lx", ret, exec->DR1, exec->DR4);
				return ret;
			}
		}
		if (IS_I830(dev) || IS_845G(dev)) {
			BEGIN_LP_RING(4);
			OUT_RING(MI_BATCH_BUFFER);
			OUT_RING(exec_start | MI_BATCH_NON_SECURE);
			OUT_RING(exec_start + exec_len - 4);
			OUT_RING(0);
			ADVANCE_LP_RING();
		} else {
			BEGIN_LP_RING(2);
			if (IS_I965G(dev)) {
				OUT_RING(MI_BATCH_BUFFER_START |
					 (2 << 6) |
					 (3 << 9) |
					 MI_BATCH_NON_SECURE_I965);
				OUT_RING(exec_start);

			} else {
				OUT_RING(MI_BATCH_BUFFER_START |
					 (2 << 6));
				OUT_RING(exec_start | MI_BATCH_NON_SECURE);
			}
			ADVANCE_LP_RING();
		}
	}
	/* XXX breadcrumb */
	return 0;
}

/* Throttle our rendering by waiting until the ring has completed our requests
 * emitted over 20 msec ago.
 *
 * This should get us reasonable parallelism between CPU and GPU but also
 * relatively low latency when blocking on a particular request to finish.
 */
static int
i915_gem_ring_throttle(struct drm_device *dev, struct drm_file *file_priv)
{
	struct drm_i915_file_private *i915_file_priv = file_priv->driver_priv;
	int ret = 0;
	uint32_t seqno;

	spin_lock(&dev->struct_mutex);
	seqno = i915_file_priv->mm.last_gem_throttle_seqno;
	i915_file_priv->mm.last_gem_throttle_seqno =
		i915_file_priv->mm.last_gem_seqno;
	if (seqno) {
		ret = i915_wait_request(dev, seqno);
		if (ret != 0)
			DRM_ERROR("%s: i915_wait_request request->seqno %d now %d\n", __func__, seqno, i915_get_gem_seqno(dev));
	}
	spin_unlock(&dev->struct_mutex);
	return ret;
}

/*ARGSUSED*/
int
i915_gem_execbuffer(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_file_private *i915_file_priv = fpriv->driver_priv;
	struct drm_i915_gem_execbuffer args;
	struct drm_i915_gem_exec_object *exec_list = NULL;
	struct drm_gem_object **object_list = NULL;
	struct drm_gem_object *batch_obj;
	struct drm_i915_gem_object *obj_priv;
	int ret = 0, i, pinned = 0;
	uint64_t exec_offset;
	uint32_t seqno, flush_domains;
	int pin_tries;

	if (dev->driver->use_gem != 1)
		return ENODEV;

        DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_execbuffer __user *) data, sizeof(args));

	DRM_DEBUG("buffer_count %d len %x\n", args.buffer_count, args.batch_len);

	if (args.buffer_count < 1) {
		DRM_ERROR("execbuf with %d buffers\n", args.buffer_count);
		return EINVAL;
	}
	/* Copy in the exec list from userland */
	exec_list = drm_calloc(sizeof(*exec_list), args.buffer_count,
			       DRM_MEM_DRIVER);
	object_list = drm_calloc(sizeof(*object_list), args.buffer_count,
				 DRM_MEM_DRIVER);
	if (exec_list == NULL || object_list == NULL) {
		DRM_ERROR("Failed to allocate exec or object list "
			  "for %d buffers\n",
			  args.buffer_count);
		ret = ENOMEM;
		goto pre_mutex_err;
	}

	ret = DRM_COPY_FROM_USER(exec_list,
			     (struct drm_i915_gem_exec_object __user *)
			     (uintptr_t) args.buffers_ptr,
			     sizeof(*exec_list) * args.buffer_count);
	if (ret != 0) {
		DRM_ERROR("copy %d exec entries failed %d\n",
			  args.buffer_count, ret);
		goto pre_mutex_err;
	}
	spin_lock(&dev->struct_mutex);

	if (dev_priv->mm.wedged) {
		DRM_ERROR("Execbuf while wedged\n");
		spin_unlock(&dev->struct_mutex);
		return EIO;
	}

	if (dev_priv->mm.suspended) {
		DRM_ERROR("Execbuf while VT-switched.\n");
		spin_unlock(&dev->struct_mutex);
		return EBUSY;
	}

	/* Look up object handles */
	for (i = 0; i < args.buffer_count; i++) {
		object_list[i] = drm_gem_object_lookup(fpriv,
						       exec_list[i].handle);
		if (object_list[i] == NULL) {
			DRM_ERROR("Invalid object handle %d at index %d\n",
				   exec_list[i].handle, i);
			ret = EBADF;
			goto err;
		}
		obj_priv = object_list[i]->driver_private;
		if (obj_priv->in_execbuffer) {
			DRM_ERROR("Object[%d] (%d) %p appears more than once in object list in args.buffer_count %d \n",
				   i, object_list[i]->name, object_list[i], args.buffer_count);

			ret = EBADF;
			goto err;
		}

		obj_priv->in_execbuffer = 1;
	}

	/* Pin and relocate */
	for (pin_tries = 0; ; pin_tries++) {
		ret = 0;
		for (i = 0; i < args.buffer_count; i++) {
			object_list[i]->pending_read_domains = 0;
			object_list[i]->pending_write_domain = 0;
			ret = i915_gem_object_pin_and_relocate(object_list[i],
							       fpriv,
							       &exec_list[i]);
			if (ret) {
				DRM_ERROR("Not all object pinned");
				break;
			}
			pinned = i + 1;
		}
		/* success */
		if (ret == 0)
		{
			DRM_DEBUG("gem_execbuffer pin_relocate success");	
			break;
		}
		/* error other than GTT full, or we've already tried again */
		if (ret != ENOMEM || pin_tries >= 1) {
			if (ret != ERESTART)
				DRM_ERROR("Failed to pin buffers %d\n", ret);
			goto err;
		}

		/* unpin all of our buffers */
		for (i = 0; i < pinned; i++)
			i915_gem_object_unpin(object_list[i]);
		pinned = 0;

		/* evict everyone we can from the aperture */
		ret = i915_gem_evict_everything(dev);
		if (ret)
			goto err;
	}

	/* Set the pending read domains for the batch buffer to COMMAND */
	batch_obj = object_list[args.buffer_count-1];
	batch_obj->pending_read_domains = I915_GEM_DOMAIN_COMMAND;
	batch_obj->pending_write_domain = 0;

	/* Zero the gloabl flush/invalidate flags. These
	 * will be modified as each object is bound to the
	 * gtt
	 */
	dev->invalidate_domains = 0;
	dev->flush_domains = 0;

	for (i = 0; i < args.buffer_count; i++) {
		struct drm_gem_object *obj = object_list[i];

		/* Compute new gpu domains and update invalidate/flush */
		i915_gem_object_set_to_gpu_domain(obj,
						  obj->pending_read_domains,
						  obj->pending_write_domain);
	}

	if (dev->invalidate_domains | dev->flush_domains) {

		DRM_DEBUG("%s: invalidate_domains %08x flush_domains %08x Then flush\n",
			  __func__,
			 dev->invalidate_domains,
			 dev->flush_domains);
                i915_gem_flush(dev,
                               dev->invalidate_domains,
                               dev->flush_domains);
                if (dev->flush_domains) {
                        (void) i915_add_request(dev, dev->flush_domains);

		}
	}

	for (i = 0; i < args.buffer_count; i++) {
		struct drm_gem_object *obj = object_list[i];

		obj->write_domain = obj->pending_write_domain;
	}

	exec_offset = exec_list[args.buffer_count - 1].offset;

	/* Exec the batchbuffer */
	ret = i915_dispatch_gem_execbuffer(dev, &args, exec_offset);
	if (ret) {
		DRM_ERROR("dispatch failed %d\n", ret);
		goto err;
	}

	/*
	 * Ensure that the commands in the batch buffer are
	 * finished before the interrupt fires
	 */
	flush_domains = i915_retire_commands(dev);

	/*
	 * Get a seqno representing the execution of the current buffer,
	 * which we can wait on.  We would like to mitigate these interrupts,
	 * likely by only creating seqnos occasionally (so that we have
	 * *some* interrupts representing completion of buffers that we can
	 * wait on when trying to clear up gtt space).
	 */
	seqno = i915_add_request(dev, flush_domains);
	ASSERT(!(seqno == 0));
	i915_file_priv->mm.last_gem_seqno = seqno;
	for (i = 0; i < args.buffer_count; i++) {
		struct drm_gem_object *obj = object_list[i];
		i915_gem_object_move_to_active(obj, seqno);
		DRM_DEBUG("%s: move to exec list %p\n", __func__, obj);
	}

err:
	if (object_list != NULL) {
		for (i = 0; i < pinned; i++)
			i915_gem_object_unpin(object_list[i]);

		for (i = 0; i < args.buffer_count; i++) {
			if (object_list[i]) {
				obj_priv = object_list[i]->driver_private;
				obj_priv->in_execbuffer = 0;
			}
			drm_gem_object_unreference(object_list[i]);
		}
	}
	spin_unlock(&dev->struct_mutex);

	if (!ret) {
	        /* Copy the new buffer offsets back to the user's exec list. */
	        ret = DRM_COPY_TO_USER((struct drm_i915_relocation_entry __user *)
	                           (uintptr_t) args.buffers_ptr,
	                           exec_list,
	                           sizeof(*exec_list) * args.buffer_count);
	        if (ret)
	                DRM_ERROR("failed to copy %d exec entries "
	                          "back to user (%d)\n",
	                           args.buffer_count, ret);
	}

pre_mutex_err:
	drm_free(object_list, sizeof(*object_list) * args.buffer_count,
		 DRM_MEM_DRIVER);
	drm_free(exec_list, sizeof(*exec_list) * args.buffer_count,
		 DRM_MEM_DRIVER);

	return ret;
}

int
i915_gem_object_pin(struct drm_gem_object *obj, uint32_t alignment)
{
	struct drm_device *dev = obj->dev;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	int ret;

	if (obj_priv->gtt_space == NULL) {
		ret = i915_gem_object_bind_to_gtt(obj, alignment);
		if (ret != 0) {
			DRM_ERROR("Failure to bind: %d", ret);
			return ret;
		}
	}
	obj_priv->pin_count++;

	/* If the object is not active and not pending a flush,
	 * remove it from the inactive list
	 */
	if (obj_priv->pin_count == 1) {
		atomic_inc(&dev->pin_count);
		atomic_add(obj->size, &dev->pin_memory);
		if (!obj_priv->active &&
		    (obj->write_domain & ~(I915_GEM_DOMAIN_CPU |
					   I915_GEM_DOMAIN_GTT)) == 0 &&
		    !list_empty(&obj_priv->list))
			list_del_init(&obj_priv->list);
	}
	return 0;
}

void
i915_gem_object_unpin(struct drm_gem_object *obj)
{
	struct drm_device *dev = obj->dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
	obj_priv->pin_count--;
	ASSERT(!(obj_priv->pin_count < 0));
	ASSERT(!(obj_priv->gtt_space == NULL));

	/* If the object is no longer pinned, and is
	 * neither active nor being flushed, then stick it on
	 * the inactive list
	 */
	if (obj_priv->pin_count == 0) {
		if (!obj_priv->active &&
		    (obj->write_domain & ~(I915_GEM_DOMAIN_CPU |
					   I915_GEM_DOMAIN_GTT)) == 0)
			list_move_tail(&obj_priv->list,
				       &dev_priv->mm.inactive_list, (caddr_t)obj_priv);
		atomic_dec(&dev->pin_count);
		atomic_sub(obj->size, &dev->pin_memory);
	}
}

/*ARGSUSED*/
int
i915_gem_pin_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_pin args;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

        DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_pin __user *) data, sizeof(args));

	spin_lock(&dev->struct_mutex);

	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL) {
		DRM_ERROR("Bad handle in i915_gem_pin_ioctl(): %d\n",
			  args.handle);
		spin_unlock(&dev->struct_mutex);
		return EBADF;
	}
	DRM_DEBUG("i915_gem_pin_ioctl obj->name %d", obj->name);
	obj_priv = obj->driver_private;

	if (obj_priv->pin_filp != NULL && obj_priv->pin_filp != fpriv) {
		DRM_ERROR("Already pinned in i915_gem_pin_ioctl(): %d\n",
			  args.handle);
		drm_gem_object_unreference(obj);
		spin_unlock(&dev->struct_mutex);
		return EINVAL;
	}

	obj_priv->user_pin_count++;
	obj_priv->pin_filp = fpriv;
	if (obj_priv->user_pin_count == 1) {
		ret = i915_gem_object_pin(obj, args.alignment);
		if (ret != 0) {
			drm_gem_object_unreference(obj);
			spin_unlock(&dev->struct_mutex);
			return ret;
		}
	}

	/* XXX - flush the CPU caches for pinned objects
	 * as the X server doesn't manage domains yet
	 */
	i915_gem_object_flush_cpu_write_domain(obj);
	args.offset = obj_priv->gtt_offset;

	ret = DRM_COPY_TO_USER((struct drm_i915_gem_pin __user *) data, &args, sizeof(args));
	if ( ret != 0)
		DRM_ERROR(" gem pin ioctl error! %d", ret);

	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);

	return 0;
}

/*ARGSUSED*/
int
i915_gem_unpin_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_pin args;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;

	if (dev->driver->use_gem != 1)
		return ENODEV;

        DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_pin __user *) data, sizeof(args));

	spin_lock(&dev->struct_mutex);

	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL) {
		DRM_ERROR("Bad handle in i915_gem_unpin_ioctl(): %d\n",
			  args.handle);
		spin_unlock(&dev->struct_mutex);
		return EBADF;
	}
	obj_priv = obj->driver_private;	
	DRM_DEBUG("i915_gem_unpin_ioctl, obj->name %d", obj->name);
	if (obj_priv->pin_filp != fpriv) {
		DRM_ERROR("Not pinned by caller in i915_gem_pin_ioctl(): %d\n",
			  args.handle);
		drm_gem_object_unreference(obj);
		spin_unlock(&dev->struct_mutex);
		return EINVAL;
	}
	obj_priv->user_pin_count--;
	if (obj_priv->user_pin_count == 0) {
		obj_priv->pin_filp = NULL;
		i915_gem_object_unpin(obj);
	}
	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);
	return 0;
}

/*ARGSUSED*/
int
i915_gem_busy_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	struct drm_i915_gem_busy args;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

        DRM_COPYFROM_WITH_RETURN(&args,
            (struct drm_i915_gem_busy __user *) data, sizeof(args));

	spin_lock(&dev->struct_mutex);
	obj = drm_gem_object_lookup(fpriv, args.handle);
	if (obj == NULL) {
		DRM_ERROR("Bad handle in i915_gem_busy_ioctl(): %d\n",
			  args.handle);
		spin_unlock(&dev->struct_mutex);
		return EBADF;
	}

	obj_priv = obj->driver_private;
	/* Don't count being on the flushing list against the object being
	 * done.  Otherwise, a buffer left on the flushing list but not getting
	 * flushed (because nobody's flushing that domain) won't ever return
	 * unbusy and get reused by libdrm's bo cache.  The other expected
	 * consumer of this interface, OpenGL's occlusion queries, also specs
	 * that the objects get unbusy "eventually" without any interference.
	 */
	args.busy = obj_priv->active && obj_priv->last_rendering_seqno != 0;
	DRM_DEBUG("i915_gem_busy_ioctl call obj->name %d busy %d", obj->name, args.busy);

        ret = DRM_COPY_TO_USER((struct drm_i915_gem_busy __user *) data, &args, sizeof(args));
        if ( ret != 0)
                DRM_ERROR(" gem busy error! %d", ret);

	drm_gem_object_unreference(obj);
	spin_unlock(&dev->struct_mutex);
	return 0;
}

/*ARGSUSED*/
int
i915_gem_throttle_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	return i915_gem_ring_throttle(dev, fpriv);
}

static int
i915_gem_object_get_page_list(struct drm_gem_object *obj)
{
	struct drm_i915_gem_object *obj_priv = obj->driver_private;
        caddr_t va;
        long i;

	if (obj_priv->page_list)
		return 0;
        pgcnt_t np = btop(obj->size);

        obj_priv->page_list = kmem_zalloc(np * sizeof(caddr_t), KM_SLEEP);
        if (obj_priv->page_list == NULL) {
                DRM_ERROR("Faled to allocate page list\n");
                return ENOMEM;
        }

	for (i = 0, va = obj->kaddr; i < np; i++, va += PAGESIZE) {
		obj_priv->page_list[i] = va;
	}
	return 0;
}


int i915_gem_init_object(struct drm_gem_object *obj)
{
	struct drm_i915_gem_object *obj_priv;

	obj_priv = drm_calloc(1, sizeof(*obj_priv), DRM_MEM_DRIVER);
	if (obj_priv == NULL)
		return ENOMEM;

	/*
	 * We've just allocated pages from the kernel,
	 * so they've just been written by the CPU with
	 * zeros. They'll need to be clflushed before we
	 * use them with the GPU.
	 */
	obj->write_domain = I915_GEM_DOMAIN_CPU;
	obj->read_domains = I915_GEM_DOMAIN_CPU;

	obj->driver_private = obj_priv;
	obj_priv->obj = obj;
	INIT_LIST_HEAD(&obj_priv->list);
	return 0;
}

void i915_gem_free_object(struct drm_gem_object *obj)
{
	struct drm_i915_gem_object *obj_priv = obj->driver_private;

	while (obj_priv->pin_count > 0)
		i915_gem_object_unpin(obj);

	DRM_DEBUG("%s: obj %d",__func__, obj->name);

	(void) i915_gem_object_unbind(obj, 1);
	if (obj_priv->page_cpu_valid != NULL)
		drm_free(obj_priv->page_cpu_valid, obj->size / PAGE_SIZE, DRM_MEM_DRIVER);
	drm_free(obj->driver_private, sizeof(*obj_priv), DRM_MEM_DRIVER);
}

/** Unbinds all objects that are on the given buffer list. */
static int
i915_gem_evict_from_list(struct drm_device *dev, struct list_head *head, uint32_t type)
{
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret;

	while (!list_empty(head)) {
		obj_priv = list_entry(head->next,
				struct drm_i915_gem_object,
			    	list);
		obj = obj_priv->obj;

		if (obj_priv->pin_count != 0) {
			DRM_ERROR("Pinned object in unbind list\n");
			spin_unlock(&dev->struct_mutex);
			return EINVAL;
		}
		DRM_DEBUG("%s: obj %d type %d",__func__, obj->name, type);
		ret = i915_gem_object_unbind(obj, type);
		if (ret != 0) {
			DRM_ERROR("Error unbinding object in LeaveVT: %d\n",
				  ret);
			spin_unlock(&dev->struct_mutex);
			return ret;
		}
	}


	return 0;
}

static int
i915_gem_idle(struct drm_device *dev, uint32_t type)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	uint32_t seqno, cur_seqno, last_seqno;
	int stuck, ret;

	spin_lock(&dev->struct_mutex);

	if (dev_priv->mm.suspended || dev_priv->ring.ring_obj == NULL) {
		spin_unlock(&dev->struct_mutex);
		return 0;
	}

	/* Hack!  Don't let anybody do execbuf while we don't control the chip.
	 * We need to replace this with a semaphore, or something.
	 */
	dev_priv->mm.suspended = 1;

	/* Cancel the retire work handler, wait for it to finish if running
	 */
	if (worktimer_id != NULL) {
		(void) untimeout(worktimer_id);
		worktimer_id = NULL;
	}

	i915_kernel_lost_context(dev);

	/* Flush the GPU along with all non-CPU write domains
	 */
	i915_gem_flush(dev, ~(I915_GEM_DOMAIN_CPU|I915_GEM_DOMAIN_GTT),
		       ~(I915_GEM_DOMAIN_CPU|I915_GEM_DOMAIN_GTT));
	seqno = i915_add_request(dev, ~(I915_GEM_DOMAIN_CPU |
					I915_GEM_DOMAIN_GTT));
	if (seqno == 0) {
		spin_unlock(&dev->struct_mutex);
		return ENOMEM;
	}

	dev_priv->mm.waiting_gem_seqno = seqno;
	last_seqno = 0;
	stuck = 0;
	for (;;) {
		cur_seqno = i915_get_gem_seqno(dev);
		if (i915_seqno_passed(cur_seqno, seqno))
			break;
		if (last_seqno == cur_seqno) {
			if (stuck++ > 100) {
				DRM_ERROR("hardware wedged\n");
				dev_priv->mm.wedged = 1;
				DRM_WAKEUP(&dev_priv->irq_queue);
				break;
			}
		}
		DRM_UDELAY(10);
		last_seqno = cur_seqno;
	}
	dev_priv->mm.waiting_gem_seqno = 0;

	i915_gem_retire_requests(dev);

	/* Empty the active and flushing lists to inactive.  If there's
	 * anything left at this point, it means that we're wedged and
	 * nothing good's going to happen by leaving them there.  So strip
	 * the GPU domains and just stuff them onto inactive.
	 */
	while (!list_empty(&dev_priv->mm.active_list)) {
		struct drm_i915_gem_object *obj_priv;

		obj_priv = list_entry(dev_priv->mm.active_list.next,
					    struct drm_i915_gem_object,
					    list);
		obj_priv->obj->write_domain &= ~I915_GEM_GPU_DOMAINS;
		i915_gem_object_move_to_inactive(obj_priv->obj);
	}

	while (!list_empty(&dev_priv->mm.flushing_list)) {
		struct drm_i915_gem_object *obj_priv;

		obj_priv = list_entry(dev_priv->mm.flushing_list.next,
					    struct drm_i915_gem_object,
					    list);
		obj_priv->obj->write_domain &= ~I915_GEM_GPU_DOMAINS;
		i915_gem_object_move_to_inactive(obj_priv->obj);
	}
	
	/* Move all inactive buffers out of the GTT. */
	ret = i915_gem_evict_from_list(dev, &dev_priv->mm.inactive_list, type);
	ASSERT(list_empty(&dev_priv->mm.inactive_list));
	if (ret) {
		spin_unlock(&dev->struct_mutex);
		return ret;
	}

	i915_gem_cleanup_ringbuffer(dev);
	spin_unlock(&dev->struct_mutex);

	return 0;
}

static int
i915_gem_init_hws(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret;

	/* If we need a physical address for the status page, it's already
	 * initialized at driver load time.
	 */
	if (!I915_NEED_GFX_HWS(dev))
		return 0;


	obj = drm_gem_object_alloc(dev, 4096);
	if (obj == NULL) {
		DRM_ERROR("Failed to allocate status page\n");
		return ENOMEM;
	}

	obj_priv = obj->driver_private;

	ret = i915_gem_object_pin(obj, 4096);
	if (ret != 0) {
		drm_gem_object_unreference(obj);
		return ret;
	}

	dev_priv->status_gfx_addr = obj_priv->gtt_offset;
	dev_priv->hws_map.offset = dev->agp->agp_info.agpi_aperbase + obj_priv->gtt_offset;
	dev_priv->hws_map.size = 4096;
	dev_priv->hws_map.type = 0;
	dev_priv->hws_map.flags = 0;
	dev_priv->hws_map.mtrr = 0;

	drm_core_ioremap(&dev_priv->hws_map, dev);
	if (dev_priv->hws_map.handle == NULL) {
		DRM_ERROR("Failed to map status page.\n");
		(void) memset(&dev_priv->hws_map, 0, sizeof(dev_priv->hws_map));
		drm_gem_object_unreference(obj);
		return EINVAL;
	}

	dev_priv->hws_obj = obj;

	dev_priv->hw_status_page = dev_priv->hws_map.handle;

	(void) memset(dev_priv->hw_status_page, 0, PAGE_SIZE);
	I915_WRITE(HWS_PGA, dev_priv->status_gfx_addr);
	(void) I915_READ(HWS_PGA); /* posting read */
	DRM_DEBUG("hws offset: 0x%08x\n", dev_priv->status_gfx_addr);

	return 0;
}

static void
i915_gem_cleanup_hws(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_gem_object *obj;

	if (dev_priv->hws_obj == NULL)
		return;

	obj = dev_priv->hws_obj;

	drm_core_ioremapfree(&dev_priv->hws_map, dev);
	i915_gem_object_unpin(obj);
	drm_gem_object_unreference(obj);
	dev_priv->hws_obj = NULL;

	(void) memset(&dev_priv->hws_map, 0, sizeof(dev_priv->hws_map));
	dev_priv->hw_status_page = NULL;

	/* Write high address into HWS_PGA when disabling. */
	I915_WRITE(HWS_PGA, 0x1ffff000);
}

int
i915_gem_init_ringbuffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_gem_object *obj;
	struct drm_i915_gem_object *obj_priv;
	int ret;
	u32 head;

	ret = i915_gem_init_hws(dev);
	if (ret != 0)
		return ret;
	obj = drm_gem_object_alloc(dev, 128 * 1024);
	if (obj == NULL) {
		DRM_ERROR("Failed to allocate ringbuffer\n");
		i915_gem_cleanup_hws(dev);
		return ENOMEM;
	}

	obj_priv = obj->driver_private;
	ret = i915_gem_object_pin(obj, 4096);
	if (ret != 0) {
		drm_gem_object_unreference(obj);
		i915_gem_cleanup_hws(dev);
		return ret;
	}

	/* Set up the kernel mapping for the ring. */
	dev_priv->ring.Size = obj->size;
	dev_priv->ring.tail_mask = obj->size - 1;

	dev_priv->ring.map.offset = dev->agp->agp_info.agpi_aperbase + obj_priv->gtt_offset;
	dev_priv->ring.map.size = obj->size;
	dev_priv->ring.map.type = 0;
	dev_priv->ring.map.flags = 0;
	dev_priv->ring.map.mtrr = 0;

	drm_core_ioremap(&dev_priv->ring.map, dev);
	if (dev_priv->ring.map.handle == NULL) {
		DRM_ERROR("Failed to map ringbuffer.\n");
		(void) memset(&dev_priv->ring, 0, sizeof(dev_priv->ring));
		drm_gem_object_unreference(obj);
		i915_gem_cleanup_hws(dev);
		return EINVAL;
	}

	dev_priv->ring.ring_obj = obj;

	dev_priv->ring.virtual_start = (u8 *) dev_priv->ring.map.handle;

	/* Stop the ring if it's running. */
	I915_WRITE(PRB0_CTL, 0);
	I915_WRITE(PRB0_HEAD, 0);
	I915_WRITE(PRB0_TAIL, 0);


	/* Initialize the ring. */
	I915_WRITE(PRB0_START, obj_priv->gtt_offset);
	head = I915_READ(PRB0_HEAD) & HEAD_ADDR;

	/* G45 ring initialization fails to reset head to zero */
	if (head != 0) {
		DRM_ERROR("Ring head not reset to zero "
			  "ctl %08x head %08x tail %08x start %08x\n",
			  I915_READ(PRB0_CTL),
			  I915_READ(PRB0_HEAD),
			  I915_READ(PRB0_TAIL),
			  I915_READ(PRB0_START));
		I915_WRITE(PRB0_HEAD, 0);

		DRM_ERROR("Ring head forced to zero "
			  "ctl %08x head %08x tail %08x start %08x\n",
			  I915_READ(PRB0_CTL),
			  I915_READ(PRB0_HEAD),
			  I915_READ(PRB0_TAIL),
			  I915_READ(PRB0_START));
	}

	I915_WRITE(PRB0_CTL,
		   ((obj->size - 4096) & RING_NR_PAGES) |
		   RING_NO_REPORT |
		   RING_VALID);

	head = I915_READ(PRB0_HEAD) & HEAD_ADDR;

	/* If the head is still not zero, the ring is dead */
	if (head != 0) {
		DRM_ERROR("Ring initialization failed "
			  "ctl %08x head %08x tail %08x start %08x\n",
			  I915_READ(PRB0_CTL),
			  I915_READ(PRB0_HEAD),
			  I915_READ(PRB0_TAIL),
			  I915_READ(PRB0_START));
		return EIO;
	}

	/* Update our cache of the ring state */
	i915_kernel_lost_context(dev);

	return 0;
}

static void
i915_gem_cleanup_ringbuffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;

	if (dev_priv->ring.ring_obj == NULL)
		return;

	drm_core_ioremapfree(&dev_priv->ring.map, dev);

	i915_gem_object_unpin(dev_priv->ring.ring_obj);
	drm_gem_object_unreference(dev_priv->ring.ring_obj);
	dev_priv->ring.ring_obj = NULL;
	(void) memset(&dev_priv->ring, 0, sizeof(dev_priv->ring));
	i915_gem_cleanup_hws(dev);
}

/*ARGSUSED*/
int
i915_gem_entervt_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	if (dev_priv->mm.wedged) {
		DRM_ERROR("Reenabling wedged hardware, good luck\n");
		dev_priv->mm.wedged = 0;
	}
        /* Set up the kernel mapping for the ring. */
        dev_priv->mm.gtt_mapping.offset = dev->agp->agp_info.agpi_aperbase;
        dev_priv->mm.gtt_mapping.size = dev->agp->agp_info.agpi_apersize;
        dev_priv->mm.gtt_mapping.type = 0;
        dev_priv->mm.gtt_mapping.flags = 0;
        dev_priv->mm.gtt_mapping.mtrr = 0;

        drm_core_ioremap(&dev_priv->mm.gtt_mapping, dev);

	spin_lock(&dev->struct_mutex);
	dev_priv->mm.suspended = 0;
	ret = i915_gem_init_ringbuffer(dev);
	if (ret != 0)
		return ret;

	spin_unlock(&dev->struct_mutex);

	(void) drm_irq_install(dev);

	return 0;
}

/*ARGSUSED*/
int
i915_gem_leavevt_ioctl(DRM_IOCTL_ARGS)
{
	DRM_DEVICE;
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	if (dev->driver->use_gem != 1)
		return ENODEV;

	ret = i915_gem_idle(dev, 0);
	(void) drm_irq_uninstall(dev);

	drm_core_ioremapfree(&dev_priv->mm.gtt_mapping, dev);
	return ret;
}

void
i915_gem_lastclose(struct drm_device *dev)
{
        drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	ret = i915_gem_idle(dev, 1);
	if (ret)
		DRM_ERROR("failed to idle hardware: %d\n", ret);

	drm_mm_clean_ml(&dev_priv->mm.gtt_space);
}

void
i915_gem_load(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;

	INIT_LIST_HEAD(&dev_priv->mm.active_list);
	INIT_LIST_HEAD(&dev_priv->mm.flushing_list);
	INIT_LIST_HEAD(&dev_priv->mm.inactive_list);
	INIT_LIST_HEAD(&dev_priv->mm.request_list);
	dev_priv->mm.next_gem_seqno = 1;

	i915_gem_detect_bit_6_swizzle(dev);

}

