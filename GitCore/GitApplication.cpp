#include "GitApplication.h"
#include <git2.h>

GitApplication::GitApplication(QObject *parent): QObject{parent}
{
    git_libgit2_init();
}

GitApplication::~GitApplication()
{
    git_libgit2_shutdown();
}
