#pragma once
#include <memory>
#include <string>
#include <vector>
#include <vix/mm/kheap.h>
#include <vix/types.h>

#define VFS_SEEK_END (1 << 0)

namespace fs::vfs {
    /*
    even newer :tm:
    */
    struct dirent {
        std::string name;
        std::shared_ptr<struct inode> inode;
    };

    struct inode {
        enum class type { REGULAR_FILE, DIRECTORY } type;
        std::vector<std::shared_ptr<struct dirent>> direntries; // TODO: we need a linked list! would be far more efficient here
    };

    struct mount {
        std::shared_ptr<struct dirent> mountpoint; // TODO: we need a linked list! would be far more efficient here
    };

    /* new */
    struct fsinfo {
        void *info; // for internal use by the filesystem

        void *(*fopen)(void *info, std::vector<std::string> *path);
        void (*fclose)(void *info, void *file);

        size_t (*fread)(void *info, void *file, void *buf, size_t count);

        size_t (*ftell)(void *info, void *file);
        void (*fseek)(void *info, void *file, size_t pos, unsigned int flags);

        /* private to VFS */
        std::vector<std::string> mount_path;
    };

    typedef struct {
        struct fsinfo mount; // do not use directly, currently we do not keep track of all open files so on every operation we have to check if this
                             // is still valid
        void *internal_file;
    } file;

    void init();

    void print_tree();

    void mount_fs(struct fsinfo fs, std::string mountpoint);

    file *fopen(std::string path);
    void fclose(file *file);

    size_t fread(file *file, void *buf, size_t count);

    size_t ftell(file *file);
    void fseek(file *file, size_t pos, unsigned int flags = 0);

    /* old */
    bool fptr(const char *path, void **fileptr);
}
