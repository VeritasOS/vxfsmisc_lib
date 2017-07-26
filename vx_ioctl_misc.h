/*
 * Veritas Open Source
 * Project: vxfsmisc
 * Contributor: Advait Jaywant, Pooja Mahadik
 * Last modification Date: 25/07/2017.
 */

#ifndef _FS_VXFS_VX_IOCTL_MISC_H
#define _FS_VXFS_VX_IOCTL_MISC_H

/*
 * This structure carries the info. required to set/get attribute inodes.
 */

struct vx_nxattr_info {
	unsigned int	nxi_length;	/* xattr length */
	unsigned int	nxi_name_len;	/* xattr name length */
	char		*nxi_name;	/* xattr name */
	void		*nxi_value;	/* xattr data */
	int		nxi_flags;	/* Determines the type of set operation
					 * to be performed.
					 */
};

struct vx_genioctl {
	int	ioc_cmd;
	void	*ioc_up;
};

struct vx_wxattr_info {
	unsigned int	vw_tvsec;
	unsigned int	vw_tvusec;
	int		vw_flags;
};

#endif	/* _FS_VXFS_VX_IOCTL_MISC_H */
