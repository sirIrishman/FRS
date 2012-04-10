#ifndef FRS_CORE_GLOBAL_H
#define FRS_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef FRS_CORE_LIB
# define FRS_CORE_EXPORT Q_DECL_EXPORT
#else
# define FRS_CORE_EXPORT Q_DECL_IMPORT
#endif

#endif // FRS_CORE_GLOBAL_H
