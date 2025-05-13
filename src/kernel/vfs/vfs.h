#ifndef VFS_H
#define VFS_H
/*
	Keystone kernel
	bingchris 2025
*/
#include <stdint.h>
#include <stddef.h>

// types
#define VFS_TYPE_FILE 1
#define VFS_TYPE_DIR  2

// opps
typedef struct vfs_file_ops {
    int (*read)(void *node, size_t offset, size_t size, void *buffer);
    int (*write)(void *node, size_t offset, size_t size, const void *buffer);
    int (*open)(void *node);
    int (*close)(void *node);
} vfs_file_ops_t;

// node
typedef struct vfs_node {
    char name[128];
    uint32_t type;
    size_t size;
    struct vfs_file_ops *ops;
    struct vfs_node *parent;
    struct vfs_node *children;
    struct vfs_node *next;
    void *data;
} vfs_node_t;

vfs_node_t *vfs_mount(const char *path, vfs_node_t *fs_root);
vfs_node_t *vfs_resolve(const char *path);
int vfs_read(vfs_node_t *node, size_t offset, size_t size, void *buffer);
int vfs_write(vfs_node_t *node, size_t offset, size_t size, const void *buffer);
int vfs_open(vfs_node_t *node);
int vfs_close(vfs_node_t *node);

vfs_node_t *vfs_create_file(const char *name, vfs_node_t *parent);
int vfs_write_to_file(vfs_node_t *file, const char *data, size_t size);
int vfs_read_from_file(vfs_node_t *file, char *buffer, size_t size);

#endif
