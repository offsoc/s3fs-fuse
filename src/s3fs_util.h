/*
 * s3fs - FUSE-based file system backed by Amazon S3
 *
 * Copyright(C) 2007 Randy Rizun <rrizun@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef S3FS_S3FS_UTIL_H_
#define S3FS_S3FS_UTIL_H_

#include <cstdint>
#include <functional>
#include <string>
#include <sys/stat.h>

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME          0
#endif
#ifndef CLOCK_MONOTONIC
#define CLOCK_MONOTONIC         CLOCK_REALTIME
#endif
#ifndef CLOCK_MONOTONIC_COARSE
#define CLOCK_MONOTONIC_COARSE  CLOCK_MONOTONIC
#endif

//-------------------------------------------------------------------
// Functions
//-------------------------------------------------------------------
std::string get_realpath(const char *path);

void init_sysconf_vars();
std::string get_username(uid_t uid);
int is_uid_include_group(uid_t uid, gid_t gid);

std::string mydirname(const char* path);
std::string mydirname(const std::string& path);
std::string mybasename(const char* path);
std::string mybasename(const std::string& path);

int mkdirp(const std::string& path, mode_t mode);
std::string get_exist_directory_path(const std::string& path);
bool check_exist_dir_permission(const char* dirpath);
bool delete_files_in_dir(const char* dir, bool is_remove_own);

bool compare_sysname(const char* target);

void print_launch_message(int argc, char** argv);

//
// Utility for nanosecond time(timespec)
//
enum class stat_time_type : uint8_t {
    ATIME,
    MTIME,
    CTIME
};

//-------------------------------------------------------------------
// Utility for nanosecond time(timespec)
//-------------------------------------------------------------------
static constexpr struct timespec S3FS_OMIT_TS = {0, UTIME_OMIT};

constexpr int compare_timespec(const struct timespec& ts1, const struct timespec& ts2);
int compare_timespec(const struct stat& st, stat_time_type type, const struct timespec& ts);
void set_timespec_to_stat(struct stat& st, stat_time_type type, const struct timespec& ts);
struct timespec* set_stat_to_timespec(const struct stat& st, stat_time_type type, struct timespec& ts);
std::string str_stat_time(const struct stat& st, stat_time_type type);
struct timespec* s3fs_realtime(struct timespec& ts);
std::string s3fs_str_realtime();

// Wrap fclose since it is illegal to take the address of a stdlib function
int s3fs_fclose(FILE* fp);

class scope_guard {
public:
    template<class Callable>
    explicit scope_guard(Callable&& undo_func)
        : func(std::forward<Callable>(undo_func))
    {}

    ~scope_guard() {
        if(func != nullptr) {
            func();
        }
    }

    void dismiss() {
        func = nullptr;
    }

    scope_guard(const scope_guard&) = delete;
    scope_guard(scope_guard&& other) = delete;
    scope_guard& operator=(const scope_guard&) = delete;
    scope_guard& operator=(scope_guard&&) = delete;

private:
    std::function<void()> func;
};

#endif // S3FS_S3FS_UTIL_H_

/*
* Local variables:
* tab-width: 4
* c-basic-offset: 4
* End:
* vim600: expandtab sw=4 ts=4 fdm=marker
* vim<600: expandtab sw=4 ts=4
*/
