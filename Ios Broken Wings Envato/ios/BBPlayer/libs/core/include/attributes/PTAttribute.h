#ifndef PTATTRIBUTE_H
#define PTATTRIBUTE_H

#ifndef __QT__
#include "PTBaseAttribute.h"

PT_ATTRIBUTE_CLASS(PTAttribute, PTBaseAttribute);
#else
#include "qt/QAttribute.h"

PT_ATTRIBUTE_CLASS(PTAttribute, QAttribute);
#endif

#endif // PTATTRIBUTE_H
