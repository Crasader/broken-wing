#ifndef PTATTRIBUTEENUM_H
#define PTATTRIBUTEENUM_H

#ifndef __QT__
#include "PTBaseAttributeEnum.h"

template <typename EnumType>
using PTAttributeEnum = PTBaseAttributeEnum<EnumType>;
#else
#include "qt/QAttributeEnum.h"

template <typename EnumType>
using PTAttributeEnum = QAttributeEnum<EnumType>;
#endif

#define PT_ATTRIBUTE_ENUM(ENUM_NAME) PT_ATTRIBUTE_CLASS2(PTAttribute##ENUM_NAME, PTAttributeEnum<ENUM_NAME>)
#define PT_ATTRIBUTE_ENUM2(ENUM_NAME, ENUM_TYPE) PT_ATTRIBUTE_CLASS2(PTAttribute##ENUM_NAME, PTAttributeEnum<ENUM_TYPE>)

#endif // PTATTRIBUTEENUM_H
