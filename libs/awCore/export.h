#pragma once

#include <QtGlobal>

#ifdef EXPORT_AW_CORE
#define AW_CORE_API Q_DECL_EXPORT
#else
#define AW_CORE_API Q_DECL_IMPORT
#endif
