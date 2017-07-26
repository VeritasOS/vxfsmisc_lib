README:
The vxfsmisc library is a dynamically linked shared object library that can be used to store extended attributes associated with files in Veritas File System (VxFS). The vxfsmisc library provides an alternative way of storing extended attributes which has been optimized for certain use cases, some of which match that of Samba. Just like the standard extended attribute interfaces, this library allows write, read, remove and list operations on the extended attributes. The library interfaces with VxFS via ioctls. This library is licensed under GPLv3.


Following are the definitions of each of APIs provided by the library. The caller should make a dlopen() call to library to access these APIs.

1. write: stores the extended attribute.
   int vxfs_nxattr_set(int fd,  const char * name,  const void *value,  size_t len, int mode)
   where:	
	fd: file descriptor of the file.
	name: extended attribute name.
	value: extended attribute value.
	mode: mode has three possible values:
		0: Ignore the mode.
		1: Store the extended attribute only if does not exist.
		2: Update the extended attribute only if already exists.

   Return value:
	Success: 0
	Failure: Non-zero with the return value indicating the error.

2. read: reads the extended attribute value.
   int vxfs_nxattr_get(int fd, const char *name,  void *value,  size_t *len)
   where:
	fd: file descriptor of the file.
	name: extended attribute name.
	value: buffer to store the extended attribute value.
	       This buffer should always be pre-allocated with the estimated size of the extended attribute. If the buffer is of a smaller size than the                     extended attribute length then vxfs_nxattr_get() will return the error EFBIG and the ‘len’ variable will contain the extended attribute size.                 The caller should then allocate a buffer of this size and then call vxfs_nxattr_get() again. 
	len: pointer to the size of ‘value’ mentioned above.
	     On success (or on error EFBIG) len contains the extended attribute length.

   Return value:
	Success: 0
	Failure: Non-zero with return the value indicating the error.

3. remove: removes the extended attribute
   int vxfs_nxattr_remove(int fd, const char *name)
   where:
	fd: file descriptor of the file.
	name: extended attribute name.

   Return value:
	Success: 0
	Failure: Non-zero with return the value indicating the error.

4. list: lists the extended attributes associated with a file.
   int vxfs_nxattr_list(int fd, void *value, size_t *len)
   where:
	fd: file descriptor of the file.
	value: buffer to store the extended attribute value.
	       This buffer should always be pre-allocated with the estimated size of the extended attribute list. If the buffer is of a smaller size than the                extended attribute list then vxfs_nxattr_list() will return the error EFBIG and the ‘len’ variable will contain the extended attribute list                   size. The caller should then allocate a buffer of this size and then call vxfs_nxattr_list() again. 
	len: pointer to the size  of ‘value’ mentioned above.
	     On success (or on error EFBIG) len contains the size of the extended attribute list.

   Return value:
	Success: 0
	Failure: Non-zero with return the value indicating the error.

5. worm attr set: sets the worm attribute for given file.
   int vxfs_wattr_set(int fd)
   where:
        fd: file descriptor of the file.

   Return value:
	Success: 0
	Failure: Non-zero with return the value indicating the error.

6. worm attr clear: clears the worm attribute for given file.
   int vxfs_wattr_clear(int fd)
   where:
        fd: file descriptor of the file.

   Return value:
	Success: 0
	Failure: Non-zero with return the value indicating the error.

7. check retention: checks if retention has expired.
   int vxfs_wattr_check(int fd)
   where:
        fd: file descriptor of the file.

   Return vaule:
	Success: Returns 0(retention is expired) or EPERM(retention is not expired).
	Failure: Return value indicating the error.


Compilation instructions:
1. Make sure you have the following files:
	a. nxattr.c
	b. vx_ioctl_misc.h
2. gcc -fPIC  -c -o nxattr.o nxattr.c
3. gcc -shared -o vxfsmisc.so nxattr.o
