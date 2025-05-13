#include "vfs.h"
#include "kernel/libc/memory.h" // memory functions
#include "kernel/memory/kmalloc.h" // memory allocator
#include <stdbool.h>
#include <stdlib.h>
#include "kernel/framebuffer/framebuffer.h" // logging
#include "kernel/limine_setup.h" // framebuffer setup
/*
	Keystone kernel
	bingchris 2025
*/
// root of VFS
static vfs_node_t *vfs_root = NULL;

// init logging
static void vfs_init_logging(void) {
    static bool logging_initialized = false;
    if (!logging_initialized) {
        struct framebuffer *fb = get_framebuffer();
        if (fb) {
            logging_initialized = true;
        }
    }
}

// split path into components
static char *vfs_next_path_component(char *path) {
    while (*path && *path != '/') path++;
    if (*path == '/') *path++ = '\0';
    return path;
}

// mount filesystem
vfs_node_t *vfs_mount(const char *path, vfs_node_t *fs_root) {
    vfs_init_logging();

    if (!vfs_root) {
        vfs_root = fs_root;
        kprint("vfs: vfs_mount root: ", 0xffffff);
        kprint(fs_root->name, 0xffffff);
        kprint("\n", 0xffffff);
        return vfs_root;
    }

    vfs_node_t *parent = vfs_resolve(path);
    if (!parent || parent->type != VFS_TYPE_DIR) {
        kprint("vfs: vfs_mount fail: ", 0xff0000);
        kprint(path, 0xff0000);
        kprint("\n", 0xff0000);
        return NULL;
    }

    fs_root->parent = parent;
    fs_root->next = parent->children;
    parent->children = fs_root;

    kprint("vfs: vfs_mount: ", 0xffffff);
    kprint(fs_root->name, 0xffffff);
    kprint(" at: ", 0xffffff);
    kprint(path, 0xffffff);
    kprint("\n", 0xffffff);

    return fs_root;
}

// resolve path
vfs_node_t *vfs_resolve(const char *path) {
    vfs_init_logging();

    if (!vfs_root || !path || *path != '/') {
        kprint("vfs: vfs_resolve: path not valid or root is not mounted: ", 0xff0000);
        kprint(path, 0xff0000);
        kprint("\n", 0xff0000);
        return NULL;
    }

    vfs_node_t *current = vfs_root;
    char temp_path[256];
    strncpy(temp_path, path, sizeof(temp_path));
    temp_path[sizeof(temp_path) - 1] = '\0';

    char *component = temp_path;
    while (*component) {
        component = vfs_next_path_component(component);
        vfs_node_t *child = current->children;
        while (child) {
            if (strcmp(child->name, component) == 0) {
                current = child;
                break;
            }
            child = child->next;
        }
        if (!child) {
            kprint("vfs: vfs_resolve: path not found: ", 0xff0000);
            kprint(path, 0xff0000);
            kprint("\n", 0xff0000);
            return NULL;
        }
    }

    kprint("vfs: vfs_resolve: resolved path: ", 0xffffff);
    kprint(path, 0xffffff);
    kprint(" to node: ", 0xffffff);
    kprint(current->name, 0xffffff);
    kprint("\n", 0xffffff);

    return current;
}

// read from file
int vfs_read(vfs_node_t *node, size_t offset, size_t size, void *buffer) {
    vfs_init_logging();

    if (!node || !node->ops || !node->ops->read) {
        kprint("vfs: vfs_read fail: invalid node or operation\n", 0xff0000);
        return -1;
    }

    int result = node->ops->read(node, offset, size, buffer);
    kprint("vfs: vfs_read: ", 0xffffff);
    kprint(node->name, 0xffffff);
    kprint("\n", 0xffffff);
    return result;
}

// write to file
int vfs_write(vfs_node_t *node, size_t offset, size_t size, const void *buffer) {
    vfs_init_logging();

    if (!node || !node->ops || !node->ops->write) {
        kprint("vfs: vfs_write fail: invalid node or operation\n", 0xff0000);
        return -1;
    }

    int result = node->ops->write(node, offset, size, buffer);
    kprint("vfs: vfs_write: ", 0xffffff);
    kprint(node->name, 0xffffff);
    kprint("\n", 0xffffff);
    return result;
}

// open file
int vfs_open(vfs_node_t *node) {
    vfs_init_logging();

    if (!node || !node->ops || !node->ops->open) {
        kprint("vfs: vfs_open fail: invalid node or operation\n", 0xff0000);
        return -1;
    }

    int result = node->ops->open(node);
    kprint("vfs: vfs_open: ", 0xffffff);
    kprint(node->name, 0xffffff);
    kprint("\n", 0xffffff);
    return result;
}

// close file
int vfs_close(vfs_node_t *node) {
    vfs_init_logging();

    if (!node || !node->ops || !node->ops->close) {
        kprint("vfs: vfs_close fail: invalid node or operation\n", 0xff0000);
        return -1;
    }

    int result = node->ops->close(node);
    kprint("vfs: vfs_close: ", 0xffffff);
    kprint(node->name, 0xffffff);
    kprint("\n", 0xffffff);
    return result;
}

// create file
vfs_node_t *vfs_create_file(const char *name, vfs_node_t *parent) {
    vfs_node_t *file = kmalloc(sizeof(vfs_node_t));
    if (!file) {
        kprint("vfs: kmalloc fail: not enough memory for file\n", 0xff0000);
        return NULL;
    }

    memset(file, 0, sizeof(vfs_node_t));
    strncpy(file->name, name, sizeof(file->name) - 1);
    file->type = VFS_TYPE_FILE;
    file->parent = parent;
    file->ops = NULL; // no opps in the hood (im very sorry)

    file->next = parent->children;
    parent->children = file;

    kprint("vfs: vfs_create_file: ", 0xffffff);
    kprint(file->name, 0xffffff);
    kprint("\n", 0xffffff);

    return file;
}

// write to file
int vfs_write_to_file(vfs_node_t *file, const char *data, size_t size) {
    if (!file || file->type != VFS_TYPE_FILE) {
        kprint("vfs: vfs_write_to_file: invalid file\n", 0xff0000);
        return -1;
    }

    file->data = kmalloc(size);
    if (!file->data) {
        kprint("vfs: kmalloc fail: not enough memory for file\n", 0xff0000);
        return -1;
    }

    memcpy(file->data, data, size);
    file->size = size;

    kprint("vfs: vfs_write_to_file: ", 0xffffff);
    kprint(file->name, 0xffffff);
    kprint("\n", 0xffffff);

    return 0;
}

// reaf from file
int vfs_read_from_file(vfs_node_t *file, char *buffer, size_t size) {
    if (!file || file->type != VFS_TYPE_FILE || !file->data) {
        kprint("vfs: invalid file\n", 0xff0000);
        return -1;
    }

    if (size > file->size) {
        size = file->size;
    }

    memcpy(buffer, file->data, size);

    kprint("vfs: vfs_read_from_file: ", 0xffffff);
    kprint(file->name, 0xffffff);
    kprint("\n", 0xffffff);

    return size;
}
