#ifndef PTBASEATTRIBUTEENUM_H
#define PTBASEATTRIBUTEENUM_H

#include "PTAttribute.h"

#include <type_traits>

template <typename EnumType, typename = typename std::enable_if<std::is_enum<EnumType>::value>::type>
class PTBaseAttributeEnum : public PTAttribute {
public:
    using Base = PTAttribute;
    using Type = EnumType;

    void pack(PTMessagePack &out) const override;
    void unpack(const PTMessagePack &objects) override;

    void initWithDictionary(cocos2d::CCDictionary *dictionary);
    cocos2d::CCDictionary* getDictionary() const;

    EnumType value() const noexcept;
    void setValue(EnumType value, bool silent = false);

protected:
    PTBaseAttributeEnum(const std::string &name, PTModel *model = nullptr, ValueType valueType = ExactValue);
    PTBaseAttributeEnum(const std::string &name, const PTBaseAttributeEnum<EnumType> &other);
    PTBaseAttributeEnum(PTModel *model, const PTBaseAttributeEnum<EnumType> &other);
    PTBaseAttributeEnum(const std::string &name, PTModel *model, const PTBaseAttributeEnum<EnumType> &other);

    PTBaseAttributeEnum<EnumType>& operator =(const PTBaseAttributeEnum<EnumType> &other);

protected:
    EnumType _value;
};

#include "PTBaseAttributeEnum.ipp"

#endif // PTBASEATTRIBUTEENUM_H
