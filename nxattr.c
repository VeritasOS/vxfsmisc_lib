#include <vx_ioctl_misc.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define VX_IOCTL		0x56584600
#define	VX_ADMIN_GROUP		128
#define VX_NXATTR_SET		203
#define VX_NXATTR_GET		204
#define VX_NXATTR_LIST		205
#define VX_NXATTR_REMOVE	206
#define VX_WORMATTR_GET		210
#define VX_WORMATTR_CLEAR	211
#define VX_WORMATTR_SET_SPL	212

#ifdef __hpux
#define VX_ADMIN_IOCTL	_IOW(VX_IOCTL, VX_ADMIN_GROUP, struct vx_genioctl)
#else
#define VX_ADMIN_IOCTL	(VX_IOCTL | VX_ADMIN_GROUP)
#endif

int
fs_do_ioctl(
	int			fd,
	int			ioccmd,
	int			cmd,
	void			*up)
{
	struct vx_genioctl	ioc;
	int			rv;

	ioc.ioc_cmd = cmd;
	ioc.ioc_up = up;
	rv = ioctl(fd, ioccmd, &ioc);
	if (rv < 0) {
		return errno;
	}
	return 0;
}

int
fs_aioctl(
	int	fd,
	int	cmd,
	void	*up)
{
	int	error;

	error = fs_do_ioctl(fd, VX_ADMIN_IOCTL, cmd, up);
	return error;
}

/*
 * Send the ioctl to the kernel to write the xattr.
 */

int
vxfs_nxattr_set(
	int			fd,
	const char		*name,
	const void		*value,
	size_t			len,
	int			mode)
{
	struct vx_nxattr_info	attr_send;
	int			err = 0;

	/*
	 * Setup the structure with information to
	 * be sent to the kernel.
	 */

	attr_send.nxi_length = len;
	attr_send.nxi_name_len = strlen(name);
	attr_send.nxi_name = (char *)name;
	attr_send.nxi_value = (void *)value;
	attr_send.nxi_flags = mode;
	err = fs_aioctl(fd, VX_NXATTR_SET, &attr_send);
	return err;
}

int
vxfs_nxattr_get(
	int				fd,
	const char			*name,
	void				*value,
	size_t				*len)
{
	struct vx_nxattr_info		attr_get;
	int				err = 0;

	/*
	 * Setup the structure with information to
	 * be sent to the kernel.
	 */

	attr_get.nxi_length = *len;
	attr_get.nxi_name_len = strlen(name);
	attr_get.nxi_name = (char *)name;
	attr_get.nxi_value = value;
	err = fs_aioctl(fd, VX_NXATTR_GET, &attr_get);
	*len = attr_get.nxi_length;
	return err;
}

int
vxfs_nxattr_list(
	int				fd,
	void				*value,
	size_t				*len)
{
	struct vx_nxattr_info		attr_list;
	int				err = 0;

	/*
	 * Setup the structure with information to
	 * be sent to the kernel.
	 */

	attr_list.nxi_length = *len;
	attr_list.nxi_name_len = 0;
	attr_list.nxi_name = NULL;
	attr_list.nxi_value = value;
	attr_list.nxi_flags = 0;
	err = fs_aioctl(fd, VX_NXATTR_LIST, &attr_list);
	*len = attr_list.nxi_length;
	return err;
}

int
vxfs_nxattr_remove(
	int			fd,
	const char		*name)
{
	struct vx_nxattr_info		attr_rem;
	int				err = 0;

	/*
	 * Setup the structure with information to
	 * be sent to the kernel.
	 */

	attr_rem.nxi_length = 0;
	attr_rem.nxi_name_len = strlen(name);
	attr_rem.nxi_name = (char *)name;
	attr_rem.nxi_value = NULL;
	attr_rem.nxi_flags = 0;
	err = fs_aioctl(fd, VX_NXATTR_REMOVE, &attr_rem);
	return err;
}

int
vxfs_wattr_set(
	int		fd)
{
	int	err = 0;

	err = fs_aioctl(fd, VX_WORMATTR_SET_SPL, NULL);
	return err;
}

int
vxfs_wattr_clear(
	int		fd)
{
	int	err = 0;

	err = fs_aioctl(fd, VX_WORMATTR_CLEAR, NULL);
	return err;
}

/*
 * Read wattr, check if retention has expired or not and return
 * EPERM error accordingly.
 */

int
vxfs_wattr_check(
	int		fd)
{
	struct vx_wxattr_info	wattr;
	struct timeval		curt;
	int			err = 0;

	err = fs_aioctl(fd, VX_WORMATTR_GET, &wattr);
	if (err) {
		goto out;
	}
	(void) gettimeofday(&curt, NULL);
	if ((curt.tv_sec < wattr.vw_tvsec) ||
	    ((curt.tv_sec == wattr.vw_tvsec) &&
	     (curt.tv_usec < wattr.vw_tvusec))) {
		err = EPERM;
	}

out:
	return err;
}
