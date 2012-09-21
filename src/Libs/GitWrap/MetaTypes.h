#ifndef METATYPES_H
#define METATYPES_H

#include "Repository.h"
#include "Submodule.h"
#include "Reference.h"

// QVariant declarations
Q_DECLARE_METATYPE( Git::Repository )
Q_DECLARE_METATYPE( Git::Submodule )
Q_DECLARE_METATYPE( Git::Reference )

#endif // METATYPES_H
