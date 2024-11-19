#include "GitApplication.h"
#include <git2.h>

GitApplication::GitApplication()
{
    git_libgit2_init();
}

GitApplication::~GitApplication()
{
    git_libgit2_shutdown();
}
