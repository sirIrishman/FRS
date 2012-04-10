#ifndef FRS.CORE_GLOBAL_H
#define FRS.CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef FRS.CORE_LIB
# define FRS.CORE_EXPORT Q_DECL_EXPORT
#else
# define FRS.CORE_EXPORT Q_DECL_IMPORT
#endif

#endif // FRS.CORE_GLOBAL_H
